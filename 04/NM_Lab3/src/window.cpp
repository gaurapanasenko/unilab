#include "window.h"
#include <iostream>
#include <fstream>

template<typename T>
Glib::ustring to_string(T t) {
	std::ostringstream oss;
	oss << t;
	return oss.str();
}

Window::Window(BaseObjectType* cobject,
               const Glib::RefPtr<Gtk::Builder>& builder) :
 Gtk::ApplicationWindow(cobject), builder_(builder) {

  {
    auto tmp =  Glib::RefPtr<Gtk::MenuItem>
      ::cast_dynamic(builder_->get_object("quit_menu_item"));
    if (tmp) {
      tmp->signal_activate()
        .connect(sigc::mem_fun(*this, &Window::quit));
    }
  }
  {
    auto tmp =  Glib::RefPtr<Gtk::ToolButton>
      ::cast_dynamic(builder_->get_object("load_tool_button"));
    if (tmp) {
      tmp->signal_clicked()
        .connect(sigc::mem_fun(*this, &Window::load));
    }
  }
  {
    auto tmp =  Glib::RefPtr<Gtk::ToolButton>
      ::cast_dynamic(builder_->get_object("save_tool_button"));
    if (tmp) {
      tmp->signal_clicked()
        .connect(sigc::mem_fun(*this, &Window::save));
    }
  }
  {
    auto tmp =  Glib::RefPtr<Gtk::ToolButton>
      ::cast_dynamic(builder_->get_object("run_tool_button"));
    if (tmp) {
      tmp->signal_clicked()
        .connect(sigc::mem_fun(*this, &Window::run));
    }
  }
  {
    auto tmp =  Glib::RefPtr<Gtk::ToolButton>
      ::cast_dynamic(builder_->get_object("quit_tool_button"));
    if (tmp) {
      tmp->signal_clicked()
        .connect(sigc::mem_fun(*this, &Window::quit));
    }
  }
  {
    auto tmp =  Glib::RefPtr<Gtk::SpinButton>
      ::cast_dynamic(builder_->get_object("n_spin_button"));
    if (tmp) {
      tmp->signal_value_changed ()
        .connect(sigc::mem_fun(*this, &Window::resize_matrix));
    }
  }

  statusbar_ = Glib::RefPtr<Gtk::Statusbar>
    ::cast_dynamic(builder->get_object("statusbar"));
  if (!statusbar_) {
    std::cerr << "No statusbar" << std::endl;
    exit(1);
  }

  adjustmentN_ = Glib::RefPtr<Gtk::Adjustment>
    ::cast_dynamic(builder->get_object("n"));
  if (!adjustmentN_) {
    std::cerr << "No n widget" << std::endl;
    exit(1);
  }

  adjustmentAlpha_ = Glib::RefPtr<Gtk::Adjustment>
    ::cast_dynamic(builder->get_object("alpha"));
  if (!adjustmentAlpha_) {
    std::cerr << "No alpha widget" << std::endl;
    exit(1);
  }

  checkButtonUseSecond_ = Glib::RefPtr<Gtk::CheckButton>
    ::cast_dynamic(builder->get_object("use_second_button"));
  if (!checkButtonUseSecond_) {
    std::cerr << "No use_second_button widget" << std::endl;
    exit(1);
  }

  treeViewData_ = Glib::RefPtr<Gtk::TreeView>
    ::cast_dynamic(builder->get_object("data"));
  if (!treeViewData_) {
    std::cerr << "No data widget" << std::endl;
    exit(1);
  }

  treeViewResult_ = Glib::RefPtr<Gtk::TreeView>
    ::cast_dynamic(builder->get_object("result"));
  if (!treeViewResult_) {
    std::cerr << "No result widget" << std::endl;
    exit(1);
  }

  unsigned int size = adjustmentN_->get_value();

  sleStoreData_ = SleStore::create(size);
  treeViewData_->set_model(sleStoreData_);

  columnStoreResult_ = ColumnStore::create(size);
  treeViewResult_->set_model(columnStoreResult_);
  auto col = columnStoreResult_->get_model_column(0);
  treeViewResult_->append_column_numeric_editable(
    "x1", col, "%Lg"
  );
  col = columnStoreResult_->get_model_column(1);
  treeViewResult_->append_column_numeric_editable(
    "δ1", col, "%Lg"
  );
  col = columnStoreResult_->get_model_column(2);
  treeViewResult_->append_column_numeric_editable(
    "x2", col, "%Lg"
  );
  col = columnStoreResult_->get_model_column(3);
  treeViewResult_->append_column_numeric_editable(
    "δ2", col, "%Lg"
  );

  load();
}

