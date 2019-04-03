#include "matrix.h"
#include <exception>

namespace Matrix {

/*******
* Cell *
*******/
Cell::Cell(Wrapper& wrapper, sizeType row,
                   sizeType column)
  : wrapper_(wrapper), row_(row), column_(column) {
  if (row_ >= wrapper_.getRowsSize()) {
    throw std::domain_error("Wrong Cell: no such row");
  }
  if (column_ >= wrapper_.getColumnsSize()) {
    throw std::domain_error("Wrong Cell: no such column");
  }
}

Cell& Cell::operator=(real data) {
  wrapper_.setData(row_, column_, data);
  return *this;
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
}

sizeType Cell::getColumn() const {
  return column_;
}

void Cell::setColumn(sizeType column) {
  column_ = column;
}

bool operator==(const Cell& lhs, const Cell& rhs) {
  bool b1 = &lhs.getWrapper() == &rhs.getWrapper(),
       b2 = lhs.getRow() == rhs.getRow(),
       b3 = lhs.getColumn() == rhs.getColumn();
  return b1 && b2 && b3;
}

/***************
* CellIterator *
***************/
CellIterator::CellIterator(
    std::iterator<std::input_iterator_tag, Cell>::value_type value,
    bool direction) : value_(value), direction_(direction) {}

CellIterator& CellIterator::operator++() {
  if (direction_) {
    value_.setColumn(value_.getColumn() + 1);
  } else {
    value_.setRow(value_.getRow() + 1);
  }
  return *this;
}

CellIterator CellIterator::operator++(int) {
  CellIterator out = *this;
  ++(*this);
  return out;
}

bool CellIterator::operator==(const CellIterator& rhs) const {
  return value_ == rhs.value_;
}

bool CellIterator::operator!=(const CellIterator& rhs) const {
  return !(*this == rhs);
}

std::iterator<std::input_iterator_tag, Cell>::reference
CellIterator::operator*() {
  return value_;
}

/******
* Row *
******/
Row::Row(Wrapper& wrapper, sizeType row)
  : wrapper_(wrapper), row_(row) {
  if (row_ >= wrapper_.getRowsSize()) {
    throw std::domain_error("Wrong Row: no such row");
  }
}

Cell Row::operator[](sizeType column) {
  return Cell(wrapper_, row_, column);
}

CellIterator Row::begin() {
  return CellIterator((*this)[0], false);
}

CellIterator Row::end() {
  return CellIterator((*this)[wrapper_.getRowsSize()], false);
}

Wrapper& Row::getWrapper() const {
  return wrapper_;
}

sizeType Row::getRow() const {
  return row_;
}

/*********
* Column *
*********/
Column::Column(Wrapper& wrapper, sizeType column)
  : wrapper_(wrapper), column_(column) {
  if (column_ >= wrapper_.getColumnsSize()) {
    throw std::domain_error("Wrong Column: no such row");
  }
}

Cell Column::operator[](sizeType row) {
  return Cell(wrapper_, row, column_);
}

CellIterator Column::begin() {
  return CellIterator((*this)[0], false);
}

CellIterator Column::end() {
  return CellIterator((*this)[wrapper_.getColumnsSize()], false);
}

Wrapper& Column::getWrapper() const {
  return wrapper_;
}

sizeType Column::getColumn() const {
  return column_;
}

/************
* ConstCell *
************/
ConstCell::ConstCell(const Wrapper& wrapper, sizeType row,
                     sizeType column)
  : wrapper_(wrapper), row_(row), column_(column) {
  if (row_ >= wrapper_.getRowsSize()) {
    throw std::domain_error("Wrong Cell: no such row");
  }
  if (column_ >= wrapper_.getColumnsSize()) {
    throw std::domain_error("Wrong Cell: no such column");
  }
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
}

sizeType ConstCell::getColumn() const {
  return column_;
}

void ConstCell::setColumn(sizeType column) {
  column_ = column;
}

bool operator==(const ConstCell& lhs, const ConstCell& rhs) {
  bool b1 = &lhs.getWrapper() == &rhs.getWrapper(),
       b2 = lhs.getRow() == rhs.getRow(),
       b3 = lhs.getColumn() == rhs.getColumn();
  return b1 && b2 && b3;
}

/********************
* ConstCellIterator *
********************/
ConstCellIterator::ConstCellIterator(
    std::iterator<std::input_iterator_tag, ConstCell>::value_type value,
    bool direction) : value_(value), direction_(direction) {}

ConstCellIterator& ConstCellIterator::operator++() {
  if (direction_) {
    value_.setColumn(value_.getColumn() + 1);
  } else {
    value_.setRow(value_.getRow() + 1);
  }
  return *this;
}

ConstCellIterator ConstCellIterator::operator++(int) {
  ConstCellIterator out = *this;
  ++(*this);
  return out;
}

bool ConstCellIterator::operator==(const ConstCellIterator& rhs) const {
  return value_ == rhs.value_;
}

bool ConstCellIterator::operator!=(const ConstCellIterator& rhs) const {
  return !(*this == rhs);
}

std::iterator<std::input_iterator_tag, ConstCell>::reference
ConstCellIterator::operator*() {
  return value_;
}

/***********
* ConstRow *
***********/
ConstRow::ConstRow(const Wrapper& wrapper, sizeType row)
  : wrapper_(wrapper), row_(row) {
  if (row_ >= wrapper_.getRowsSize()) {
    throw std::domain_error("Wrong Row: no such row");
  }
}

ConstCell ConstRow::operator[](sizeType column) {
  return ConstCell(wrapper_, row_, column);
}

ConstCellIterator ConstRow::begin() {
  return ConstCellIterator((*this)[0], false);
}

ConstCellIterator ConstRow::end() {
  return ConstCellIterator((*this)[wrapper_.getRowsSize()], false);
}

const Wrapper& ConstRow::getWrapper() const {
  return wrapper_;
}

sizeType ConstRow::getRow() const {
  return row_;
}

/**************
* ConstColumn *
**************/
ConstColumn::ConstColumn(const Wrapper& wrapper, sizeType column)
  : wrapper_(wrapper), column_(column) {
  if (column_ >= wrapper_.getColumnsSize()) {
    throw std::domain_error("Wrong Column: no such row");
  }
}

ConstCell ConstColumn::operator[](sizeType row) {
  return ConstCell(wrapper_, row, column_);
}

ConstCellIterator ConstColumn::begin() {
  return ConstCellIterator((*this)[0], false);
}

ConstCellIterator ConstColumn::end() {
  return ConstCellIterator((*this)[wrapper_.getColumnsSize()], false);
}

const Wrapper& ConstColumn::getWrapper() const {
  return wrapper_;
}

sizeType ConstColumn::getColumn() const {
  return column_;
}

/**********
* Wrapper *
**********/
void Wrapper::resize(sizeType rows, sizeType columns) {}

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

Row Wrapper::getRow(sizeType row) {
  return Row(*this, row);
}

ConstRow Wrapper::getRow(sizeType row) const {
  return ConstRow(*this, row);
}

Column Wrapper::getColumn(sizeType column) {
  return Column(*this, column);
}

ConstColumn Wrapper::getColumn(sizeType column) const {
  return ConstColumn(*this, column);
}

Row Wrapper::operator[](sizeType row) {
  return getRow(row);
}

ConstRow Wrapper::operator[](sizeType row) const {
  return getRow(row);
}

/********
* Minor *
********/
Minor::Minor(Wrapper& wrapper, sizeType row,
             sizeType column, sizeType rows, sizeType columns)
  : wrapper_(wrapper), row_(row), column_(column),
    rows_(rows), columns_(columns) {
  if (row_ >= wrapper_.getRowsSize()) {
    throw std::domain_error("Wrong Minor: no such row");
  }
  if (column_ >= wrapper_.getColumnsSize()) {
    throw std::domain_error("Wrong Minor: no such column");
  }
  if (row_ + rows_ >= wrapper_.getRowsSize()) {
    throw std::domain_error("Wrong Minor: no such rows");
  }
  if (column_ + columns_ >= wrapper_.getColumnsSize()) {
    throw std::domain_error("Wrong Minor: no such columns");
  }
}

Minor::Minor(Column& column)
  : wrapper_(column.getWrapper()), row_(0),
    column_(column.getColumn()),
    rows_(wrapper_.getRowsSize() - 1), columns_(1) {
  if (row_ >= wrapper_.getRowsSize()) {
    throw std::domain_error("Wrong Minor: no such row");
  }
  if (column_ >= wrapper_.getColumnsSize()) {
    throw std::domain_error("Wrong Minor: no such column");
  }
  if (row_ + rows_ >= wrapper_.getRowsSize()) {
    throw std::domain_error("Wrong Minor: no such rows");
  }
  if (column_ + columns_ >= wrapper_.getColumnsSize()) {
    throw std::domain_error("Wrong Minor: no such columns");
  }
}

Minor::Minor(Row& row)
  : wrapper_(row.getWrapper()), row_(row.getRow()),
    column_(0),
    rows_(1), columns_(wrapper_.getColumnsSize() - 1) {
  if (row_ >= wrapper_.getRowsSize()) {
    throw std::domain_error("Wrong Minor: no such row");
  }
  if (column_ >= wrapper_.getColumnsSize()) {
    throw std::domain_error("Wrong Minor: no such column");
  }
  if (row_ + rows_ >= wrapper_.getRowsSize()) {
    throw std::domain_error("Wrong Minor: no such rows");
  }
  if (column_ + columns_ >= wrapper_.getColumnsSize()) {
    throw std::domain_error("Wrong Minor: no such columns");
  }

}

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

/*************
* ConstMinor *
*************/
ConstMinor::ConstMinor(const Wrapper& wrapper, sizeType row,
                       sizeType column, sizeType rows, sizeType columns)
  : wrapper_(wrapper), row_(row), column_(column),
    rows_(rows), columns_(columns) {
  if (row_ >= wrapper_.getRowsSize()) {
    throw std::domain_error("Wrong Minor: no such row");
  }
  if (column_ >= wrapper_.getColumnsSize()) {
    throw std::domain_error("Wrong Minor: no such column");
  }
  if (row_ + rows_ >= wrapper_.getRowsSize()) {
    throw std::domain_error("Wrong Minor: no such rows");
  }
  if (column_ + columns_ >= wrapper_.getColumnsSize()) {
    throw std::domain_error("Wrong Minor: no such columns");
  }
}

ConstMinor::ConstMinor(ConstColumn& column)
  : wrapper_(column.getWrapper()), row_(0),
    column_(column.getColumn()),
    rows_(wrapper_.getRowsSize() - 1), columns_(1) {
  if (row_ >= wrapper_.getRowsSize()) {
    throw std::domain_error("Wrong Minor: no such row");
  }
  if (column_ >= wrapper_.getColumnsSize()) {
    throw std::domain_error("Wrong Minor: no such column");
  }
  if (row_ + rows_ >= wrapper_.getRowsSize()) {
    throw std::domain_error("Wrong Minor: no such rows");
  }
  if (column_ + columns_ >= wrapper_.getColumnsSize()) {
    throw std::domain_error("Wrong Minor: no such columns");
  }
}

ConstMinor::ConstMinor(ConstRow& row)
  : wrapper_(row.getWrapper()), row_(row.getRow()),
    column_(0),
    rows_(1), columns_(wrapper_.getColumnsSize() - 1) {
  if (row_ >= wrapper_.getRowsSize()) {
    throw std::domain_error("Wrong Minor: no such row");
  }
  if (column_ >= wrapper_.getColumnsSize()) {
    throw std::domain_error("Wrong Minor: no such column");
  }
  if (row_ + rows_ >= wrapper_.getRowsSize()) {
    throw std::domain_error("Wrong Minor: no such rows");
  }
  if (column_ + columns_ >= wrapper_.getColumnsSize()) {
    throw std::domain_error("Wrong Minor: no such columns");
  }

}

sizeType ConstMinor::getColumnsSize() const {
  return columns_;
}

sizeType ConstMinor::getRowsSize() const {
  return rows_;
}

real ConstMinor::getData(sizeType row, sizeType column) const {
  return wrapper_.getData(row_ + row, column_ + column);
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

void Matrix::resize(sizeType rows, sizeType columns) {
  rows_ = rows;
  columns_ = columns;
  matrix_.resize(rows_);
  for (auto& i : matrix_) {
    i.resize(columns_);
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
}

}
