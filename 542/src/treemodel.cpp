#include "treemodel.h"
#include <glibmm/timer.h>
#include <gtkmm/treepath.h>
#include <cmath>

/**************
* SimpleStore *
**************/
SimpleStore::SimpleStore(sizeType size)
  : columnRecord(),
    modelColumns(size), stamp(1), columnsSize_(size) {
  for(unsigned int i = 0; i < modelColumns.size(); ++i) {
    columnRecord.add(modelColumns[i]);
  }
}

SimpleStore::~SimpleStore() {}

void SimpleStore::resize(sizeType rows, sizeType columns, real data) {
  sizeType oldRows = getRowsSizeVirtual();
  resizeVirtual(rows, columns, data);
  stamp++;
  sizeType newRows = getRowsSizeVirtual();
  sizeType newColumns = getColumnsSizeVirtual();

  columnsSize_ = newColumns;
  if (columnsSize_ > modelColumns.size()) {
    sizeType oldSize = modelColumns.size();
    sizeType newSize = columnsSize_;
    modelColumns.resize(newSize);
    for (sizeType i = oldSize; i < newSize; i++) {
      columnRecord.add(modelColumns[i]);
    }
  }

  for (sizeType i = newRows; i < oldRows; i++) {
    auto path = Path(1);
    path[0] = i;
    row_deleted(path);
    Glib::usleep(100);
  }

  for (unsigned int i = oldRows; i < newRows; i++) {
    auto iter = iterator();
    iter.set_stamp(stamp);
    const long row_index = i;
    iter.gobj()->user_data = (void*) row_index;
    row_inserted(get_path(iter), iter);
    Glib::usleep(100);
  }
}

sizeType SimpleStore::getColumnsSize() const {
  return columnsSize_;
}

sizeType SimpleStore::getRowsSize() const {
  return getRowsSizeVirtual();
}

real SimpleStore::getData(sizeType row, sizeType column) const {
  if (row >= getRowsSize() || column >= getColumnsSize()) {
    return NAN;
  }
  return getDataVirtual(row, column);
}

void SimpleStore::setData(sizeType row, sizeType column, real data) {
  if (row >= getRowsSize() || column >= getColumnsSize()) {
    return;
  }
  setDataVirtual(row, column, data);
  auto iter = iterator();
  iter.set_stamp(stamp);
  const long row_index = row;
  iter.gobj()->user_data = (void*) row_index;
  row_changed(get_path(iter), iter);
}

Gtk::TreeModelColumn<real>& SimpleStore::get_model_column(
    sizeType column) {
  return modelColumns[column];
}

SimpleStore& SimpleStore::getReference() {
  return *this;
}

void SimpleStore::resizeVirtual(sizeType rows, sizeType columns,
                                real data) {}

sizeType SimpleStore::getColumnsSizeVirtual() const {
  return 0;
}

sizeType SimpleStore::getRowsSizeVirtual() const {
  return 0;
}

real SimpleStore::getDataVirtual(
  sizeType row, sizeType column
) const {
  return 0;
}

void SimpleStore::setDataVirtual(
  sizeType row, sizeType column, real data
) {}

Gtk::TreeModelFlags SimpleStore::get_flags_vfunc() const {
  return Gtk::TreeModelFlags(0);
}

int SimpleStore::get_n_columns_vfunc() const {
  return columnsSize_;
}

GType SimpleStore::get_column_type_vfunc(int index) const {
  if(index <= (int)modelColumns.size())
    return modelColumns[index].type();
  else return 0;
}

bool SimpleStore::iter_next_vfunc(const iterator& iter,
                                  iterator& iter_next) const {
  iter_next = iterator();
  long index = (long)iter.gobj()->user_data;
  index++;
  if(check_treeiter_validity(iter) && index < (long) getRowsSize()) {
    iter_next.set_stamp(stamp);
    iter_next.gobj()->user_data = (void*)index;
    return true;
  } else {
    return false;
  }
}

bool SimpleStore::get_iter_vfunc(const Path& path,
                                 iterator& iter) const {
  iter = iterator();
  unsigned sz = path.size();
  if(!sz || sz > 1) {
   return false;
  }
  const long row_index = path[0];
  if (row_index < (long) getRowsSize()) {
    iter.set_stamp(stamp);
    iter.gobj()->user_data = (void*) row_index;
  }
  return true;
}

bool SimpleStore::iter_children_vfunc(const iterator& parent,
                                      iterator& iter) const {
  return iter_nth_child_vfunc(parent, 0, iter);
}

bool SimpleStore::iter_parent_vfunc(const iterator& child,
                                    iterator& iter) const {
  iter = iterator();
  return false;
}

