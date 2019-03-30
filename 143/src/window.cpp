/***************************************************************
 * Name:      window.cpp
 * Purpose:   Implementation of window widget
 * Author:    Egor Panasenko (gaura.panasenko@gmail.com)
 * Created:   2019-01-20
 * Copyright: Egor Panasenko (elfiny.top)
 * License:   GPLv3
 **************************************************************/
#include "window.h"
#include "shape-childs.h"
#include "libgaura.h"
#include <chrono>
#include <thread>
#include <utility>

Timer::Timer(Glib::Dispatcher& dispatcher) : dispatcher_(dispatcher) {}

void Timer::do_work() {
	while (true) {
		std::this_thread::sleep_for(std::chrono::milliseconds(10));
		dispatcher_.emit();
	}
}

template<class T>
void getObject(
  const Glib::RefPtr<Gtk::Builder>& builder, Glib::RefPtr<T>& pointer,
  const char* name
) {
	pointer = Glib::RefPtr<T>::cast_dynamic(
		builder->get_object(name)
	);
  if (!pointer) {
		exit(1);
	}
}

Window::Window(
	BaseObjectType* cobject,
  Glib::RefPtr<Gtk::Builder> builder
) : Gtk::ApplicationWindow(cobject), builder_(std::move(builder)),
timer(dispatcher) {
	{
		auto tmp =  Glib::RefPtr<Gtk::Button>::cast_dynamic(
			builder_->get_object("add_rectangle_button")
		);
		if (tmp) {
			tmp->signal_clicked()
				.connect(sigc::mem_fun(*this, &Window::addRectangle));
		}
	}
	{
		auto tmp =  Glib::RefPtr<Gtk::Button>::cast_dynamic(
			builder_->get_object("add_triangle_button")
		);
		if (tmp) {
			tmp->signal_clicked()
				.connect(sigc::mem_fun(*this, &Window::addTriangle));
		}
	}
	{
		auto tmp =  Glib::RefPtr<Gtk::Button>::cast_dynamic(
			builder_->get_object("add_ellipse_button")
		);
		if (tmp) {
			tmp->signal_clicked()
				.connect(sigc::mem_fun(*this, &Window::addEllipse));
		}
	}
	{
		auto tmp =  Glib::RefPtr<Gtk::Button>::cast_dynamic(
			builder_->get_object("trace_button")
		);
		if (tmp) {
			tmp->signal_clicked()
				.connect(sigc::mem_fun(*this, &Window::toggleTrace));
		}
	}
	{
		auto tmp =  Glib::RefPtr<Gtk::Button>::cast_dynamic(
			builder_->get_object("reset_button")
		);
		if (tmp) {
			tmp->signal_clicked()
				.connect(sigc::mem_fun(*this, &Window::reset));
		}
	}
	{
		auto tmp =  Glib::RefPtr<Gtk::Button>::cast_dynamic(
			builder_->get_object("color_button")
		);
		if (tmp) {
			tmp->signal_clicked()
				.connect(sigc::mem_fun(*this, &Window::changeColor));
		}
	}
	{
		auto tmp =  Glib::RefPtr<Gtk::Button>::cast_dynamic(
			builder_->get_object("visibility_button")
		);
		if (tmp) {
			tmp->signal_clicked()
				.connect(sigc::mem_fun(*this, &Window::toggleVisibility));
		}
	}
	{
		auto tmp =  Glib::RefPtr<Gtk::Button>::cast_dynamic(
			builder_->get_object("zoom_button")
		);
		if (tmp) {
			tmp->signal_clicked()
				.connect(sigc::mem_fun(*this, &Window::zoomShape));
		}
	}
	{
		auto tmp =  Glib::RefPtr<Gtk::Button>::cast_dynamic(
			builder_->get_object("clone_button")
		);
		if (tmp) {
			tmp->signal_clicked()
				.connect(sigc::mem_fun(*this, &Window::cloneShape));
		}
	}
	{
		auto tmp =  Glib::RefPtr<Gtk::Button>::cast_dynamic(
			builder_->get_object("delete_button")
		);
		if (tmp) {
			tmp->signal_clicked()
				.connect(sigc::mem_fun(*this, &Window::deleteShape));
		}
	}

	getObject(builder_, nAdjustment_, "n_adjustment");
	getObject(builder_, drawingArea_, "drawing_area");
	getObject(builder_, statusbar_, "statusbar");


	getObject(builder_, xAdjustment_, "x_adjustment");
	xAdjustment_->signal_value_changed().connect(sigc::mem_fun(
		*this, &Window::parametersChanged
	));
	getObject(builder_, yAdjustment_, "y_adjustment");
	yAdjustment_->signal_value_changed().connect(sigc::mem_fun(
		*this, &Window::parametersChanged
	));
	getObject(builder_, widthAdjustment_, "width_adjustment");
	widthAdjustment_->signal_value_changed().connect(sigc::mem_fun(
		*this, &Window::parametersChanged
	));
	getObject(builder_, heightAdjustment_, "height_adjustment");
	heightAdjustment_->signal_value_changed().connect(sigc::mem_fun(
		*this, &Window::parametersChanged
	));
	getObject(builder_, minimumZoomAdjustment_, "minimum_zoom_adjustment");
	minimumZoomAdjustment_->signal_value_changed().connect(sigc::mem_fun(
		*this, &Window::parametersChanged
	));
	getObject(builder_, traceSizeAdjustment_, "trace_size_adjustment");
	traceSizeAdjustment_->signal_value_changed().connect(sigc::mem_fun(
		*this, &Window::parametersChanged
	));
	getObject(builder_, traceTimeAdjustment_, "trace_time_adjustment");
	traceTimeAdjustment_->signal_value_changed().connect(sigc::mem_fun(
		*this, &Window::parametersChanged
	));

	drawingArea_->add_events(
		Gdk::BUTTON_PRESS_MASK |
		Gdk::BUTTON_MOTION_MASK |
		Gdk::BUTTON_RELEASE_MASK |
		Gdk::SCROLL_MASK
	);
	drawingArea_->signal_draw().connect(sigc::mem_fun(
		*this, &Window::draw
	));
	drawingArea_->signal_button_press_event().connect(sigc::mem_fun(
		*this, &Window::activate
	));
	drawingArea_->signal_motion_notify_event().connect(sigc::mem_fun(
		*this, &Window::moveActive
	));
	drawingArea_->signal_button_release_event().connect(sigc::mem_fun(
		*this, &Window::release
	));
	drawingArea_->signal_scroll_event().connect(sigc::mem_fun(
		*this, &Window::scrollZoom
	));

	dispatcher.connect(sigc::mem_fun(*this, &Window::update));
	new std::thread(&Timer::do_work, &timer);
	parametersChanged();
}