Window::~Window() {
}

void Window::resize_matrix() {
  sizeType size = adjustmentN_->get_value();

  sleStoreData_->resize(size);
  treeViewData_->remove_all_columns();

  for (sizeType i = 1; i <= size; i++) {
    auto col = sleStoreData_->get_model_column(i);
    treeViewData_->append_column_numeric_editable(
      to_string(i), col, "%Lg"
    );
  }
  auto col = sleStoreData_->get_model_column(0);
  treeViewData_->append_column_numeric_editable("y", col, "%Lg");

  columnStoreResult_->resize(size);
}


void Window::load() {
  std::ifstream in("data.txt");
  if (in) {
    sizeType n;
    in >> n;
    adjustmentN_->set_value(n);
    resize_matrix();
    in >> sleStoreData_ >> columnStoreResult_;
    statusbar_->push("Loaded data.txt");
  } else {
    resize_matrix();
  }
}

void Window::save() {
  std::ofstream out("data.txt");
  out << adjustmentN_->get_value() << "\n"
      << sleStoreData_ << columnStoreResult_;
  statusbar_->push("Saved");
}

void Window::run() {
  const Matrix& a = sleStoreData_->getA();
  const Vector& f = sleStoreData_->getF();
  SimpleStore& ssd =      sleStoreData_->getReference();
  SimpleStore& ssr = columnStoreResult_->getReference();

  int x = gauss(a, f, columnStoreResult_->getColumn(0));
  if (x) {
    statusbar_->remove_all_messages();
    statusbar_->push("Gauss error");
    return;
  }

  {
    Vector d = calculateDelta(a, f, columnStoreResult_->getCol(0));
    copy(d.begin(), d.end(), ssr.getColumn(1).begin());
  }

  int n = a.size();
  Matrix b(n);
  Vector g(n);
  if (checkButtonUseSecond_->get_active()){
    convertToComfortIterable2(
      a, f, b, g, adjustmentAlpha_->get_value()
    );
  } else {
    convertToComfortIterable(a, f, b, g);
    if (!checkIsConverge(b)) {
      std::cout << calculateMatrixNorm(b) << "\n" << b;
      statusbar_->remove_all_messages();
      statusbar_->push("The norm of the matrix B "
                       "is greater than or equal to 1");
      return;
    }
  }

  auto col = ssr.getColumn(2);
  Vector v1(n, 0), v2;
  Vector *pv1 = &v1, *pv2 = &v2;
  bool y = true;
  size_t k = 0;
  while (y && k < 16384) {
    *pv2 = makeIteration(b, g, *pv1);
    Vector d = calculateDelta(a, f, *pv2);
    y = false;
    for (int i = 0; i < n; i++) {
      if (std::abs(d[i]) > eps) {
        y = true;
        break;
      }
    }
    std::swap(pv1, pv2);
    k++;
  }
  copy(v1.begin(), v1.end(), col.begin());
  if (k >= 16384) {
    statusbar_->remove_all_messages();
    statusbar_->push("Out of maximum iteration limit");
    return;
  }

  {
    Vector d = calculateDelta(a, f, columnStoreResult_->getCol(2));
    copy(d.begin(), d.end(), ssr.getColumn(3).begin());
  }

  statusbar_->remove_all_messages();
  std::stringstream ss;
  ss << k;
  statusbar_->remove_all_messages();
  statusbar_->push(
    "Success, the result was found in k=" + ss.str() +" iterations."
  );
}

void Window::quit() {
  exit(0);
}
