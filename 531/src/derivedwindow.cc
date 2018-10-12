#include "derivedwindow.h"

Col::Col() {
	for (int i = 0; i < 8; i++) {
		add(s[i]); add(d[i]);
	}
}
Gtk::TreeModelColumn<Glib::ustring>& Col::operator()(const int& i,
                                                     bool) {
	return s[i];
}
Gtk::TreeModelColumn<double>& Col::operator()(const int& i) {
	return d[i];
}

template<typename T>
Glib::ustring to_string(T t)
{
	std::ostringstream oss;
	oss << t;
	return oss.str();
}

DerivedWindow::DerivedWindow(BaseObjectType* cobject,
                             const Glib::RefPtr<Gtk::Builder>& builder)
		: Gtk::Window(cobject),
		m(),
		m_builder(builder),
		m_drawingarea1(nullptr),
		m_textview1(nullptr),
		m_data_label(nullptr),
		m_liststore1(nullptr) {
	m_builder->get_widget("drawingarea1", m_drawingarea1);
	if(m_drawingarea1)
		m_drawingarea1->signal_draw()
			.connect(sigc::mem_fun(*this,
                                 &DerivedWindow::on_drawingarea1_draw));
	else exit(1);
	m_builder->get_widget("data_label", m_data_label);
	if (!m_data_label) exit(1);
	m_liststore1 = Glib::RefPtr<Gtk::ListStore>::cast_dynamic
		(m_builder->get_object("liststore1"));
	if (!m_liststore1) exit(1);
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
	m_interval_b_entry_buffer->set_text("10");
	m_builder->get_widget("n_entry", entry);
	if (!entry) exit(1);
	entry->signal_activate().connect(sigc::mem_fun(*this,
                             &DerivedWindow::on_render_button_clicked));
	m_n_entry_buffer = entry->get_buffer();
	m_n_entry_buffer->set_text("5");
	Gtk::Button * btn = nullptr;
	m_builder->get_widget("render_button", btn);
	if (!btn) exit(1);
	btn->signal_clicked().connect(sigc::mem_fun(*this,
                             &DerivedWindow::on_render_button_clicked));
	this->signal_key_press_event().connect(
		sigc::mem_fun(*this, &DerivedWindow::on_key_press));
}

DerivedWindow::~DerivedWindow() {
	delete m_drawingarea1;
	delete m_textview1;
	delete m_data_label;
}

Glib::ustring DerivedWindow::get_function_str() {
	return m_function_entry_buffer->get_text();
}

Glib::ustring DerivedWindow::get_interval_a_str() {
	return m_interval_a_entry_buffer->get_text();
}

Glib::ustring DerivedWindow::get_interval_b_str() {
	return m_interval_b_entry_buffer->get_text();
}

Glib::ustring DerivedWindow::get_n_str() {
	return m_n_entry_buffer->get_text();
}

