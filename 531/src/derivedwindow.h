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
	Glib::ustring get_n_str();

protected:
	bool on_drawingarea1_draw(const Cairo::RefPtr<Cairo::Context>& cr);
	bool on_key_press(GdkEventKey* key_event);
	void on_render_button_clicked();
	bool draw_text(const Cairo::RefPtr<Cairo::Context>& cr,
	               const Glib::ustring& text, double x, double y,
	               int size, char xalign, char yalign);
	Model m;
	Glib::RefPtr<Gtk::Builder> m_builder;
	Gtk::DrawingArea * m_drawingarea1;
	Gtk::TextView * m_textview1;
	Gtk::Label * m_data_label;
	Glib::RefPtr<Gtk::TextBuffer> m_textview1_textbuffer;
	Glib::RefPtr<Gtk::EntryBuffer>
		m_function_entry_buffer,
		m_interval_a_entry_buffer,
		m_interval_b_entry_buffer,
		m_n_entry_buffer;
	Glib::RefPtr<Gtk::ListStore> m_liststore1;
};

void calculate_axis(const double& a, const double& b,
					long long& new_a, long long& new_b,
					long long& step, long long& enlarger);

#endif //GTKMM_DERIVED_WINDOW_H
