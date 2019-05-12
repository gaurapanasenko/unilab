/***************************************************************
 * Name:      window.cpp
 * Purpose:   Definition of Window
 * Author:    Egor Panasenko (gaura.panasenko@gmail.com)
 * Created:   2019-01-20
 * Copyright: Egor Panasenko (elfiny.top)
 * License:   GPLv3
 **************************************************************/
#include "window.h"
#include "shape-childs.h"
#include "aggregator.h"
#include <gtkmm/filechooserdialog.h>
#include <fstream>
#include <chrono>
#include <thread>
#include <ctime>

Timer::Timer(Glib::Dispatcher& dispatcher)
  : dispatcher_(dispatcher), continue_(true) {}

void Timer::doWork() {
  bool c = true;
  while(c) {
    {
      std::lock_guard<std::mutex> lock(mutex);
      c = continue_;
    }
    std::this_thread::sleep_for(std::chrono::milliseconds(10));
    dispatcher_.emit();
  }
}

void Timer::stop() {
  std::lock_guard<std::mutex> lock(mutex);
  continue_ = false;
}

template<class T>
void getObject(const Glib::RefPtr<Gtk::Builder>& builder,
               Glib::RefPtr<T>& pointer, const char* name) {
  pointer = Glib::RefPtr<T>::cast_dynamic(builder->get_object(name));
  if (!pointer) exit(1);
}

template<class A, class B>
void connectButton(const Glib::RefPtr<Gtk::Builder>& builder,
                   const char* name, A& object, B function) {
  Glib::RefPtr<Gtk::Button> tmp;
  getObject(builder, tmp, name);
  tmp->signal_clicked().connect(sigc::mem_fun(object, function));
}

Window::Window(BaseObjectType* cobject,
               Glib::RefPtr<Gtk::Builder> builder)
  : Gtk::ApplicationWindow(cobject), builder_(std::move(builder)),
    timer(dispatcher), thread_(nullptr) {
  connectButton(builder_, "save_button"         , *this, &Window::save);
  connectButton(builder_, "load_button"         , *this, &Window::load);

  connectButton(builder_, "add_rectangle_button", *this, &Window::addRectangle);
  connectButton(builder_, "add_triangle_button" , *this, &Window::addTriangle );
  connectButton(builder_, "add_ellipse_button"  , *this, &Window::addEllipse  );

  connectButton(builder_, "trace_button"        , *this, &Window::toggleTrace );
  connectButton(builder_, "reset_button"        , *this, &Window::reset       );
  connectButton(builder_, "color_button"        , *this, &Window::changeColor );
  connectButton(builder_, "visibility_button"   , *this, &Window::hideOrShow  );
  connectButton(builder_, "aggregate_button"    , *this, &Window::aggregate   );
  connectButton(builder_, "deaggregate_button"  , *this, &Window::deaggregate );
  connectButton(builder_, "clone_button"        , *this, &Window::cloneShape  );
  connectButton(builder_, "delete_button"       , *this, &Window::deleteShape );
  connectButton(builder_, "automove_button"   , *this, &Window::toggleAutomove);

  getObject(builder_, drawingArea_, "drawing_area");
  getObject(builder_, statusbar_, "statusbar");


  getObject(builder_, nAdjustment_          , "n_adjustment");
  getObject(builder_, xAdjustment_          , "x_adjustment");
  getObject(builder_, yAdjustment_          , "y_adjustment");
  getObject(builder_, widthAdjustment_      , "width_adjustment");
  getObject(builder_, heightAdjustment_     , "height_adjustment");
  getObject(builder_, minimumZoomAdjustment_, "minimum_zoom_adjustment");
  getObject(builder_, traceSizeAdjustment_  , "trace_size_adjustment");
  getObject(builder_, traceTimeAdjustment_  , "trace_time_adjustment");

  auto mfpc = sigc::mem_fun(*this, &Window::parametersChanged);
            xAdjustment_->signal_value_changed().connect(mfpc);
            yAdjustment_->signal_value_changed().connect(mfpc);
        widthAdjustment_->signal_value_changed().connect(mfpc);
       heightAdjustment_->signal_value_changed().connect(mfpc);
  minimumZoomAdjustment_->signal_value_changed().connect(mfpc);
    traceSizeAdjustment_->signal_value_changed().connect(mfpc);
    traceTimeAdjustment_->signal_value_changed().connect(mfpc);

  drawingArea_->add_events(
        Gdk::BUTTON_PRESS_MASK | Gdk::BUTTON_MOTION_MASK |
        Gdk::BUTTON_RELEASE_MASK | Gdk::SCROLL_MASK);
  this->add_events(Gdk::KEY_PRESS_MASK | Gdk::KEY_RELEASE_MASK);

  auto mfd = sigc::mem_fun(*this, &Window::draw);
  auto mfa = sigc::mem_fun(*this, &Window::activate);
  auto mfm = sigc::mem_fun(*this, &Window::moveActive);
  auto mfr = sigc::mem_fun(*this, &Window::release);
  auto mfs = sigc::mem_fun(*this, &Window::scrollZoom);

  drawingArea_->signal_draw().connect(mfd);
  drawingArea_->signal_button_press_event().connect(mfa);
  drawingArea_->signal_motion_notify_event().connect(mfm);
  drawingArea_->signal_button_release_event().connect(mfr);
  drawingArea_->signal_scroll_event().connect(mfs);

  ShapesMap map;
  map["Triangle"] = ShapeChilds::Triangle::create;
  map["Rectangle"] = ShapeChilds::Rectangle::create;
  map["Ellipse"] = ShapeChilds::Ellipse::create;
  map["Aggregator"] = Aggregator::create;
  SHAPES_REGISTRY.setShapesMap(map);

  dispatcher.connect(sigc::mem_fun(*this, &Window::update));
  thread_ = new std::thread(&Timer::doWork, &timer);
  parametersChanged();
}

