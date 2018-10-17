#ifndef GTKMM_DERIVED_WINDOW_H
#define GTKMM_DERIVED_WINDOW_H
#include "model.h"

class Col : public Gtk::TreeModel::ColumnRecord {
public:
	Col();
	Gtk::TreeModelColumn<Glib::ustring>& operator()(const int& i, bool);
	Gtk::TreeModelColumn<double>& operator()(const int& i);
private:
	Gtk::TreeModelColumn<Glib::ustring> s[8];
	Gtk::TreeModelColumn<double> d[8];
};

class DerivedWindow : public Gtk::Window {
public:
	DerivedWindow(BaseObjectType* cobject,
	              const Glib::RefPtr<Gtk::Builder>& builder);
	~DerivedWindow();
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
	bool draw_map (const Cairo::RefPtr<Cairo::Context>& cr,
	               const std::map<real, real>::const_iterator& begin,
	               const std::map<real, real>::const_iterator& end);
	void set(Gtk::TreeRow& row, const int& i, const real& a);
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
	Col cols;
};

void calculate_axis(const real& a, const real& b,
					long long& new_a, long long& new_b,
					long long& step, long long& enlarger);


#endif //GTKMM_DERIVED_WINDOW_H
