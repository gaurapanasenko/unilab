#include "matrix.h"
#include <exception>
#include <string>
#include <cmath>

namespace Matrix {

template<class A>
void validate(const A& a, sizeType row, sizeType column,
              sizeType rows, sizeType columns, bool soft) {
  const Wrapper& wrapper = a.getWrapper();
  std::string name = typeid(A).name();
  sizeType rs = wrapper.getRowsSize();
  sizeType cs = wrapper.getColumnsSize();
  if (soft) {
    rs++; cs++;
  }
  if (row >= rs) {
    throw std::domain_error("Wrong " + name + ": no such row");
  }
  if (column >= cs) {
    throw std::domain_error("Wrong " + name + ": no such column");
  }
  if (row + rows > rs) {
    throw std::domain_error("Wrong " + name + ": no such rows");
  }
  if (column + columns > cs) {
    throw std::domain_error("Wrong " + name + ": no such columns");
  }
}

/*******
* Cell *
*******/
Cell::Cell(Wrapper& wrapper, sizeType row, sizeType column)
  : wrapper_(wrapper), row_(row), column_(column) {
  validate(*this, row_, column_, 0, 0, true);
}

Cell& Cell::operator=(real data) {
  wrapper_.setData(row_, column_, data);
  return *this;
}

Cell& Cell::operator=(const Cell& cell) {
  (*this) = real(cell);
  return *this;
}

Cell& Cell::operator=(Cell&& cell) noexcept {
  (*this) = real(cell);
  return *this;
}

Cell::operator real() {
  return wrapper_.getData(row_, column_);
}

Cell::operator real() const {
  return wrapper_.getData(row_, column_);
}

Wrapper& Cell::getWrapper() const {
  return wrapper_;
}

sizeType Cell::getRow() const {
  return row_;
}

void Cell::setRow(sizeType row) {
  row_ = row;
  validate(*this, row_, column_);
}

sizeType Cell::getColumn() const {
  return column_;
}

void Cell::setColumn(sizeType column) {
  column_ = column;
  validate(*this, row_, column_, 0, 0, true);
}

sizeType Cell::getIndex(bool direction) const {
  if (direction == DIRECTION_COLUMN) {
    return row_;
  }
  return column_;
}

void Cell::setIndex(sizeType index, bool direction) {
  if (direction == DIRECTION_COLUMN) {
    row_ = index;
  } else {
    column_ = index;
  }
  validate(*this, row_, column_, 0, 0, true);
}

bool Cell::compare(const Cell& cell) const {
  bool b1 = &getWrapper() == &cell.getWrapper(),
       b2 = getRow() == cell.getRow(),
       b3 = getColumn() == cell.getColumn();
  return b1 && b2 && b3;
}

std::istream& operator>>(std::istream& input, Cell cell) {
  real x;
  input >> x;
  cell = x;
  return input;
}

/************
* ConstCell *
************/
ConstCell::ConstCell(const Wrapper& wrapper, sizeType row,
                     sizeType column)
  : wrapper_(wrapper), row_(row), column_(column) {
  validate(*this, row_, column_, 0, 0, true);
}

ConstCell::ConstCell(const Cell& cell)
  : ConstCell(cell.getWrapper(), cell.getRow(), cell.getColumn()) {}

ConstCell::operator real() {
  return wrapper_.getData(row_, column_);
}

ConstCell::operator real() const {
  return wrapper_.getData(row_, column_);
}

const Wrapper& ConstCell::getWrapper() const {
  return wrapper_;
}

sizeType ConstCell::getRow() const {
  return row_;
}

void ConstCell::setRow(sizeType row) {
  row_ = row;
  validate(*this, row_, column_, 0, 0, true);
}

sizeType ConstCell::getColumn() const {
  return column_;
}

void ConstCell::setColumn(sizeType column) {
  column_ = column;
  validate(*this, row_, column_, 0, 0, true);
}

sizeType ConstCell::getIndex(bool direction) const {
  if (direction == DIRECTION_COLUMN) {
    return row_;
  }
  return column_;
}

void ConstCell::setIndex(sizeType index, bool direction) {
  if (direction == DIRECTION_COLUMN) {
    row_ = index;
  } else {
    column_ = index;
  }
  validate(*this, row_, column_, 0, 0, true);
}

bool ConstCell::compare(const ConstCell& cell) const {

  bool b1 = &getWrapper() == &cell.getWrapper(),
       b2 = getRow() == cell.getRow(),
       b3 = getColumn() == cell.getColumn();
  return b1 && b2 && b3;
}

/*
real operator+(const ConstCell& a) {
  return + real(a);
}

real operator-(const ConstCell& a) {
  return - real(a);
}

real operator+(const ConstCell& a, const ConstCell& b) {
  return real(a) + real(b);
}

real operator-(const ConstCell& a, const ConstCell& b) {
  return real(a) - real(b);
}

real operator*(const ConstCell& a, const ConstCell& b) {
  return real(a) * real(b);
}

real operator/(const ConstCell& a, const ConstCell& b) {
  return real(a) / real(b);
}*/

/*********
* Vector *
*********/
Vector::Vector(Wrapper& wrapper, sizeType index,
               bool direction)
  : wrapper_(wrapper), index_(index), direction_(direction) {
  if (direction_ == DIRECTION_COLUMN) {
    validate(*this, 0, index_, 0, 0, true);
  } else {
    validate(*this, index_, 0, 0, 0, true);
  }
}

Cell Vector::operator[](sizeType index) const {
  if (direction_ == DIRECTION_COLUMN) {
    return Cell(wrapper_, index, index_);
  }
  return Cell(wrapper_, index_, index);

}

Vector& Vector::operator=(const ConstVector& vector) {
  if (size() != vector.size()) {
    throw std::domain_error("Wrong size on copy vectors");
  }
  std::copy(vector.begin(), vector.end(), begin());
  return *this;
}

Vector& Vector::operator=(const ConstMinor& minor) {
  Minor(*this) = minor;
  return *this;
}

Iterator<Cell> Vector::begin() const {
  return Iterator<Cell>((*this)[0], direction_);
}

Iterator<Cell> Vector::end() const {
  return Iterator<Cell>((*this)[size()], direction_);
}

sizeType Vector::size() const {
  if (direction_ == DIRECTION_COLUMN) {
    return wrapper_.getRowsSize();
  }
  return wrapper_.getColumnsSize();
}

Wrapper& Vector::getWrapper() const  {
  return wrapper_;
}

sizeType Vector::getIndex(bool) const  {
  return index_;
}

void Vector::setIndex(sizeType index, bool) {
  index_ = index;
  if (direction_ == DIRECTION_COLUMN) {
    validate(*this, 0, index_, 0, 0, true);
  } else {
    validate(*this, index_, 0, 0, 0, true);
  }
}

bool Vector::getDirection() const {
  return direction_;
}

bool Vector::compare(const Vector& vector) const {
  bool b1 = &getWrapper() == &vector.getWrapper(),
       b2 = getIndex() == vector.getIndex(),
       b3 = getDirection() == vector.getDirection();
  return b1 && b2 && b3;
}

/**************
* ConstVector *
**************/
ConstVector::ConstVector(const Wrapper& wrapper, sizeType index,
                         bool direction)
  : wrapper_(wrapper), index_(index), direction_(direction) {
  if (direction_ == DIRECTION_COLUMN) {
    validate(*this, 0, index_, 0, 0, true);
  } else {
    validate(*this, index_, 0, 0, 0, true);
  }
}

ConstVector::ConstVector(const Vector& vector)
  : ConstVector(vector.getWrapper(), vector.getIndex(),
                vector.getDirection()) {}

ConstCell ConstVector::operator[](sizeType index) const {
  if (direction_ == DIRECTION_COLUMN) {
    return ConstCell(wrapper_, index, index_);
  }
  return ConstCell(wrapper_, index_, index);
}

Iterator<ConstCell> ConstVector::begin() const {
  return Iterator<ConstCell>((*this)[0], direction_);
}

Iterator<ConstCell> ConstVector::end() const {
  return Iterator<ConstCell>((*this)[size()], direction_);
}

sizeType ConstVector::size() const {
  if (direction_ == DIRECTION_COLUMN) {
    return wrapper_.getRowsSize();
  }
  return wrapper_.getColumnsSize();
}

const Wrapper& ConstVector::getWrapper() const {
  return wrapper_;
}

sizeType ConstVector::getIndex(bool) const {
  return index_;
}

void ConstVector::setIndex(sizeType index, bool) {
  index_ = index;
  if (direction_ == DIRECTION_COLUMN) {
    validate(*this, 0, index_, 0, 0, true);
  } else {
    validate(*this, index_, 0, 0, 0, true);
  }
}

bool ConstVector::getDirection() const {
  return direction_;
}

bool ConstVector::compare(const ConstVector& vector) const {
  bool b1 = &getWrapper() == &vector.getWrapper(),
       b2 = getIndex() == vector.getIndex(),
       b3 = getDirection() == vector.getDirection();
  return b1 && b2 && b3;
}

/**********
* Wrapper *
**********/
void Wrapper::resize(sizeType rows, sizeType columns, real data) {}

sizeType Wrapper::getColumnsSize() const {
  return 0;
}

sizeType Wrapper::getRowsSize() const {
  return 0;
}

real Wrapper::getData(sizeType, sizeType) const {
  return 0;
}

void Wrapper::setData(sizeType, sizeType, real) {}

Wrapper& Wrapper::operator=(const Wrapper& wrapper) {
  bool b1 = getRowsSize() != wrapper.getRowsSize();
  bool b2 = getColumnsSize() != wrapper.getColumnsSize();
  if (b1 || b2) {
    throw std::domain_error("Wrong size on copy minors");
  }
  std::copy(wrapper.begin(), wrapper.end(), begin());
  return *this;
}


Vector Wrapper::operator[](sizeType row) {
  return getRow(row);
}

ConstVector Wrapper::operator[](sizeType row) const {
  return getRow(row);
}

Iterator<Vector> Wrapper::begin() {
  auto v = Vector(*this, 0, DIRECTION_ROW);
  return Iterator<Vector>(v, DIRECTION_ROW);
}

Iterator<ConstVector> Wrapper::begin() const {
  auto v = ConstVector(*this, 0, DIRECTION_ROW);
  return Iterator<ConstVector>(v, DIRECTION_ROW);
}

Iterator<Vector> Wrapper::beginColumn() {
  auto v = Vector(*this, 0, DIRECTION_COLUMN);
  return Iterator<Vector>(v, DIRECTION_COLUMN);
}

Iterator<ConstVector> Wrapper::beginColumn() const {
  auto v = ConstVector(*this, 0, DIRECTION_COLUMN);
  return Iterator<ConstVector>(v, DIRECTION_COLUMN);
}

Iterator<Vector> Wrapper::end() {
  sizeType size = getRowsSize();
  auto v = Vector(*this, size, DIRECTION_ROW);
  return Iterator<Vector>(v, DIRECTION_ROW);
}

Iterator<ConstVector> Wrapper::end() const {
  sizeType size = getRowsSize();
  auto v = ConstVector(*this, size, DIRECTION_ROW);
  return Iterator<ConstVector>(v, DIRECTION_ROW);
}

Iterator<Vector> Wrapper::endColumn() {
  sizeType size = getColumnsSize();
  auto v = Vector(*this, size, DIRECTION_COLUMN);
  return Iterator<Vector>(v, DIRECTION_COLUMN);
}

Iterator<ConstVector> Wrapper::endColumn() const {
  sizeType size = getColumnsSize();
  auto v = ConstVector(*this, size, DIRECTION_COLUMN);
  return Iterator<ConstVector>(v, DIRECTION_COLUMN);
}

Vector Wrapper::getRow(sizeType row) {
  return Vector(*this, row, DIRECTION_ROW);
}

ConstVector Wrapper::getRow(sizeType row) const {
  return ConstVector(*this, row, DIRECTION_ROW);
}

Vector Wrapper::getColumn(sizeType column) {
  return Vector(*this, column, DIRECTION_COLUMN);
}

ConstVector Wrapper::getColumn(sizeType column) const {
  return ConstVector(*this, column, DIRECTION_COLUMN);
}

sizeType Wrapper::size() {
  return getRowsSize();
}

/********
* Minor *
********/
Minor::Minor(Wrapper& wrapper, sizeType row,
             sizeType column, sizeType rows, sizeType columns)
  : wrapper_(wrapper), row_(row), column_(column),
    rows_(rows), columns_(columns) {
  validateMinor();
}

Minor::Minor(const Vector& vector)
  : wrapper_(vector.getWrapper()), row_(0), column_(0), rows_(1),
    columns_(1) {
  if (vector.getDirection() == DIRECTION_COLUMN) {
    column_ = vector.getIndex();
    rows_ = wrapper_.getRowsSize();
  } else {
    row_ = vector.getIndex();
    columns_ = wrapper_.getColumnsSize();
  }
  validateMinor();
}

Minor& Minor::operator=(const ConstMinor& minor) {
  *this = minor;
  return *this;
}

/*Minor& Minor::operator=(const ConstMinor& minor) {
  bool b1 = getRowsSize() != minor.getRowsSize();
  bool b2 = getColumnsSize() != minor.getColumnsSize();
  if (b1 || b2) {
    throw std::domain_error("Wrong size on copy minors");
  }
  std::copy(minor.begin(), minor.end(), begin());
  return *this;
}*/

sizeType Minor::getColumnsSize() const {
  return columns_;
}

sizeType Minor::getRowsSize() const {
  return rows_;
}

real Minor::getData(sizeType row, sizeType column) const {
  return wrapper_.getData(row_ + row, column_ + column);
}

void Minor::setData(sizeType row, sizeType column, real data) {
  wrapper_.setData(row_ + row, column_ + column, data);
}

Wrapper& Minor::getWrapper() const {
  return wrapper_;
}

sizeType Minor::getRowMinor() const {
  return row_;
}

sizeType Minor::getColumnMinor() const {
  return column_;
}

void Minor::validateMinor() {
  if (rows_ == AUTO_SIZE) {
    rows_ = wrapper_.getRowsSize();
  }
  if (columns_ == AUTO_SIZE) {
    columns_ = wrapper_.getColumnsSize();
  }
}

/*************
* ConstMinor *
*************/
ConstMinor::ConstMinor(const Wrapper& wrapper, sizeType row,
                       sizeType column, sizeType rows,
                       sizeType columns)
  : wrapper_(wrapper), row_(row), column_(column),
    rows_(rows), columns_(columns) {
  validateMinor();
}

ConstMinor::ConstMinor(const Vector& vector)
  : wrapper_(vector.getWrapper()), row_(0), column_(0), rows_(1),
    columns_(1) {
  if (vector.getDirection() == DIRECTION_COLUMN) {
    column_ = vector.getIndex();
    rows_ = wrapper_.getRowsSize();
  } else {
    row_ = vector.getIndex();
    columns_ = wrapper_.getColumnsSize();
  }
  validateMinor();
}

ConstMinor::ConstMinor(const ConstVector& vector)
  : wrapper_(vector.getWrapper()), row_(0), column_(0), rows_(1),
    columns_(1) {
  if (vector.getDirection() == DIRECTION_COLUMN) {
    column_ = vector.getIndex();
    rows_ = wrapper_.getRowsSize();
  } else {
    row_ = vector.getIndex();
    columns_ = wrapper_.getColumnsSize();
  }
}

ConstMinor::ConstMinor(const Minor& minor)
  : ConstMinor(minor.getWrapper(), minor.getRowMinor(),
               minor.getColumnMinor(), minor.getRowsSize(),
               minor.getColumnsSize()) {}

sizeType ConstMinor::getColumnsSize() const {
  return columns_;
}

sizeType ConstMinor::getRowsSize() const {
  return rows_;
}

real ConstMinor::getData(sizeType row, sizeType column) const {
  return wrapper_.getData(row_ + row, column_ + column);
}

const Wrapper& ConstMinor::getWrapper() const {
  return wrapper_;
}

void ConstMinor::validateMinor() {
  if (rows_ == AUTO_SIZE) {
    rows_ = wrapper_.getRowsSize();
  }
  if (columns_ == AUTO_SIZE) {
    columns_ = wrapper_.getColumnsSize();
  }
  validate(*this, row_, column_, rows_, columns_);
}

/********
* Merge *
********/
Merge::Merge(const std::vector<std::reference_wrapper<Wrapper>>::iterator& first,
             const std::vector<std::reference_wrapper<Wrapper>>::iterator& last,
             bool direction)
  : direction_(direction) {
  if (first != last) {
    sizeType size;
    if (direction_ == DIRECTION_COLUMN) {
      size = static_cast<Wrapper&>(*first).getColumnsSize();
    } else {
      size = static_cast<Wrapper&>(*first).getRowsSize();
    }
    for (auto i = first; i != last; i++) {
      sizeType sz;
      std::string name;
      if (direction_ == DIRECTION_COLUMN) {
        sz = static_cast<Wrapper&>(*i).getColumnsSize();
        name = "columns";
      } else {
        sz = static_cast<Wrapper&>(*i).getRowsSize();
        name = "row";
      }
      if (size != sz) {
        auto message = "Wrong Merge: " + name + " size not equal";
        throw std::domain_error(message);
      }
      wrappers_.emplace_back(*i);
    }
  }
}

sizeType Merge::getColumnsSize() const {
  if (direction_ == DIRECTION_COLUMN) {
    return static_cast<Wrapper&>(wrappers_[0]).getColumnsSize();
  }
  sizeType size = 0;
  for (Wrapper& i : wrappers_) {
    size += i.getColumnsSize();
  }
  return size;
}

sizeType Merge::getRowsSize() const {
  if (direction_ == DIRECTION_ROW) {
    return static_cast<Wrapper&>(wrappers_[0]).getRowsSize();
  }
  sizeType size = 0;
  for (Wrapper& i : wrappers_) {
    size += i.getRowsSize();
  }
  return size;
}

real Merge::getData(sizeType row, sizeType column) const {
  sizeType r = row;
  sizeType c = column;
  for (Wrapper& i : wrappers_) {
    if (direction_ == DIRECTION_COLUMN) {
      if (r < i.getRowsSize()) {
        return i.getData(r, c);
      }
      r -= i.getRowsSize();
    } else {
      if (c < i.getColumnsSize()) {
        return i.getData(r, c);
      }
      c -= i.getColumnsSize();
    }
  }
  return NAN;
}

void Merge::setData(sizeType row, sizeType column, real data) {
  sizeType r = row;
  sizeType c = column;
  for (Wrapper& i : wrappers_) {
    if (direction_ == DIRECTION_COLUMN) {
      if (r < i.getRowsSize()) {
        return i.setData(r, c, data);
      }
      r -= i.getRowsSize();
    } else {
      if (c < i.getColumnsSize()) {
        return i.setData(r, c, data);
      }
      c -= i.getColumnsSize();
    }
  }
}

Wrapper& Merge::getWrapper(sizeType index) const {
  return wrappers_[index];
}

bool Merge::getDirection() const {
  return direction_;
}

/*********
* Number *
*********/
Number::Number(real number) : number_(number) {}

sizeType Number::getColumnsSize() const {
  return 1;
}

sizeType Number::getRowsSize() const {
  return 1;
}

real Number::getData(sizeType, sizeType) const {
  return number_;
}

void Number::setData(sizeType, sizeType, real data) {
  number_ = data;
}

/*********
* Matrix *
*********/
Matrix::Matrix() : rows_(0), columns_(0) {}

Matrix::Matrix(sizeType rows, sizeType columns, real data)
  : rows_(rows), columns_(columns),
    matrix_(rows, std::vector<real>(columns, data)) {}

Matrix::Matrix(const ConstMinor& minor)
  : Matrix(minor.getRowsSize(), minor.getColumnsSize()) {
  (*this) = minor;
}

Matrix& Matrix::operator=(const ConstMinor& minor) {
  static_cast<Wrapper&>(*this) = static_cast<const Wrapper&>(minor);
  return *this;
}

void Matrix::resize(sizeType rows, sizeType columns, real data) {
  rows_ = rows;
  columns_ = columns;
  matrix_.resize(rows_, std::vector<real>(columns_, data));
  for (auto& i : matrix_) {
    i.resize(columns_, data);
  }
}

sizeType Matrix::getColumnsSize() const {
  return columns_;
}

sizeType Matrix::getRowsSize() const {
  return rows_;
}

real Matrix::getData(sizeType row, sizeType column) const {
  return matrix_[row][column];
}

void Matrix::setData(sizeType row, sizeType column, real data) {
  matrix_[row][column] = data;
}


const Matrix operator+(const ConstMinor& rhs) {
  return rhs;
}

const Matrix operator-(const ConstMinor& rhs) {
  Matrix out = rhs;
  for (auto i : out) {
    for (auto j : i) {
      j = -j;
    }
  }
  return out;
}

const Matrix operator*(const ConstMinor& lhs, const ConstMinor& rhs) {
  if (lhs.getColumnsSize() != rhs.getRowsSize()) {
    throw std::domain_error("Can't multiply matrices");
  }
  size_t a = lhs.getRowsSize(), b = lhs.getColumnsSize(),
         c = rhs.getColumnsSize();
  Matrix m(a, c, 0);
  for (size_t i = 0; i < a; i++) {
    for (size_t j = 0; j < c; j++) {
      real x = 0;
      for (size_t k = 0; k < b; k++) {
        x += lhs[i][k] * rhs[k][j];
      }
      m[i][j] = x;
    }
  }
  return m;
}

const Matrix operator+(const ConstMinor& lhs, const ConstMinor& rhs) {
  bool bw = lhs.getColumnsSize() != rhs.getColumnsSize();
  bool bh = lhs.getRowsSize() != rhs.getRowsSize();
  if (bw || bh) {
    throw std::domain_error("Can't add matrices");
  }
  size_t a = lhs.getRowsSize(), b = lhs.getColumnsSize();
  Matrix m(a, b, 0);
  for (size_t i = 0; i < a; i++) {
    for (size_t j = 0; j < b; j++) {
      m[i][j] = lhs[i][j] + rhs[i][j];
    }
  }
  return m;
}

const Matrix operator-(const ConstMinor& lhs, const ConstMinor& rhs) {
  bool bw = lhs.getColumnsSize() != rhs.getColumnsSize();
  bool bh = lhs.getRowsSize() != rhs.getRowsSize();
  if (bw || bh) {
    throw std::domain_error("Can't subtract matrices");
  }
  size_t a = lhs.getRowsSize(), b = lhs.getColumnsSize();
  Matrix m(a, b, 0);
  for (size_t i = 0; i < a; i++) {
    for (size_t j = 0; j < b; j++) {
      m[i][j] = lhs[i][j] - rhs[i][j];
    }
  }
  return m;
}

std::ostream& operator<<(std::ostream& output,
                         const ConstMinor& data) {
  size_t a = data.getRowsSize(), b = data.getColumnsSize();
  for (sizeType i = 0; i < a; i++) {
    for (sizeType j = 0; j < b; j++) {
      output << data[i][j] << " ";
    }
    output << "\n";
  }
  return output;
}

std::istream& operator>>(std::istream& input, Minor data) {
  size_t a = data.getRowsSize(), b = data.getColumnsSize();
  for (sizeType i = 0; i < a; i++) {
    for (sizeType j = 0; j < b; j++) {
      input >> data[i][j];
    }
  }
  return input;
}

}
