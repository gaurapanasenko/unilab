#ifndef MATRIX_H
#define MATRIX_H

#include <climits>
#include <iterator>
#include <vector>
#include <memory>

using sizeType = unsigned long long;
using real = long double;

namespace Matrix {

class Wrapper;

class Cell {
public:
  explicit Cell(Wrapper& wrapper, sizeType row, sizeType column);
  Cell& operator=(real data);
  operator real() const;
  Wrapper& getWrapper() const;
  sizeType getRow() const;
  void setRow(sizeType row);
  sizeType getColumn() const;
  void setColumn(sizeType column);

private:
  Wrapper& wrapper_;
  sizeType row_, column_;
};

bool operator==(const Cell& lhs, const Cell& rhs);

class CellIterator
    : public std::iterator<std::input_iterator_tag, Cell> {
public:
  explicit CellIterator(value_type value, bool direction);
  CellIterator& operator++();
  CellIterator operator++(int);
  bool operator==(const CellIterator& rhs) const;
  bool operator!=(const CellIterator& rhs) const;
  reference operator*();

private:
  value_type value_;
  bool direction_;
};

class Row {
public:
  explicit Row(Wrapper& wrapper, sizeType row);
  Cell operator[](sizeType column);

  CellIterator begin();
  CellIterator end();

  Wrapper& getWrapper() const;
  sizeType getRow() const;

private:
  Wrapper& wrapper_;
  sizeType row_;
};

class Column {
public:
  explicit Column(Wrapper& wrapper, sizeType column);
  Cell operator[](sizeType row);

  CellIterator begin();
  CellIterator end();

  Wrapper& getWrapper() const;
  sizeType getColumn() const;

private:
  Wrapper& wrapper_;
  sizeType column_;
};

class ConstCell {
public:
  explicit ConstCell(const Wrapper& wrapper,
                     sizeType row, sizeType column);
  operator real() const;
  const Wrapper& getWrapper() const;
  sizeType getRow() const;
  void setRow(sizeType row);
  sizeType getColumn() const;
  void setColumn(sizeType column);

private:
  const Wrapper& wrapper_;
  sizeType row_, column_;
};

bool operator==(const ConstCell& lhs, const ConstCell& rhs);

class ConstCellIterator
    : public std::iterator<std::input_iterator_tag, ConstCell> {
public:
  explicit ConstCellIterator(value_type value, bool direction);
  ConstCellIterator& operator++();
  ConstCellIterator operator++(int);
  bool operator==(const ConstCellIterator& rhs) const;
  bool operator!=(const ConstCellIterator& rhs) const;
  reference operator*();

private:
  value_type value_;
  bool direction_;
};

class ConstRow {
public:
  explicit ConstRow(const Wrapper& wrapper, sizeType row);
  ConstCell operator[](sizeType column);

  ConstCellIterator begin();
  ConstCellIterator end();

  const Wrapper& getWrapper() const;
  sizeType getRow() const;

private:
  const Wrapper& wrapper_;
  sizeType row_;
};

class ConstColumn {
public:
  explicit ConstColumn(const Wrapper& wrapper, sizeType column);
  ConstCell operator[](sizeType row);

  ConstCellIterator begin();
  ConstCellIterator end();

  const Wrapper& getWrapper() const;
  sizeType getColumn() const;

private:
  const Wrapper& wrapper_;
  sizeType column_;
};

class Wrapper {
public:
  Wrapper() = default;

  virtual void resize(sizeType rows, sizeType columns);
  virtual sizeType getColumnsSize() const;
  virtual sizeType getRowsSize() const;
  virtual real getData(sizeType row,
                             sizeType column) const;
  virtual void setData(sizeType row, sizeType column, real data);

  Row getRow(sizeType row);
  ConstRow getRow(sizeType row) const;
  Column getColumn(sizeType column);
  ConstColumn getColumn(sizeType column) const;

  Row operator[](sizeType row);
  ConstRow operator[](sizeType row) const;

};

class Minor : public Wrapper {
public:
  Minor(Wrapper& wrapper, sizeType row = 0,
        sizeType column = 0,
        sizeType rows = ULLONG_MAX,
        sizeType columns = ULLONG_MAX);
  Minor(Column& column);
  Minor(Row& row);
  sizeType getColumnsSize() const override;
  sizeType getRowsSize() const override;
  real getData(sizeType row,
                     sizeType column) const override;
  void setData(sizeType row, sizeType column, real data) override;

private:
  Wrapper& wrapper_;
  sizeType column_, row_, rows_, columns_;
};

class ConstMinor : public Wrapper {
public:
  explicit ConstMinor(const Wrapper& wrapper, sizeType row = 0,
        sizeType column = 0,
        sizeType rows = ULLONG_MAX,
        sizeType columns = ULLONG_MAX);
  ConstMinor(ConstColumn& column);
  ConstMinor(ConstRow& row);
  sizeType getColumnsSize() const override;
  sizeType getRowsSize() const override;
  real getData(sizeType row, sizeType column) const override;

private:
  const Wrapper& wrapper_;
  sizeType column_, row_, rows_, columns_;
};

class Number : public Wrapper {
  Number(real number);
  sizeType getColumnsSize() const override;
  sizeType getRowsSize() const override;
  real getData(sizeType row, sizeType column) const override;
  void setData(sizeType row, sizeType column, real data) override;

private:
  real number_;
};

class Matrix : public Wrapper {
public:
  Matrix();
  Matrix(sizeType rows, sizeType columns, real data = 0);
  void resize(sizeType rows, sizeType columns) override;
  sizeType getColumnsSize() const override;
  sizeType getRowsSize() const override;
  real getData(sizeType row, sizeType column) const override;
  void setData(sizeType row, sizeType column, real data) override;

private:
  std::vector< std::vector<real> > matrix_;
  sizeType rows_, columns_;
};

const Matrix operator*(const ConstMinor& lhs, const ConstMinor& rhs);
const Matrix operator+(const ConstMinor& lhs, const ConstMinor& rhs);
const Matrix operator-(const ConstMinor& lhs, const ConstMinor& rhs);
std::ostream& operator<<(std::ostream& output, const Minor& data);

}

#endif // MATRIX_H
