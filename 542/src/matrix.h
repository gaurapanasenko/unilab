#ifndef MATRIX_H
#define MATRIX_H

#include <climits>
#include <iterator>
#include <vector>
#include <memory>

using sizeType = unsigned long long;
using real = long double;

namespace Matrix {

const sizeType autoSizeMask = ULLONG_MAX;

class Wrapper;
class Minor;

template<class A>
class Iterator
    : public std::iterator<std::input_iterator_tag, A> {
public:
  explicit Iterator(A value, bool direction);
  Iterator& operator++();
  Iterator operator++(int);
  bool operator==(const Iterator& rhs) const;
  bool operator!=(const Iterator& rhs) const;
  A operator*();

private:
  A value_;
  bool direction_;
};

class ConstCell {
public:
  explicit ConstCell(const Wrapper& wrapper,
                     sizeType row, sizeType column);
  operator real();
  operator real() const;
  const Wrapper& getWrapper() const;
  sizeType getRow() const;
  void setRow(sizeType row);
  sizeType getColumn() const;
  void setColumn(sizeType column);
  sizeType getIndex(bool direction) const;
  void setIndex(sizeType index, bool direction);

private:
  const Wrapper& wrapper_;
  sizeType row_, column_;
};

bool operator==(const ConstCell& lhs, const ConstCell& rhs);

class Cell {
public:
  explicit Cell(Wrapper& wrapper, sizeType row, sizeType column);
  Cell& operator=(const Cell& cell);
  Cell& operator=(real data);
  operator real();
  operator real() const;
  Wrapper& getWrapper() const;
  sizeType getRow() const;
  void setRow(sizeType row);
  sizeType getColumn() const;
  void setColumn(sizeType column);
  operator ConstCell();

private:
  Wrapper& wrapper_;
  sizeType row_, column_;
};

class ConstVector {
public:
  explicit ConstVector(const Wrapper& wrapper, sizeType index,
                       bool direction);
  ConstCell operator[](sizeType index);

  Iterator<ConstCell> begin() const;
  Iterator<ConstCell> end() const;

  const Wrapper& getWrapper() const;
  sizeType getIndex(bool) const;
  void setIndex(sizeType index, bool);

  operator Minor() const;

private:
  const Wrapper& wrapper_;
  sizeType index_;
  bool direction;
};

class Vector {
public:
  explicit Vector(Wrapper& wrapper, sizeType index, bool direction);
  Cell operator[](sizeType index);

  Iterator<Cell> begin() const;
  Iterator<Cell> end() const;

  const Wrapper& getWrapper() const;
  sizeType getIndex(bool) const;
  void setIndex(sizeType index, bool);

  operator Minor() const;

private:
  const Wrapper& wrapper_;
  sizeType index_;
  bool direction;
};

class Wrapper {
public:
  Wrapper() = default;

  virtual void resize(sizeType rows, sizeType columns, real data);
  virtual sizeType getColumnsSize() const;
  virtual sizeType getRowsSize() const;
  virtual real getData(sizeType row, sizeType column) const;
  virtual void setData(sizeType row, sizeType column, real data);

  Vector getRow(sizeType row);
  ConstVector getRow(sizeType row) const;
  Vector getColumn(sizeType column);
  ConstVector getColumn(sizeType column) const;
  sizeType size();

  Vector operator[](sizeType row);
  ConstVector operator[](sizeType row) const;

};

class ConstMinor : public Wrapper {
public:
  ConstMinor(const Wrapper& wrapper, sizeType row = 0,
             sizeType column = 0,
             sizeType rows = autoSizeMask,
             sizeType columns = autoSizeMask);
  sizeType getColumnsSize() const override;
  sizeType getRowsSize() const override;
  real getData(sizeType row, sizeType column) const override;

private:
  void validate();

  const Wrapper& wrapper_;
  sizeType row_, column_, rows_, columns_;
};

class Minor : public Wrapper {
public:
  Minor(Wrapper& wrapper, sizeType row = 0,
        sizeType column = 0,
        sizeType rows = autoSizeMask,
        sizeType columns = autoSizeMask);

  Minor& operator=(ConstMinor minor);

  sizeType getColumnsSize() const override;
  sizeType getRowsSize() const override;
  real getData(sizeType row,
                     sizeType column) const override;
  void setData(sizeType row, sizeType column, real data) override;

  Wrapper& getWrapper();
  sizeType getRowMinor();
  sizeType getColumnMinor();

  operator ConstMinor() const;

private:
  void validate();

  Wrapper& wrapper_;
  sizeType row_, column_, rows_, columns_;
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
  void resize(sizeType rows, sizeType columns, real data) override;
  sizeType getColumnsSize() const override;
  sizeType getRowsSize() const override;
  real getData(sizeType row, sizeType column) const override;
  void setData(sizeType row, sizeType column, real data) override;

private:
  sizeType rows_, columns_;
  std::vector< std::vector<real> > matrix_;
};

const Matrix operator*(ConstMinor lhs, ConstMinor rhs);
const Matrix operator+(ConstMinor lhs, ConstMinor rhs);
const Matrix operator-(ConstMinor lhs, ConstMinor rhs);
std::ostream& operator<<(std::ostream& output, ConstMinor data);
std::istream& operator>>(std::istream& input, Minor data);

}

#endif // MATRIX_H
