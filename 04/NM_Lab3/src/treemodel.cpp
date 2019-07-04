#include "treemodel.h"

////////////
// Vector //
////////////
Vector::Vector(const size_type& count, const real& default_value) :
vector(count, default_value) {}

Vector& operator+=(Vector& lhs, const Vector& rhs) {
  if (lhs.size() != rhs.size()) return lhs;
  for (size_t i = 0; i < lhs.size(); i++) {
    lhs[i] += rhs[i];
  }
  return lhs;
}

Vector& operator-=(Vector& lhs, const Vector& rhs) {
  if (lhs.size() != rhs.size()) return lhs;
  for (size_t i = 0; i < lhs.size(); i++) {
    lhs[i] -= rhs[i];
  }
  return lhs;
}

Vector& operator*=(Vector& lhs, const Vector& rhs) {
  if (lhs.size() != rhs.size()) return lhs;
  for (size_t i = 0; i < lhs.size(); i++) {
    lhs[i] *= rhs[i];
  }
  return lhs;
}

Vector& operator/=(Vector& lhs, const Vector& rhs) {
  if (lhs.size() != rhs.size()) return lhs;
  for (size_t i = 0; i < lhs.size(); i++) {
    lhs[i] /= rhs[i];
  }
  return lhs;
}

const Vector operator+(Vector lhs, const Vector& rhs) {
  lhs += rhs;
  return lhs;
}

const Vector operator-(Vector lhs, const Vector& rhs) {
  lhs -= rhs;
  return lhs;
}

const Vector operator*(Vector lhs, const Vector& rhs) {
  lhs *= rhs;
  return lhs;
}

const Vector operator/(Vector lhs, const Vector& rhs) {
  lhs /= rhs;
  return lhs;
}

const Vector operator-(Vector lhs) {
  lhs *= -1;
  return lhs;
}

Vector& operator*=(Vector& lhs, const real rhs) {
  for (auto i = lhs.begin(); i != lhs.end(); i++) {
    *i *= rhs;
  }
  return lhs;
}

Vector& operator/=(Vector& lhs, const real rhs) {
  for (auto i = lhs.begin(); i != lhs.end(); i++) {
    *i /= rhs;
  }
  return lhs;
}

const Vector operator*(Vector lhs, const real rhs) {
  lhs *= rhs;
  return lhs;
}

const Vector operator/(Vector lhs, const real rhs) {
  lhs /= rhs;
  return lhs;
}

std::ostream& operator<<(std::ostream& output, const Vector& data) {
  for (auto i = data.begin(); i != data.end(); i++) {
    output << *i << " ";
  }
  return output;
}

const real addElements(const Vector& vector) {
  real x = 0;
  for (auto i = vector.begin(); i != vector.end(); i++) {
    x += *i;
  }
  return x;
}

const real addElementsAbs(const Vector& vector) {
  real x = 0;
  for (auto i = vector.begin(); i != vector.end(); i++) {
    x += std::abs(*i);
  }
  return x;
}


//Row::Row(const size_type& count, const real& default_value) :
//Vector(count, default_value) {}


////////////
// Matrix //
////////////
Matrix::Matrix(const size_type& count, const real& default_value) :
  vector(count, Vector(count, default_value)) {}

void Matrix::resize(const size_type& count, const real& default_value) {
  vector::resize(count);
  for (size_type i = 0; i < size(); i++) {
    (*this)[i].resize(count, default_value);
  }
}

const Vector operator*(const Matrix& lhs, const Vector& rhs) {
  if (lhs.size() != rhs.size()) return Vector();
  Vector out(lhs.size());
  for (size_t i = 0; i < lhs.size(); i++) {
    out[i] = addElements(lhs[i] * rhs);
  }
  return out;
}

const Vector operator*(const Vector& lhs, const Matrix& rhs) {
  if (lhs.size() != rhs.size()) return Vector();
  Vector out(lhs.size(), 0);
  for (size_t i = 0; i < lhs.size(); i++) {
    for (size_t j = 0; j < lhs.size(); j++) {
      out[i] += lhs[j] * rhs[j][i];
    }
  }
  return out;
}

const Matrix operator*(const Matrix& lhs, const real rhs) {
  Matrix out(lhs.size());
  for (size_t i = 0; i < lhs.size(); i++) {
    out[i] = lhs[i] * rhs;
  }
  return out;
}