Window::~Window() {
  if (thread_ && thread_->joinable()) {
    timer.stop();
    thread_->join();
  }
  delete thread_;
}

void Window::quit[[noreturn]]() {
  exit(0);
}

void Window::update() {
  drawingArea_->queue_draw();
}

bool Window::draw(const Cairo::RefPtr<Cairo::Context>& context) {
  if (context) {
    Gtk::Allocation allocation = drawingArea_->get_allocation();
    SHAPE.setMaximumWidth(float(allocation.get_width() - 4));
    SHAPE.setMaximumHeight(float(allocation.get_height() - 4));
    SHAPE.setDefaultMatrix(context->get_matrix());
    context->translate(2, 2);
    context->set_antialias(Cairo::ANTIALIAS_NONE);
    context->set_line_width(3);
    context->set_line_cap(Cairo::LINE_CAP_ROUND);
    shapes_.draw(context);
  }
  return true;
}

void Window::parametersChanged() {
  auto tsa = traceSizeAdjustment_;
  SHAPE.setX(float(xAdjustment_->get_value()));
  SHAPE.setY(float(yAdjustment_->get_value()));
  SHAPE.setMaximumWidth(float(widthAdjustment_->get_value()) + 4);
  SHAPE.setMaximumHeight(float(heightAdjustment_->get_value()) + 4);
  SHAPE.setDefaultWidth(float(widthAdjustment_->get_value()));
  SHAPE.setDefaultHeight(float(heightAdjustment_->get_value()));
  SHAPE.setMinimumZoom(float(minimumZoomAdjustment_->get_value()));
  SHAPE.setTraceSize(static_cast<unsigned char>(tsa->get_value()));
  SHAPE.setTraceTime(float(traceTimeAdjustment_->get_value()));
  update();
}

void Window::save() {
  Gtk::FileChooserDialog dialog("Please choose a folder",
           Gtk::FILE_CHOOSER_ACTION_SAVE);
  dialog.set_transient_for(*this);

  dialog.add_button("_Cancel", Gtk::RESPONSE_CANCEL);
  dialog.add_button("Save", Gtk::RESPONSE_OK);

  int result = dialog.run();

  if (result == Gtk::RESPONSE_OK) {
    std::ofstream out(dialog.get_filename());
    out << shapes_;
  }
}


void Window::load() {
  Gtk::FileChooserDialog dialog("Please choose a folder",
           Gtk::FILE_CHOOSER_ACTION_OPEN);
  dialog.set_transient_for(*this);

  dialog.add_button("_Cancel", Gtk::RESPONSE_CANCEL);
  dialog.add_button("Open", Gtk::RESPONSE_OK);

  int result = dialog.run();
  if (result == Gtk::RESPONSE_OK) {
    std::ifstream in(dialog.get_filename());
    in >> shapes_;
  }
}