bool DerivedWindow::on_drawingarea1_draw
(const Cairo::RefPtr<Cairo::Context>& cr) {
	Gtk::Allocation allocation = m_drawingarea1->get_allocation();
	const int& width = allocation.get_width();
	const int& height = allocation.get_height();
	cr->set_line_width(2);
	cr->set_line_cap(Cairo::LINE_CAP_ROUND);
	cr->set_line_join(Cairo::LINE_JOIN_ROUND);

	// draw red lines out from the center of the window
	bool update = m.update(get_function_str(), get_interval_a_str(),
	                       get_interval_b_str(), get_n_str());
	m.resize(height, width);

	const real& a = m.get_a(), b = m.get_b(),
		min = m.get_min(), max = m.get_max();
	const int& ox = m.get_ox(), oy = m.get_oy();
	const std::map<real, real>& fx = m.get_fx(), Lx = m.get_Lx(),
		Px = m.get_Px(), Fx = m.get_Fx();
	std::vector<double> dashes = {2.0, 4.0};
	real tmp;
	long long xai, xbi, xii, xd, yai, ybi, yii, yd;
	calculate_axis(a, b, xai, xbi, xii, xd);
	calculate_axis(min, max, yai, ybi, yii, yd);

	cr->set_source_rgb(1, 1, 1);
	cr->move_to(width * 1.00 - 1, height * 0.00 + 1);
	cr->line_to(width * 1.00 - 1, height * 0.15 + 1);
	cr->line_to(width * 0.85 - 1, height * 0.15 + 1);
	cr->line_to(width * 0.85 - 1, height * 0.00 + 1);
	cr->line_to(width * 1.00 - 1, height * 0.00 + 1);
	cr->fill_preserve();
	cr->set_source_rgb(0, 0, 0);
	cr->stroke();

	cr->set_source_rgb(1, 0, 0);
	cr->arc(width * 0.87, height * 0.025, 8, 0, 2 * M_PI);
	cr->fill();
	cr->set_source_rgb(0, 0, 0);
	draw_text(cr, "M(x,f(x))", width * 0.89, height * 0.025+1, 10, 1, 0);

	cr->set_source_rgb(0.8, 0.8, 0.2);
	cr->move_to(width * 0.87 - 5, height * 0.05);
	cr->line_to(width * 0.87 + 5, height * 0.05);
	cr->stroke();
	cr->set_source_rgb(0, 0, 0);
	draw_text(cr, "f(x)", width * 0.89, height * 0.05+1, 10, 1, 0);

	cr->set_dash(dashes, 0);
	cr->set_source_rgb(0.8, 0.2, 0.2);
	cr->move_to(width * 0.87 - 5, height * 0.075);
	cr->line_to(width * 0.87 + 5, height * 0.075);
	cr->stroke();
	cr->set_source_rgb(0, 0, 0);
	draw_text(cr, "L(x)", width * 0.89, height * 0.075+1, 10, 1, 0);

	cr->set_source_rgb(0.2, 0.8, 0.2);
	cr->move_to(width * 0.87 - 5, height * 0.1);
	cr->line_to(width * 0.87 + 5, height * 0.1);
	cr->stroke();
	cr->set_source_rgb(0, 0, 0);
	draw_text(cr, "P(x)", width * 0.89, height * 0.1+1, 10, 1, 0);

	cr->set_source_rgb(0.2, 0.2, 0.8);
	cr->move_to(width * 0.87 - 5, height * 0.125);
	cr->line_to(width * 0.87 + 5, height * 0.125);
	cr->stroke();
	cr->set_source_rgb(0, 0, 0);
	draw_text(cr, "Ф(x)", width * 0.89, height * 0.125+1, 10, 1, 0);

	cr->unset_dash();
	cr->set_source_rgb(0, 0, 0);
	cr->move_to(width * 0.00, m('Y', oy));
	cr->line_to(width * 1.00 - 1, m('Y', oy));
	cr->move_to(width * 1.00 - 6, m('Y', oy) - 5);
	cr->line_to(width * 1.00 - 1, m('Y', oy));
	cr->line_to(width * 1.00 - 6, m('Y', oy) + 5);
	draw_text(cr, "X", width * 1.00 - 2, m('Y', oy) + 7, 10, -1, 1);
	for (long long i = xai; i <= xbi; i += xii) {
		tmp = i * 1.0 / xd;
		if (i % xd == 0) {
			cr->move_to(m('X', tmp), m('Y', oy) - 6);
			cr->line_to(m('X', tmp), m('Y', oy) + 6);
			draw_text(cr, Glib::Ascii::dtostr(tmp),
			          m('X', tmp), m('Y', oy) + 8, 10, 0, 1);
		} else {
			cr->move_to(m('X', tmp), m('Y', oy) - 2);
			cr->line_to(m('X', tmp), m('Y', oy) + 2);
			draw_text(cr,Glib::Ascii::dtostr(i) + "/" +
			          Glib::Ascii::dtostr(xd),
			          m('X', tmp), m('Y', oy) + 5, 10, 0, 1);
		}
	}


	cr->move_to(m('X', ox), height * 1.0);
	cr->line_to(m('X', ox), height * 0.0 + 1);
	cr->move_to(m('X', ox) + 5, height * 0.0 + 6);
	cr->line_to(m('X', ox), height * 0.0 + 1);
	cr->line_to(m('X', ox) - 5, height * 0.0 + 6);
	draw_text(cr, "Y", m('X', ox) - 7, height * 0.0, 10, -1, 1);
	for (long long i = yai; i <= ybi; i += yii) {
		tmp = i * 1.0 / yd;
		if (i % yd == 0) {
			cr->move_to(m('X', ox) - 6, m('Y', tmp));
			cr->line_to(m('X', ox) + 6, m('Y', tmp));
			draw_text(cr, Glib::Ascii::dtostr(tmp),
			          m('X', ox) - 8, m('Y', tmp), 10, -1, 0);
		} else {
			cr->move_to(m('X', ox) - 2, m('Y', tmp));
			cr->line_to(m('X', ox) + 2, m('Y', tmp));
			draw_text(cr,Glib::Ascii::dtostr(i) + "/"
			          + Glib::Ascii::dtostr(yd),
			          m('X', ox) - 5, m('Y', tmp), 10, -1, 0);
		}
	}
	cr->stroke();


	const std::vector<real>& cp = m.get_cp();
	cr->set_source_rgb(1, 0, 0);
	for (size_t i = 0; i < cp.size(); i++) {
		cr->arc(m('X', cp[i]), m('Y', m('f', cp[i])), 8, 0, 2 * M_PI);
		cr->fill();
	}

	cr->set_source_rgb(0.8, 0.8, 0.2);
	draw_map(cr, fx.begin(), fx.end());

	cr->set_dash(dashes, 0);
	cr->set_source_rgb(0.8, 0.2, 0.2);
	draw_map(cr, Lx.begin(), Lx.end());

	cr->set_source_rgb(0.2, 0.8, 0.2);
	draw_map(cr, Px.begin(), Px.end());

	cr->set_source_rgb(0.2, 0.2, 0.8);
	draw_map(cr, Fx.begin(), Fx.end());

	if (update) {
		m_liststore1->clear();
		Gtk::TreeRow row;
		for (int i = 0; i <= m.get_n() + 1; i++) {
			for (int j = 0; j < 2; j++) {
				row = *(m_liststore1->append());
				if (j == 0) {
					tmp = a + (2 * i - 1) * m.get_h() / 2;
					set(row, 0, tmp);
				} else {
					tmp = a + i * m.get_h();
					row[cols(0,1)] = to_string(tmp) + " (вузол)";
					row[cols(0)] = tmp;
				}
				set(row, 1, m('f', tmp));
				set(row, 2, m('L', tmp));
				set(row, 3, m('P', tmp));
				set(row, 4, m('F', tmp));
				set(row, 5, m('f', tmp) - m('L', tmp));
				set(row, 6, m('f', tmp) - m('P', tmp));
				set(row, 7, m('f', tmp) - m('F', tmp));
			}
		}
		m_data_label->set_text(m.generate_str());
	}
	return true;
}