const Matrix operator-(const Matrix& lhs, const Matrix& rhs) {
  if (lhs.size() != rhs.size()) return Matrix();
  Matrix out(lhs.size());
  for (size_t i = 0; i < lhs.size(); i++) {
    out[i] = lhs[i] - rhs[i];
  }
  return out;
}

const Matrix operator*(const Matrix& lhs, const Matrix& rhs) {
  if (lhs.size() != rhs.size()) return Matrix();
  Matrix out(lhs.size());
  for (size_t i = 0; i < lhs.size(); i++) {
    out[i] = lhs[i] * rhs;
  }
  return out;
}

std::ostream& operator<<(std::ostream& output, const Matrix& data) {
  for (auto i = data.begin(); i != data.end(); i++) {
    output << *i << "\n";
  }
  return output;
}


namespace SimpleStoreStuff {

///////////////////////
// SimpleStore::Cell //
///////////////////////
Cell::Cell(
  SimpleStore& simpleStore,
  sizeType row, sizeType column
) : simpleStore_(&simpleStore), row_(row), column_(column) {}

Cell& Cell::operator=(real data) {
  simpleStore_->setData(row_, column_, data);
  return *this;
}

Cell& Cell::operator=(
  const Cell& data
) {
  return (*this) = (real) data;
}

Cell::operator real() {
  return simpleStore_->getData(row_, column_);
}

Cell::operator real() const {
  return simpleStore_->getData(row_, column_);
}

SimpleStore& Cell::getSimpleStore() {
  return *simpleStore_;
}

const SimpleStore& Cell::getSimpleStore() const {
  return *simpleStore_;
}

sizeType& Cell::getRow() {
  return row_;
}

const sizeType& Cell::getRow() const {
  return row_;
}

sizeType& Cell::getColumn() {
  return column_;
}

const sizeType& Cell::getColumn() const {
  return column_;
}

bool operator==(const Cell& lhs, const Cell& rhs) {
  return &(lhs.getSimpleStore()) == &(rhs.getSimpleStore()) &&
    lhs.getRow() == rhs.getRow() && lhs.getColumn() == rhs.getColumn();
}

/////////////////////////
// SimpleStore::Column //
/////////////////////////
Column::Column(
  SimpleStore& simpleStore, sizeType column
) : simpleStore_(simpleStore), column_(column) {}

Cell Column::operator[](sizeType row) {
  return Cell(simpleStore_, row, column_);
}

SimpleStore& Column::getSimpleStore() {
  return simpleStore_;
}


Column::iterator::iterator(
  const Cell& cell
) : cell_(cell) {}

Column::iterator&
Column::iterator::operator++() {
  cell_.getRow()++;
  return *this;
}

Column::iterator
Column::iterator::operator++(int) {
  iterator retval = *this;
  ++(*this);
  return retval;
}

bool Column::iterator::operator==(iterator other) const {
  return cell_ == other.cell_;
}

bool Column::iterator::operator!=(iterator other) const {
  return !(*this == other);
}

Column::iterator::reference
Column::iterator::operator*() const {
  return cell_;
}

Column::iterator Column::begin() {
  return iterator((*this)[0]);
}

Column::iterator Column::end() {
  return iterator((*this)[simpleStore_.rowsSize()]);
}

//////////////////////
// SimpleStore::Row //
//////////////////////
Row::Row(SimpleStore& simpleStore, sizeType row) :
  simpleStore_(simpleStore), row_(row)
{}

Cell Row::operator[](sizeType column) {
  return Cell(simpleStore_, row_, column);
}

}

/////////////////
// SimpleStore //
/////////////////
SimpleStore::SimpleStore(sizeType size) :
  TreeModel(), columnRecord(),
  modelColumns(size), stamp(1), columnsSize_(size)
{
  for(unsigned int i = 0; i < modelColumns.size(); ++i) {
    columnRecord.add(modelColumns[i]);
  }
}

SimpleStore::~SimpleStore() {}

Gtk::TreeModelColumn<real>& SimpleStore::get_model_column(
  sizeType column
) {
  return modelColumns[column];
}


