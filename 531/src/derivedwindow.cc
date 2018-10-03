#include "derivedwindow.h"

DerivedWindow::DerivedWindow(BaseObjectType* cobject,
                             const Glib::RefPtr<Gtk::Builder>& builder)
: Gtk::Window(cobject),
m(),
m_builder(builder),
m_drawingarea1(nullptr),
m_textview1(nullptr) {
	m_builder->get_widget("drawingarea1", m_drawingarea1);
	if(m_drawingarea1)
		m_drawingarea1->signal_draw()
			.connect(sigc::mem_fun(*this,&DerivedWindow::on_drawingarea1_draw));
	else exit(1);
	m_builder->get_widget("textview1", m_textview1);
	if (!m_textview1) exit(1);
	m_textview1_textbuffer = m_textview1->get_buffer();
	Gtk::Entry * entry;
	m_builder->get_widget("function_entry", entry);
	if (!entry) exit(1);
	entry->signal_activate().connect(sigc::mem_fun(*this,
		                             &DerivedWindow::on_render_button_clicked));
	m_function_entry_buffer = entry->get_buffer();
	m_function_entry_buffer->set_text("cos(x)*sqrt(x)");
	m_builder->get_widget("interval_a_entry", entry);
	if (!entry) exit(1);
	entry->signal_activate().connect(sigc::mem_fun(*this,
		                             &DerivedWindow::on_render_button_clicked));
	m_interval_a_entry_buffer = entry->get_buffer();
	m_interval_a_entry_buffer->set_text("0");
	m_builder->get_widget("interval_b_entry", entry);
	if (!entry) exit(1);
	entry->signal_activate().connect(sigc::mem_fun(*this,
		                             &DerivedWindow::on_render_button_clicked));
	m_interval_b_entry_buffer = entry->get_buffer();
	m_interval_b_entry_buffer->set_text("20");
	m_builder->get_widget("control_points_entry", entry);
	if (!entry) exit(1);
	entry->signal_activate().connect(sigc::mem_fun(*this,
		                             &DerivedWindow::on_render_button_clicked));
	m_control_points_entry_buffer = entry->get_buffer();
	m_control_points_entry_buffer->set_text("0 2 4 6 8 10");
	Gtk::Button * btn = nullptr;
	m_builder->get_widget("render_button", btn);
	if (!btn) exit(1);
	btn->signal_clicked().connect(sigc::mem_fun(*this,
		                             &DerivedWindow::on_render_button_clicked)); 
	this->signal_key_press_event().connect(
		sigc::mem_fun(*this, &DerivedWindow::on_key_press));
}

DerivedWindow::~DerivedWindow() {}

Glib::ustring DerivedWindow::get_function_str() {
	return m_function_entry_buffer->get_text();
}

Glib::ustring DerivedWindow::get_interval_a_str() {
	return m_interval_a_entry_buffer->get_text();
}

Glib::ustring DerivedWindow::get_interval_b_str() {
	return m_interval_b_entry_buffer->get_text();
}

Glib::ustring DerivedWindow::get_control_points_str() {
	return m_control_points_entry_buffer->get_text();
}

bool DerivedWindow::on_drawingarea1_draw
(const Cairo::RefPtr<Cairo::Context>& cr) {
	Gtk::Allocation allocation = m_drawingarea1->get_allocation();
	const int width = allocation.get_width();
	const int height = allocation.get_height();

	// draw red lines out from the center of the window
	m.update(get_function_str(), get_interval_a_str(),
	         get_interval_b_str(), get_control_points_str());
	std::map<double, double> fx;
	double a = m.get_a(), b = m.get_b(), min = 0, max = 0;
	double dx = width / (b - a);
	double ii = 1 / dx / 4, tmp;
	for (double i = a; i <= b; i += ii) {
		tmp = m('f',i);
		fx[i] = tmp;
		if (tmp > max) max = tmp;
		if (tmp < min) min = tmp;
	}
	double dy = height / (max - min);
	cr->set_line_width(8);
	cr->set_source_rgb(0.2, 0.8, 0.2);
	for (std::map<double, double>::iterator it=fx.begin(); it!=fx.end(); it++)
		if (it == fx.begin())
			cr->move_to((it->first - a) * dx, (-it->second - min) * dy);
		else cr->line_to((it->first - a) * dx, (-it->second - min) * dy);
	cr->stroke();
	cr->set_line_width(4);
	std::valarray<double> dashes = {10.0, 10.0};
	cr->set_dash(dashes, -50);
	cr->set_source_rgb(0.2, 0.2, 0.2);
	for (double i = a; i < b; i += ii)
		if (i == a) cr->move_to((i - a) * dx, (-m('L', i) - min) * dy);
		else cr->line_to((i - a) * dx, (-m('L', i) - min) * dy);
	cr->stroke();
	const std::vector<double>& cp = m.get_cp();
	cr->set_source_rgb(1, 0, 0);
	for (size_t i = 0; i < cp.size(); i++) {
		cr->arc((cp[i] - a) * dx, (-m('f',cp[i]) - min) * dy, 10, 0, 2 * M_PI);
		cr->fill();
	}
	m_textview1_textbuffer->set_text(m.lagrange_str());
	return true;
}

bool DerivedWindow::on_key_press(GdkEventKey* key_event) {
	/*switch (key_event->keyval) {
		case Gdk::keysyms.Enter:
			m_drawingarea1->queue_draw();
			break;
	}*/
	return true;
}

void DerivedWindow::on_render_button_clicked() {
	m_drawingarea1->queue_draw();
}