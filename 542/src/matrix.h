#ifndef MATRIX_H
#define MATRIX_H

#include <climits>
#include <iterator>
#include <vector>
#include <memory>
#include <functional>

using sizeType = unsigned long long;
using real = long double;

namespace Matrix {

const sizeType AUTO_SIZE = ULLONG_MAX;
const bool DIRECTION_ROW = false;
const bool DIRECTION_COLUMN = true;

class Wrapper;
class Minor;
class ConstMinor;

template<class A>
void validate(const A& a, sizeType row, sizeType column,
              sizeType rows = 0, sizeType columns = 0,
              bool soft = false);

template<class A>
class Iterator
    : public std::iterator<std::input_iterator_tag, A> {
public:
  explicit Iterator(A value, bool direction)
    : value_(value), direction_(direction) {}
  Iterator& operator++() {
    value_.setIndex(value_.getIndex(direction_) + 1, direction_);
    return *this;
  }
  Iterator operator++(int) {
    Iterator<A> out = *this;
    value_.setIndex(value_.getIndex(direction_) + 1, direction_);
    return *this;
  }
  bool operator==(const Iterator& rhs) const{
    return value_.compare(rhs.value_);
  }
  bool operator!=(const Iterator& rhs) const{
    return !((*this) == rhs);
  }
  A operator*(){
    return value_;
  }

private:
  A value_;
  bool direction_;
};

class Cell {
public:
  explicit Cell(Wrapper& wrapper, sizeType row, sizeType column);
  Cell(const Cell&) = default;
  Cell(Cell&&) = default;
  ~Cell() = default;

  Cell& operator=(real data);
  Cell& operator=(const Cell& cell);
  Cell& operator=(Cell&& cell) noexcept;

  operator real();
  operator real() const;

  Wrapper& getWrapper() const;

  sizeType getRow() const;
  void setRow(sizeType row);

  sizeType getColumn() const;
  void setColumn(sizeType column);

  sizeType getIndex(bool direction) const;
  void setIndex(sizeType index, bool direction);

  bool compare(const Cell& cell) const;

private:
  Wrapper& wrapper_;
  sizeType row_, column_;
};

std::istream& operator>>(std::istream& input, Cell cell);

class ConstCell {
public:
  explicit ConstCell(const Wrapper& wrapper,
                     sizeType row, sizeType column);
  ConstCell(const Cell& cell);
  operator real();
  operator real() const;
  const Wrapper& getWrapper() const;

  sizeType getRow() const;
  void setRow(sizeType row);

  sizeType getColumn() const;
  void setColumn(sizeType column);

  sizeType getIndex(bool direction) const;
  void setIndex(sizeType index, bool direction);

  bool compare(const ConstCell& cell) const;

private:
  const Wrapper& wrapper_;
  sizeType row_, column_;
};

/*bool operator==(const ConstCell& lhs, const ConstCell& rhs);

real operator+(const ConstCell& a);
real operator-(const ConstCell& a);
real operator+(const ConstCell& a, const ConstCell& b);
real operator-(const ConstCell& a, const ConstCell& b);
real operator*(const ConstCell& a, const ConstCell& b);
real operator/(const ConstCell& a, const ConstCell& b);*/

class ConstVector;

class Vector {
public:
  explicit Vector(Wrapper& wrapper, sizeType index, bool direction);
  Cell operator[](sizeType index) const;

  Vector& operator=(const ConstVector& vector);
  Vector& operator=(const ConstMinor& minor);

  Iterator<Cell> begin() const;
  Iterator<Cell> end() const;
  sizeType size() const;

  Wrapper& getWrapper() const;

  sizeType getIndex(bool = true) const;
  void setIndex(sizeType index, bool);

  bool getDirection() const;

  bool compare(const Vector& vector) const;

private:
  Wrapper& wrapper_;
  sizeType index_;
  bool direction_;
};

class ConstVector {
public:
  explicit ConstVector(const Wrapper& wrapper, sizeType index,
                       bool direction);
  ConstVector(const Vector& vector);

  ConstCell operator[](sizeType index) const;

  Iterator<ConstCell> begin() const;
  Iterator<ConstCell> end() const;
  sizeType size() const;

  const Wrapper& getWrapper() const;

  sizeType getIndex(bool = true) const;
  void setIndex(sizeType index, bool);

  bool getDirection() const;

  bool compare(const ConstVector& vector) const;

private:
  const Wrapper& wrapper_;
  sizeType index_;
  bool direction_;
};

class Wrapper {
public:
  Wrapper() = default;
  Wrapper(const Wrapper&) = default;
  Wrapper(Wrapper&&) = default;
  ~Wrapper() = default;