void Window::addRectangle() {
  int n = int(nAdjustment_->get_value());
  for (int i = 0; i < n; i++) {
    shapes_.add(ShapeChilds::Rectangle::create());
  }
  update();
}

void Window::addTriangle() {
  int n = int(nAdjustment_->get_value());
  for (int i = 0; i < n; i++) {
    shapes_.add(ShapeChilds::Triangle::create());
  }
  update();
}

void Window::addEllipse() {
  int n = int(nAdjustment_->get_value());
  for (int i = 0; i < n; i++) {
    shapes_.add(ShapeChilds::Ellipse::create());
  }
  update();
}

void Window::toggleAutomove() {
  auto s = shapes_.getActive();
  if (s) {
    s->toggleAutomove();
    update();
  }
}

void Window::toggleTrace() {
  auto s = shapes_.getActive();
  if (s) {
    s->toggleTrace();
    update();
  }
}

void Window::reset() {
  auto s = shapes_.getActive();
  if (s) {
    s->reset();
    update();
  }
}

void Window::changeColor() {
  auto s = shapes_.getActive();
  if (s) {
    s->changeColor();
    update();
  }
}

void Window::hideOrShow() {
  auto s = shapes_.getActive();
  if (s) {
    s->toggleVisibility();
    update();
  }
}

void Window::aggregate() {
  auto arr = shapes_.getSelected();
  if (!arr.empty()) {
    if (arr.size() > 1) {
      shapes_.add(Aggregator::create(arr));
      update();
    } else {
      shapes_.add(arr[0]);
      update();
    }
  }
}

void Window::deaggregate() {
  auto arr = shapes_.getSelected();
  for (auto& i : arr) {
    if (i) {
      auto a = dynamic_cast<Aggregator*>(i.operator->());
      if (a) {
        auto iarr = a->deaggregate();
        for (auto& j : iarr) {
          shapes_.add(j);
        }
      } else {
        shapes_.add(i);
      }
    }
  }
  if (!arr.empty())
    update();
}

void Window::cloneShape() {
  auto s = shapes_.getActive();
  if (s) {
    int n = int(nAdjustment_->get_value());
    for (int i = 0; i < n; i++) {
      shapes_.add(s->clone());
    }
  }
  update();
}

void Window::deleteShape() {
  shapes_.erase(shapes_.getActiveIterator());
  update();
}

bool Window::activate(GdkEventButton* event) {
  if (event->type == GDK_BUTTON_PRESS) {
    auto p = Point(float(event->x), float(event->y));
    if (event->button == 1) {
      shapes_.activate(p);
      auto a = shapes_.getActive();
      if (a) {
        a->startRecordingPath();
      }
    } else if (event->button == 3) {
      shapes_.toggleSelection(shapes_.getTopIterator(p));
    }
    update();
  }
  return true;
}

bool Window::moveActive(GdkEventMotion* event) {
  shapes_.moveActive(Point(float(event->x), float(event->y)));
  update();
  return true;
}

bool Window::release(GdkEventButton*) {
  auto a = shapes_.getActive();
  if (a) {
    a->stopRecordingPath();
  }
  shapes_.release();
  update();
  return true;
}

bool Window::scrollZoom(GdkEventScroll* event) {
  auto p = Point(float(event->x), float(event->y));
  auto shape = shapes_.getTop(p);
  if (shape) {
    float diff = 2.0f;
    Size size = shape->getSize();
    switch (event->direction) {
    case GDK_SCROLL_UP:
      if (!(event->state & GDK_CONTROL_MASK)) {
        size.setY(size.getY() + diff);
      } else {
        size.setX(size.getX() + diff);
      }
      break;
    case GDK_SCROLL_DOWN:
      if (!(event->state & GDK_CONTROL_MASK)) {
        size.setY(size.getY() - diff);
      } else {
        size.setX(size.getX() - diff);
      }
      break;
    case GDK_SCROLL_LEFT:
      size.setX(size.getX() + diff);
      break;
    case GDK_SCROLL_RIGHT:
      size.setX(size.getX() - diff);
      break;
    default:
      break;
    }
    shape->setSize(size);
    update();
  }
  return true;
}