bool DerivedWindow::on_key_press(GdkEventKey* key_event) {
	return true;
}

void DerivedWindow::on_render_button_clicked() {
	m_drawingarea1->queue_draw();
}

bool DerivedWindow::draw_text(const Cairo::RefPtr<Cairo::Context>& cr,
                              const Glib::ustring& text,
                              double x, double y,
                              int size, char xalign, char yalign) {
	Pango::FontDescription font;
	font.set_size(size * 1000);
	int text_width, text_height;
	Glib::RefPtr<Pango::Layout> layout = create_pango_layout(text);
	layout->set_font_description(font);
	layout->get_pixel_size(text_width, text_height);
	cr->move_to(x + (text_width  / 2) * xalign - text_width  / 2,
	            y + (text_height / 2) * yalign - text_height / 2);
	layout->show_in_cairo_context(cr);
	return true;
}

bool DerivedWindow::draw_map (const Cairo::RefPtr<Cairo::Context>& cr,
		const std::map<real, real>::const_iterator& begin,
		const std::map<real, real>::const_iterator& end) {
	bool t = false;
	const real& a = m.get_a(), b = m.get_b();
	for (std::map<real, real>::const_iterator it = begin; it!=end; it++)
		if (it->first > a && it->first < b) {
			if (!t) {
				cr->move_to(m('X', it->first), m('Y', it->second));
				t = true;
			} else cr->line_to(m('X', it->first), m('Y', it->second));
		}
	cr->stroke();
	return true;
}

void DerivedWindow::set(Gtk::TreeRow& row, const int& i,
                        const real& a) {
	row[cols(i,1)] = to_string(a);
	row[cols(i)] = a;
}

void calculate_axis(const double& a, const double& b,
					long long& new_a, long long& new_b,
					long long& step, long long& enlarger) {
	double double_step = (b - a) / 10;
	new_a = a - 1, new_b = b + 1, step = 1;
	// dsl - double step log10
	long long dsl = floor(log10(double_step));
	enlarger = pow(10, abs(dsl));
	double tmp = double_step * pow(10, -dsl);
	if (dsl < 0) {
		new_a *= enlarger; new_b *= enlarger;
		if (tmp >= 5) {new_a /= 5; new_b /= 5; enlarger /= 5;}
		else if (tmp >= 2) {new_a /= 2; new_b /= 2; enlarger /= 2;}
	} else {
		step = enlarger; enlarger = 1;
		if (tmp >= 5) step *= 5;
		else if (tmp >= 2) step *= 2;
	}
}