bool SimpleStore::iter_nth_child_vfunc(const iterator& parent,
                                       int n, iterator& iter) const {
  iter = iterator();
  return false;
}

bool SimpleStore::iter_nth_root_child_vfunc(int n,
                                            iterator& iter) const {
  iter = iterator();
  if(n < (int) getRowsSize()) {
    iter.set_stamp(stamp);
    const long row_index = n;
    iter.gobj()->user_data = (void*) row_index;
    return true;
  }
  return false;
}

bool SimpleStore::iter_has_child_vfunc(const iterator& iter) const {
  return (iter_n_children_vfunc(iter) > 0);
}

int SimpleStore::iter_n_children_vfunc(const iterator& iter) const {
  return 0;
}

int SimpleStore::iter_n_root_children_vfunc() const {
  return getRowsSize();
}

Gtk::TreeModel::Path SimpleStore::get_path_vfunc(
    const iterator& iter) const {
  const long index = (long) iter.gobj()->user_data;
  if (index < (long) getRowsSize()) {
    auto path = Path(1);
    path[0] = index;
    return path;
  } else {
    return Path();
  }
}

void SimpleStore::get_value_vfunc(const TreeModel::iterator& iter,
                                  int column,
                                  Glib::ValueBase& value) const {
  Gtk::TreeModelColumn<real>::ValueType valueSpecific;
  valueSpecific.init(
    Gtk::TreeModelColumn<real>::ValueType::value_type()
  );
  auto index = (long)iter.gobj()->user_data;
  real result;
  if (
    check_treeiter_validity(iter) &&
    column <= (int) getColumnsSize() &&
    index < (long) getRowsSize()
  ) {
    result = getData(index, column);
  } else result = NAN;
  valueSpecific.set(result);
  value.init(Glib::Value<real>::value_type());
  value = valueSpecific;
}

void SimpleStore::set_value_impl(const iterator& row,
                                 int column,
                                 const Glib::ValueBase& value) {
  auto new_value = reinterpret_cast<Glib::Value<real>&>(
    const_cast<Glib::ValueBase&>(value)
  ).get();
  long index = (long) row.gobj()->user_data;
  (*this)[index][column] = new_value;
  row_changed(get_path(row), row);
}

bool SimpleStore::check_treeiter_validity(
  const const_iterator& iter
) const {
  return stamp == iter.get_stamp();
}

/***********
* TwoStore *
***********/
TwoStore::TwoStore(sizeType size)
  : Glib::ObjectBase(typeid(TwoStore)), SimpleStore(size + 1),
    Glib::Object(), a_(size, size, 1), y_(size, 1, 1) {}

Glib::RefPtr<TwoStore> TwoStore::create(sizeType size) {
  return Glib::RefPtr<TwoStore>(new TwoStore(size));
}

const Matrix::Matrix& TwoStore::getA() {
  return a_;
}

const Matrix::Matrix& TwoStore::getY() {
  return y_;
}

void TwoStore::resizeVirtual(sizeType rows, sizeType, real data) {
  a_.resize(rows, rows, data);
  y_.resize(rows, 1, data);
}

sizeType TwoStore::getColumnsSizeVirtual() const {
  return a_.getRowsSize() + 1;
}

sizeType TwoStore::getRowsSizeVirtual() const {
  return a_.getRowsSize();
}

real TwoStore::getDataVirtual(
  sizeType row, sizeType column
) const {
  if (column == 0) {
    return y_[row][0];
  } else {
    return a_[row][column - 1];
  }
}

void TwoStore::setDataVirtual(
  sizeType row, sizeType column, real data
) {
  if (column == 0) {
    y_[row][0] = data;
  } else {
    a_[row][column - 1] = data;
  }
}


/***********
* OneStore *
***********/
OneStore::OneStore(sizeType size)
  : Glib::ObjectBase(typeid(OneStore)), SimpleStore(1),
    Glib::Object(), matrix_(size, 1, 0) {}

Glib::RefPtr<OneStore> OneStore::create(sizeType size) {
  return Glib::RefPtr<OneStore>(new OneStore(size));
}

void OneStore::resizeVirtual(sizeType rows, sizeType columns,
                             real data) {
  matrix_.resize(rows, 1, data);
}

sizeType OneStore::getColumnsSizeVirtual() const {
  return 1;
}

sizeType OneStore::getRowsSizeVirtual() const {
  return matrix_.getRowsSize();
}

real OneStore::getDataVirtual(sizeType row, sizeType column) const {
  return matrix_[column][row];
}

void OneStore::setDataVirtual(sizeType row, sizeType column,
                              real data) {
  matrix_[column][row] = data;
}
