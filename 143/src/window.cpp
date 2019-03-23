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

template<class T>
class Interface {

};

template<class T>
void getObject(
	Glib::RefPtr<Gtk::Builder> builder, Glib::RefPtr<T>& pointer, const char* name
) {
	pointer = Glib::RefPtr<T>::cast_dynamic(
		builder->get_object(name)
	);
	if (!pointer) {
		printf("No %s\n", name);
		exit(1);
	}
}

Window::Window(
	BaseObjectType* cobject,
	const Glib::RefPtr<Gtk::Builder>& builder
) : Gtk::ApplicationWindow(cobject), builder_(builder) {
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
			builder_->get_object("add_circle_button")
		);
		if (tmp) {
			tmp->signal_clicked()
				.connect(sigc::mem_fun(*this, &Window::addCircle));
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

	getObject(builder_, adjustment_, "adjustment1");
	getObject(builder_, drawingArea_, "drawing_area");
	getObject(builder_, statusbar_, "statusbar");

	drawingArea_->add_events(
		Gdk::BUTTON_PRESS_MASK |
		Gdk::BUTTON_MOTION_MASK |
		Gdk::BUTTON_RELEASE_MASK
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
}

Window::~Window() {
}

void Window::quit() {
	exit(0);
}

bool Window::draw(const Cairo::RefPtr<Cairo::Context>& context) {
	Gtk::Allocation allocation = drawingArea_->get_allocation();
	shapes_.draw(context, allocation);
	return true;
}

void Window::addRectangle() {
	int n = adjustment_->get_value();
	for (int i = 0; i < n; i++) {
		shapes_.add(ShapeChilds::Rectangle::create());
	}
	drawingArea_->queue_draw();
}

void Window::addTriangle() {
	int n = adjustment_->get_value();
	for (int i = 0; i < n; i++) {
		shapes_.add(ShapeChilds::Triangle::create());
	}
	drawingArea_->queue_draw();
}

void Window::addCircle() {
	int n = adjustment_->get_value();
	for (int i = 0; i < n; i++) {
		shapes_.add(ShapeChilds::Circle::create());
	}
	drawingArea_->queue_draw();
}

void Window::toggleTrace() {
	try {
		shapes_.getActive().toggleTrace();
	} catch(const gauraException&) {}
	drawingArea_->queue_draw();
}

void Window::reset() {
	try {
		shapes_.getActive().resetColor();
	} catch(const gauraException&) {}
	drawingArea_->queue_draw();
}

void Window::changeColor() {
	try {
		shapes_.getActive().changeColor();
	} catch(const gauraException&) {}
	drawingArea_->queue_draw();
}

void Window::toggleVisibility() {
	try {
		shapes_.getActive().toggleVisibility();
	} catch(const gauraException&) {}
	drawingArea_->queue_draw();
}

void Window::cloneShape() {
	try {
		shapes_.add(shapes_.getActive().clone());
	} catch(const gauraException&) {}
	drawingArea_->queue_draw();
}

void Window::deleteShape() {
	shapes_.erase(shapes_.getActiveId());
	drawingArea_->queue_draw();
}

bool Window::activate(GdkEventButton* event) {
	if ((event->type == GDK_BUTTON_PRESS) && (event->button == 1)) {
		shapes_.activate(Point(event->x, event->y));
		drawingArea_->queue_draw();
	}
	return true;
}

bool Window::moveActive(GdkEventMotion* event) {
	shapes_.moveActive(Point(event->x, event->y));
	drawingArea_->queue_draw();
	return true;
}

bool Window::release(GdkEventButton* event) {
	shapes_.release();
	drawingArea_->queue_draw();
	return true;
}