  virtual void resize(sizeType rows, sizeType columns, real data);
  virtual sizeType getColumnsSize() const;
  virtual sizeType getRowsSize() const;
  virtual real getData(sizeType row, sizeType column) const;
  virtual void setData(sizeType row, sizeType column, real data = 0);

  Wrapper& operator=(const Wrapper& wrapper);
  Wrapper& operator=(Wrapper&&) = delete;

  Vector operator[](sizeType row);
  ConstVector operator[](sizeType row) const;

  Iterator<Vector> begin();
  Iterator<ConstVector> begin() const;
  Iterator<Vector> beginColumn();
  Iterator<ConstVector> beginColumn() const;
  Iterator<Vector> end();
  Iterator<ConstVector> end() const;
  Iterator<Vector> endColumn();
  Iterator<ConstVector> endColumn() const;

  Vector getRow(sizeType row);
  ConstVector getRow(sizeType row) const;

  Vector getColumn(sizeType column);
  ConstVector getColumn(sizeType column) const;

  sizeType size() const;

};

class Minor : public Wrapper {
public:
  Minor(Wrapper& wrapper, sizeType row = 0,
        sizeType column = 0,
        sizeType rows = AUTO_SIZE,
        sizeType columns = AUTO_SIZE);
  Minor(const Vector& vector);

  Minor& operator=(const ConstMinor& minor);

  sizeType getColumnsSize() const override;
  sizeType getRowsSize() const override;
  real getData(sizeType row,
                     sizeType column) const override;
  void setData(sizeType row, sizeType column, real data) override;

  Wrapper& getWrapper() const;
  sizeType getRowMinor() const;
  sizeType getColumnMinor() const;

private:
  void validateMinor();

  Wrapper& wrapper_;
  sizeType row_, column_, rows_, columns_;
};

class ConstMinor : public Wrapper {
public:
  ConstMinor(const Wrapper& wrapper, sizeType row = 0,
             sizeType column = 0,
             sizeType rows = AUTO_SIZE,
             sizeType columns = AUTO_SIZE);
  ConstMinor(const Vector& vector);
  ConstMinor(const ConstVector& vector);
  ConstMinor(const Minor& minor);

  sizeType getColumnsSize() const override;
  sizeType getRowsSize() const override;
  real getData(sizeType row, sizeType column) const override;

  const Wrapper& getWrapper() const;

private:
  void validateMinor();

  const Wrapper& wrapper_;
  sizeType row_, column_, rows_, columns_;
};

class Matrix;

class Merge : public Wrapper {
public:
  Merge(const std::vector<std::reference_wrapper<Wrapper>>::iterator& first,
        const std::vector<std::reference_wrapper<Wrapper>>::iterator& last,
        bool direction);

  sizeType getColumnsSize() const override;
  sizeType getRowsSize() const override;
  real getData(sizeType row, sizeType column) const override;
  void setData(sizeType row, sizeType column, real data) override;

  Wrapper& getWrapper(sizeType index) const;
  bool getDirection() const;

private:
  std::vector<std::reference_wrapper<Wrapper>> wrappers_;
  bool direction_;
};

template<typename T>
Merge mergeWrappers(T first, T last, bool direction) {
  std::vector< std::reference_wrapper<Wrapper> > w;
  for (auto i = first; i != last; i++) {
    w.emplace_back(*i);
  }
  return {w.begin(), w.end(), direction};
}

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
  Matrix(const ConstMinor& minor);

  Matrix& operator=(const ConstMinor& minor);

  void resize(sizeType rows, sizeType columns, real data) override;
  sizeType getColumnsSize() const override;
  sizeType getRowsSize() const override;
  real getData(sizeType row, sizeType column) const override;
  void setData(sizeType row, sizeType column, real data) override;

private:
  sizeType rows_, columns_;
  std::vector< std::vector<real> > matrix_;
};

const Matrix operator+(const ConstMinor& rhs);
const Matrix operator-(const ConstMinor& rhs);
const Matrix operator*(const ConstMinor& lhs, const ConstMinor& rhs);
const Matrix operator+(const ConstMinor& lhs, const ConstMinor& rhs);
const Matrix operator-(const ConstMinor& lhs, const ConstMinor& rhs);
std::ostream& operator<<(std::ostream& output, const ConstMinor& data);
std::istream& operator>>(std::istream& input, Minor data);

}

#endif // MATRIX_H
