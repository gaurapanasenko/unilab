#ifndef GTKMM_DERIVED_WINDOW_H
#define GTKMM_DERIVED_WINDOW_H
#include <map>
#include "model.h"

class DerivedWindow : public Gtk::Window {
public:
	DerivedWindow(BaseObjectType* cobject,
	              const Glib::RefPtr<Gtk::Builder>& builder);
	virtual ~DerivedWindow();
	Glib::ustring get_function_str();
	Glib::ustring get_interval_a_str();
	Glib::ustring get_interval_b_str();
	Glib::ustring get_control_points_str();

protected:
	Model m;
	bool on_drawingarea1_draw(const Cairo::RefPtr<Cairo::Context>& cr);
	bool on_key_press(GdkEventKey* key_event);
	void on_render_button_clicked();
	Glib::RefPtr<Gtk::Builder> m_builder;
	Gtk::DrawingArea * m_drawingarea1;
	Gtk::TextView * m_textview1;
	Glib::RefPtr<Gtk::TextBuffer> m_textview1_textbuffer;
	Glib::RefPtr<Gtk::EntryBuffer>
		m_function_entry_buffer,
		m_interval_a_entry_buffer,
		m_interval_b_entry_buffer,
		m_control_points_entry_buffer;
};


#endif //GTKMM_DERIVED_WINDOW_H