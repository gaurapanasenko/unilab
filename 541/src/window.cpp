#include "window.h"
#include <iostream>

template<typename T>
Glib::ustring to_string(T t) {
	std::ostringstream oss;
	oss << t;
	return oss.str();
}

void Matrix::resize(size_type count) {
  vector<Vector>::resize(count);
  for (size_type i = 0; i < size(); i++) {
    (*this)[i].resize(count, 1);
  }
}

Columns::Columns() {
  add(y);
}

void Columns::resize(const size_t& sz) {
  if (sz > array_::size()) {
    size_t oldSize = array_::size();
    array_::resize(sz);
    for (size_t i = oldSize; i < array_::size(); i++) {
      add((*this)[i]);
    }
  }
}

ColumnSignals::ColumnSignals(Window& window, const size_t& index)
: window_(window), index_(index) {
}

void ColumnSignals::set_connection(
Gtk::CellRendererText& cell) {
  connection_ = cell.signal_edited().connect(sigc::mem_fun(*this,
    &ColumnSignals::edited));
}

ColumnSignals::~ColumnSignals() {
  connection_.disconnect();
}

void ColumnSignals::edited(const Glib::ustring& path_string,
                           const Glib::ustring& new_text) {
  window_.edit(index_, path_string, new_text);
}

Window::Window(BaseObjectType* cobject,
               const Glib::RefPtr<Gtk::Builder>& builder)
: Gtk::ApplicationWindow(cobject), builder_(builder) {

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

  adjustmentN_ = Glib::RefPtr<Gtk::Adjustment>
    ::cast_dynamic(builder->get_object("n"));
  if (!adjustmentN_) {
    std::cerr << "No n widget" << std::endl;
    exit(1);
  }

  treeViewData_ = Glib::RefPtr<Gtk::TreeView>
    ::cast_dynamic(builder->get_object("data"));
  if (!treeViewData_) {
    std::cerr << "No data widget" << std::endl;
    exit(1);
  }

  resize_matrix();
}

Window::~Window() {
}

void Window::resize_matrix() {
  treeViewData_->remove_all_columns();
  columnSignals_.clear();
  if (listStoreData_)
    listStoreData_->clear();

  size_t size_ = adjustmentN_->get_value();
  a_.resize(size_);
  y_.resize(size_);
  columns_.resize(size_);
  listStoreData_ = Gtk::ListStore::create(columns_);
  treeViewData_->set_model(listStoreData_);

  for (size_t i = 0; i < size_; i++) {
    Gtk::TreeModel::Row row = *(listStoreData_->append());
    row[columns_.y] = y_[i];
    for (size_t j = 0; j < size_; j++) {
      row[columns_[j]] = a_[i][j];
    }
  }

  for (size_t i = 0; i <= size_; i++) {
    Glib::ustring title;
    Gtk::TreeModelColumn<real>* column;
    Gtk::CellRendererText cellRenderer;
    Gtk::TreeView::Column treeViewColumn;
    int columns;

    if (i < size_) {
      title = to_string(i + 1);
      column = &columns_[i];
      //columns = treeViewData_->
        //append_column_numeric_editable(to_string(i + 1),
                                       //columns_[i], "%Lg");
    } else {
      title = "y";
      column = &columns_.y;
      //columns = treeViewData_->
        //append_column_numeric_editable("y",
                                       //columns_.y, "%Lg");
    }

    treeViewColumn.set_title(title);
    treeViewColumn.pack_start(cellRenderer);
    treeViewData_->append_column(treeViewColumn);

    cellRenderer.property_editable() = true;
    /*Gtk::CellRendererText* text =
      dynamic_cast<Gtk::CellRendererText*>(treeViewData_->
      get_column_cell_renderer(columns - 1));*/
    columnSignals_.push_back(ColumnSignals(*this, (int)i));
    ColumnSignals& cs = columnSignals_.back();
    cs.set_connection(cellRenderer);
  }
  show_all_children();
}


void Window::edit(const size_t& i, const Glib::ustring& path_string,
                  const Glib::ustring& new_text) {
  Gtk::TreePath path(path_string);

  char* pchEnd = nullptr;
  size_t j = strtoll(path_string.c_str(), &pchEnd, 10);
  pchEnd = nullptr;
  real new_value = strtold(new_text.c_str(), &pchEnd);
  Gtk::TreeModel::Row row = *(listStoreData_->get_iter(path));
  if (i < a_.size()) {
    a_[j][i] = new_value;
    //row[columns_[i]] = new_value;
  } else {
    y_[i] = new_value;
    //row[columns_.y] = new_value;
  }
}

void Window::quit() {
  exit(0);
}