void SimpleStore::resize(sizeType n) {
  columnsSize_ = columnsSizeVirtual(n);
  if (columnsSize_ > modelColumns.size()) {
    sizeType oldSize = modelColumns.size();
    sizeType newSize = columnsSize_;
    modelColumns.resize(newSize);
    for (sizeType i = oldSize; i < newSize; i++) {
      columnRecord.add(modelColumns[i]);
    }
  }
  sizeType oldSize = rowsSize();
  resizeVirtual(n);
  sizeType newSize = rowsSize();
  stamp++;
  for (sizeType i = newSize; i < oldSize; i++) {
    auto path = Path(1);
    path[0] = i;
    row_deleted(path);
    Glib::usleep(100);
  }
  for (unsigned int i = oldSize; i < newSize; i++) {
    auto iter = iterator();
    iter.set_stamp(stamp);
    const long row_index = i;
    iter.gobj()->user_data = (void*) row_index;
    row_inserted(get_path(iter), iter);
    Glib::usleep(100);
  }
}

const sizeType SimpleStore::columnsSize() {
  return columnsSize_;
}

const sizeType SimpleStore::columnsSize() const {
  return columnsSize_;
}

const sizeType SimpleStore::rowsSize() {
  return static_cast<const SimpleStore&>(*this).rowsSize();
}

const sizeType SimpleStore::rowsSize() const {
  return rowsSizeVirtual();
}

const real SimpleStore::getData(sizeType row, sizeType column) {
  return static_cast<const SimpleStore&>(*this).getData(row, column);
}

const real SimpleStore::getData(sizeType row, sizeType column) const {
  if (row >= rowsSize() || column >= columnsSize()) {
    return NAN;
  }
  return getDataVirtual(row, column);
}

void SimpleStore::setData(sizeType row, sizeType column, real data) {
  if (row >= rowsSize() || column >= columnsSize()) {
    return;
  }
  setDataVirtual(row, column, data);
  auto iter = iterator();
  iter.set_stamp(stamp);
  const long row_index = row;
  iter.gobj()->user_data = (void*) row_index;
  row_changed(get_path(iter), iter);
}

SimpleStoreStuff::Row SimpleStore::operator[](sizeType row) {
  return SimpleStoreStuff::Row(*this, row);
}

SimpleStoreStuff::Column SimpleStore::getColumn(sizeType column) {
  return SimpleStoreStuff::Column(*this, column);
}

SimpleStore& SimpleStore::getReference() {
  return *this;
}

void SimpleStore::resizeVirtual(sizeType n) {}

const sizeType SimpleStore::columnsSizeVirtual(sizeType n) const {
  return n;
}

const sizeType SimpleStore::rowsSizeVirtual() const {
  return 0;
}

