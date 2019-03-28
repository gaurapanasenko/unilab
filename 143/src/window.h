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

class Timer
{
public:
	Timer(Glib::Dispatcher& dispatcher);
	void do_work();

private:
	Glib::Dispatcher& dispatcher_;
};

class Window : public Gtk::ApplicationWindow {
public:
	Window(
		BaseObjectType* cobject,
		const Glib::RefPtr<Gtk::Builder>& builder
	);
	~Window();

	void quit();

private:
	void update();
	bool draw(const Cairo::RefPtr<Cairo::Context>& context);
	void parametersChanged();
	void addRectangle();
	void addTriangle();
	void addEllipse();
	void toggleTrace();
	void reset();
	void changeColor();
	void toggleVisibility();
	void zoomShape();
	void cloneShape();
	void deleteShape();
	bool activate(GdkEventButton* event);
	bool moveActive(GdkEventMotion* event);
	bool release(GdkEventButton* event);

	Glib::RefPtr<Gtk::Builder> builder_;
	Glib::RefPtr<Gtk::Adjustment> nAdjustment_;
	Glib::RefPtr<Gtk::Adjustment> xAdjustment_;
	Glib::RefPtr<Gtk::Adjustment> yAdjustment_;
	Glib::RefPtr<Gtk::Adjustment> widthAdjustment_;
	Glib::RefPtr<Gtk::Adjustment> heightAdjustment_;
	Glib::RefPtr<Gtk::Adjustment> minimumZoomAdjustment_;
	Glib::RefPtr<Gtk::Adjustment> traceSizeAdjustment_;
	Glib::RefPtr<Gtk::Adjustment> traceTimeAdjustment_;
	Glib::RefPtr<Gtk::DrawingArea> drawingArea_;
	Glib::RefPtr<Gtk::Statusbar> statusbar_;
	Shapes shapes_;
	Glib::Dispatcher dispatcher;
	Timer timer;
};

#endif //WINDOW_H
