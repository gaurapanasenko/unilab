#include "window.h"
#include "model.h"
#include <ctime>
#include <gtkmm/toolbutton.h>
#include <gtkmm/menuitem.h>
#include <iostream>
#include <fstream>
#include <sstream>

template<class T>
void getObject(const Glib::RefPtr<Gtk::Builder>& builder,
               Glib::RefPtr<T>& pointer, const char* name) {
  pointer = Glib::RefPtr<T>::cast_dynamic(builder->get_object(name));
  if (!pointer) {
    Glib::ustring n = name;
    throw std::domain_error("ERROR: No " + n + " widget");
  }
}

Window::Window(BaseObjectType* cobject,
               Glib::RefPtr<Gtk::Builder> builder)
  : Gtk::ApplicationWindow(cobject), builder_(std::move(builder)) {
  getObject(builder_, statusbar_, "statusbar");
  getObject(builder_, dataTreeView_, "data");
  getObject(builder_, resultTreeView_, "result");

  Glib::RefPtr<Gtk::MenuItem> qmi;
  getObject(builder_, qmi, "quit_menu_item");
  qmi->signal_activate().connect(sigc::mem_fun(*this, &Window::quit));

  Glib::RefPtr<Gtk::ToolButton> ltb;
  getObject(builder_, ltb, "load_tool_button");
  ltb->signal_clicked().connect(sigc::mem_fun(*this, &Window::load));

  Glib::RefPtr<Gtk::ToolButton> stb;
  getObject(builder_, stb, "save_tool_button");
  stb->signal_clicked().connect(sigc::mem_fun(*this, &Window::save));

  Glib::RefPtr<Gtk::ToolButton> rtb;
  getObject(builder_, rtb, "run_tool_button");
  rtb->signal_clicked().connect(sigc::mem_fun(*this, &Window::run));

  getObject(builder_, nAdjustment_, "n_adjustment");
  auto mfrm = sigc::mem_fun(*this, &Window::resizeMatrix);
  nAdjustment_->signal_value_changed().connect(mfrm);

  auto size = static_cast<sizeType>(nAdjustment_->get_value());

  dataStore_ = TwoStore::create(size);
  dataTreeView_->set_model(dataStore_);

  resultStore_ = OneStore::create(size);
  resultTreeView_->set_model(resultStore_);
  auto col0 = resultStore_->get_model_column(0);
  resultTreeView_->append_column_numeric_editable("x1", col0, "%Lg");
  auto col1 = resultStore_->get_model_column(1);
  resultTreeView_->append_column_numeric_editable("δ1", col1, "%Lg");

  load();
}

void Window::resizeMatrix() {
  auto size = static_cast<sizeType>(nAdjustment_->get_value());

  dataStore_->resize(size, size, 1);
  resultStore_->resize(size, 1, 1);

  dataTreeView_->remove_all_columns();

  for (sizeType i = 0; i < size; i++) {
    auto col = dataStore_->get_model_column(i);
    auto name = Glib::ustring::format(i);
    dataTreeView_->append_column_numeric_editable(name, col, "%Lg");
  }
  auto col = dataStore_->get_model_column(size);
  dataTreeView_->append_column_numeric_editable("y", col, "%Lg");
}

void Window::load() {
  std::ifstream in("data.txt");
  if (in) {
    sizeType n;
    in >> n;
    nAdjustment_->set_value(n);
    resizeMatrix();
    in >> dataStore_->getReference() >> resultStore_->getReference();
    statusbar_->push("Loaded data.txt");
  } else {
    resizeMatrix();
  }
}

void Window::save() {
  std::ofstream out("data.txt");
  out << nAdjustment_->get_value() << "\n";
  out << dataStore_->getReference() << resultStore_->getReference();
  statusbar_->push("Saved");
}

void Window::run() {
  auto& ds = dataStore_->getReference();
  auto& rs = resultStore_->getReference();
  sizeType size = ds.getRowsSize();
  std::vector<Matrix::Matrix> y(size + 1, Matrix::Matrix(size, 1));
  auto a = Matrix::Minor(ds, 0, 0, size, size);
  std::cout << "A:\n" << a << "\n";
  y[0] = ds.getColumn(size);
  std::cout << "y0:\n" << y[0] << "\n";
  auto r = rs.getColumn(0);
  for (sizeType i = 0; i < size; i++) {
    y[i + 1] = a * y[i];
    std::cout << "y" << i + 1 << ":\n" << y[i + 1] << "\n";
  }
  y[size] = - y[size];
  std::cout << "y" << size << ":\n" << y[size] << "\n";
  auto m = mergeWrappers(y.begin(), y.end() - 1, Matrix::DIRECTION_ROW);
  Matrix::Matrix ma; ma = m;
  std::cout << "Merged:\n" << ma;
  gauss(ma, Matrix::Matrix(y[size]).getColumn(0), r);
  std::cout << "P:\n" << r;
  rs.getColumn(1) = (m * r) - y[size];

  std::stringstream ss;

  ss << "λ ^ " << size;
  for (sizeType i = 0; i < r.size(); i++) {
    ss << " + " << "λ ^ " << size - i - 1 << " * " << r[size - i - 1];
  }
  ss << " = 0";
  std::cout << ss.str() << "\n";
  statusbar_->remove_all_messages();
  statusbar_->push(ss.str());

}

void Window::quit[[noreturn]]() {
  exit(0);
}