const real SimpleStore::getDataVirtual(
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

bool SimpleStore::iter_next_vfunc(
  const iterator& iter,
  iterator& iter_next
) const {
  iter_next = iterator();
  long index = (long)iter.gobj()->user_data;
  index++;
  if(check_treeiter_validity(iter) && index < (long) rowsSize()) {
    iter_next.set_stamp(stamp);
    iter_next.gobj()->user_data = (void*)index;
    return true;
  } else {
    return false;
  }
}

bool SimpleStore::get_iter_vfunc(
  const Path& path,
  iterator& iter
) const {
  iter = iterator();
  unsigned sz = path.size();
  if(!sz || sz > 1) {
   return false;
  }
  const long row_index = path[0];
  if (row_index < (long) rowsSize()) {
    iter.set_stamp(stamp);
    iter.gobj()->user_data = (void*) row_index;
  }
  return true;
}

bool SimpleStore::iter_children_vfunc(
  const iterator& parent,
  iterator& iter
) const {
  return iter_nth_child_vfunc(parent, 0, iter);
}

bool SimpleStore::iter_parent_vfunc(
  const iterator& child,
  iterator& iter
) const {
  iter = iterator();
  return false;
}

bool SimpleStore::iter_nth_child_vfunc(
  const iterator& parent,
  int n, iterator& iter
) const {
  iter = iterator();
  return false;
}

bool SimpleStore::iter_nth_root_child_vfunc(
  int n,
  iterator& iter
) const {
  iter = iterator();
  if(n < (int) rowsSize()) {
    iter.set_stamp(stamp);
    const long row_index = n;
    iter.gobj()->user_data = (void*) row_index;
    return true;
  }
  return false;
}

bool SimpleStore::iter_has_child_vfunc(
  const iterator& iter
) const {
  return (iter_n_children_vfunc(iter) > 0);
}

int SimpleStore::iter_n_children_vfunc(
  const iterator& iter
) const {
  return 0;
}

int SimpleStore::iter_n_root_children_vfunc() const {
  return rowsSize();
}

Gtk::TreeModel::Path SimpleStore::get_path_vfunc(
  const iterator& iter
) const {
  const long index = (long) iter.gobj()->user_data;
  if (index < (long) rowsSize()) {
    auto path = Path(1);
    path[0] = index;
    return path;
  } else {
    return Path();
  }
}

void SimpleStore::get_value_vfunc(
  const TreeModel::iterator& iter,
  int column, Glib::ValueBase& value
) const {
  Gtk::TreeModelColumn<real>::ValueType valueSpecific;
  valueSpecific.init(
    Gtk::TreeModelColumn<real>::ValueType::value_type()
  );
  auto index = (long)iter.gobj()->user_data;
  real result;
  if (
    check_treeiter_validity(iter) &&
    column <= (int) columnsSize() &&
    index < (long) rowsSize()
  ) {
    result = getData(index, column);
  } else result = NAN;
  valueSpecific.set(result);
  value.init(Glib::Value<real>::value_type());
  value = valueSpecific;
}

void SimpleStore::set_value_impl(
  const iterator& row,
  int column, const Glib::ValueBase& value
) {
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

std::ostream& operator<<(
  std::ostream& output, const Glib::RefPtr<SimpleStore>& store
) {
  for (sizeType i = 0; i < store->rowsSize(); i++) {
    for (sizeType j = 0; j < store->columnsSize(); j++) {
      output << store->operator[](i)[j] << " ";
    }
    output << "\n";
  }
  return output;
}

std::istream& operator>>(
  std::istream& input, const Glib::RefPtr<SimpleStore>& store
) {
  for (sizeType i = 0; i < store->rowsSize(); i++) {
    for (sizeType j = 0; j < store->columnsSize(); j++) {
      real x;
      input >> x;
      store->operator[](i)[j] = x;
    }
  }
  return input;
}

//////////////
// SleStore //
//////////////
SleStore::SleStore(sizeType size) :
  Glib::ObjectBase(typeid(SleStore)),
  SimpleStore(size + 1),
  Glib::Object(),
  a_(size, 1), f_(size, 1) {}

Glib::RefPtr<SleStore> SleStore::create(sizeType size) {
  return Glib::RefPtr<SleStore>(new SleStore(size));
}

const Matrix& SleStore::getA() {
  return a_;
}

const Vector& SleStore::getF() {
  return f_;
}

void SleStore::resizeVirtual(sizeType n) {
  a_.resize(n, 1);
  f_.resize(n, 1);
}

const sizeType SleStore::columnsSizeVirtual(sizeType n) const {
  return n + 1;
}

const sizeType SleStore::rowsSizeVirtual() const {
  return a_.size();
}

const real SleStore::getDataVirtual(
  sizeType row, sizeType column
) const {
  if (column == 0) {
    return f_[row];
  } else {
    return a_[row][column - 1];
  }
}

void SleStore::setDataVirtual(
  sizeType row, sizeType column, real data
) {
  if (column == 0) {
    f_[row] = data;
  } else {
    a_[row][column - 1] = data;
  }
}


/////////////////
// ColumnStore //
/////////////////
ColumnStore::ColumnStore(sizeType size) :
  Glib::ObjectBase(typeid(ColumnStore)),
  SimpleStore(4),
  Glib::Object(),
  vector_(4, Vector(size, 0)) {}

Glib::RefPtr<ColumnStore> ColumnStore::create(sizeType size) {
  return Glib::RefPtr<ColumnStore>(new ColumnStore(size));
}

const Vector& ColumnStore::getCol(sizeType index) {
  return vector_[index];
}

void ColumnStore::resizeVirtual(sizeType n) {
  for (int i = 0; i < 4; i++) {
    vector_[i].resize(n, 0);
  }
}

const sizeType ColumnStore::columnsSizeVirtual(sizeType n) const {
  return 4;
}

const sizeType ColumnStore::rowsSizeVirtual() const {
  return vector_[0].size();
}

const real ColumnStore::getDataVirtual(
  sizeType row, sizeType column
) const {
  return vector_[column][row];
}

void ColumnStore::setDataVirtual(
  sizeType row, sizeType column, real data
) {
  vector_[column][row] = data;
}