Window::~Window() = default;

void Window::quit[[noreturn]]() {
	exit(0);
}

void Window::update() {
	drawingArea_->queue_draw();
}

bool Window::draw(const Cairo::RefPtr<Cairo::Context>& context) {
	Gtk::Allocation allocation = drawingArea_->get_allocation();
	SHAPE.setContextWidth(allocation.get_width());
	SHAPE.setContextHeight(allocation.get_height());
	SHAPE.setDefaultMatrix(context->get_matrix());
	context->set_antialias(Cairo::ANTIALIAS_NONE);
	context->set_line_width(3);
	context->set_line_cap(Cairo::LINE_CAP_ROUND);
	shapes_.draw(context);
	return true;
}

void Window::parametersChanged() {
  SHAPE.setX(float(xAdjustment_->get_value()));
  SHAPE.setY(float(yAdjustment_->get_value()));
  SHAPE.setWidth(float(widthAdjustment_->get_value()));
  SHAPE.setHeight(float(heightAdjustment_->get_value()));
  SHAPE.setMinimumZoom(float(minimumZoomAdjustment_->get_value()));
  SHAPE.setTraceSize(
    static_cast<unsigned char>(traceSizeAdjustment_->get_value())
  );
  SHAPE.setTraceTime(float(traceTimeAdjustment_->get_value()));
	update();
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
		shapes_.add(ShapeChilds::Aggregator::create());
	}
	update();
}

void Window::toggleTrace() {
	try {
		shapes_.getActive().toggleTrace();
  } catch(const GauraException&) {}
	update();
}

void Window::reset() {
	try {
		shapes_.getActive().reset();
  } catch(const GauraException&) {}
	update();
}

void Window::changeColor() {
	try {
		shapes_.getActive().changeColor();
  } catch(const GauraException&) {}
	update();
}

void Window::toggleVisibility() {
	try {
		shapes_.getActive().toggleVisibility();
  } catch(const GauraException&) {}
	update();
}

void Window::zoomShape() {
	try {
		shapes_.getActive().toggleDefaultZoom();
  } catch(const GauraException&) {}
	update();
}

void Window::cloneShape() {
	try {
    int n = int(nAdjustment_->get_value());
		for (int i = 0; i < n; i++) {
			shapes_.add(shapes_.getActive().clone());
		}
  } catch(const GauraException&) {}
	update();
}

void Window::deleteShape() {
	shapes_.erase(shapes_.getActiveId());
	update();
}

bool Window::activate(GdkEventButton* event) {
	try {
		if (event->type == GDK_BUTTON_PRESS) {
			if (event->button == 1) {
        shapes_.activate(Point(float(event->x), float(event->y)));
				update();
			} else if (event->button == 3) {
        shapes_.getTop(
              Point(float(event->x), float(event->y))
        ).toggleSelection();
				update();
			}
		}
  } catch(const GauraException&) {}
	return true;
}

bool Window::moveActive(GdkEventMotion* event) {
  shapes_.moveActive(Point(float(event->x), float(event->y)));
	update();
	return true;
}

bool Window::release(GdkEventButton*) {
	shapes_.release();
	update();
	return true;
}

bool Window::scrollZoom(GdkEventScroll* event) {
	try {
    Shape& shape = shapes_.getTop(
      Point(float(event->x), float(event->y))
    );
    float diff = 1000000.0f / event->time;
		if (event->direction == GDK_SCROLL_UP) {
		} else if (event->direction == GDK_SCROLL_DOWN) {
			diff = -diff;
		} else {
			return true;
		}
		shape.setDefaultZoom(shape.getDefaultZoom() + diff);
  } catch(const GauraException&) {}
	update();
	return true;
}
