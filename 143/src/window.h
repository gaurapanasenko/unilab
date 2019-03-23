/***************************************************************
 * Name:      window.h
 * Purpose:   Defines window widget
 * Author:    Egor Panasenko (gaura.panasenko@gmail.com)
 * Created:   2019-01-20
 * Copyright: Egor Panasenko (elfiny.top)
 * License:   GPLv3
 **************************************************************/
#ifndef WINDOW_H
#define WINDOW_H

#include <gtkmm.h>
#include "shapes.h"

class Window : public Gtk::ApplicationWindow {
public:
	Window(
		BaseObjectType* cobject,
		const Glib::RefPtr<Gtk::Builder>& builder
	);
	~Window();

	void quit();

private:
	bool draw(const Cairo::RefPtr<Cairo::Context>& context);
	void addRectangle();
	void addTriangle();
	void addCircle();
	void toggleTrace();
	void reset();
	void changeColor();
	void toggleVisibility();
	void cloneShape();
	void deleteShape();
	bool activate(GdkEventButton* event);
	bool moveActive(GdkEventMotion* event);
	bool release(GdkEventButton* event);

	Glib::RefPtr<Gtk::Builder> builder_;
	Glib::RefPtr<Gtk::Adjustment> adjustment_;
	Glib::RefPtr<Gtk::DrawingArea> drawingArea_;
	Glib::RefPtr<Gtk::Statusbar> statusbar_;
	Shapes shapes_;
};

#endif //WINDOW_H
