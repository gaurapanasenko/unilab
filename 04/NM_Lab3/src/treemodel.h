#ifndef TREEMODEL_H
#define TREEMODEL_H

#include <gtkmm.h>

#include <vector>
#include <memory>
#include <algorithm>
#include <iostream>

typedef long double real;

const real eps = pow(2, -56);

class Vector : public std::vector<real> {
public:
  Vector(const size_type& count = 0, const real& default_value = 1);

  /*using vector::resize;
  using vector::operator[];
  using vector::begin;
  using vector::end;
  using vector::size;
  using vector::size_type;*/
};

Vector& operator+=(Vector& lhs, const Vector& rhs);
Vector& operator-=(Vector& lhs, const Vector& rhs);
Vector& operator*=(Vector& lhs, const Vector& rhs);
Vector& operator/=(Vector& lhs, const Vector& rhs);
const Vector operator+(Vector lhs, const Vector& rhs);
const Vector operator-(Vector lhs, const Vector& rhs);
const Vector operator*(Vector lhs, const Vector& rhs);
const Vector operator/(Vector lhs, const Vector& rhs);
const Vector operator-(Vector lhs);

Vector& operator*=(Vector& lhs, const real rhs);
Vector& operator/=(Vector& lhs, const real rhs);
const Vector operator*(Vector lhs, const real rhs);
const Vector operator/(Vector lhs, const real rhs);

std::ostream& operator<<(std::ostream& output, const Vector& data);

const real addElements(const Vector& vector);
const real addElementsAbs(const Vector& vector);

/*
class Row : public Vector {
public:
  Row(const size_type& count = 0, const real& default_value = 1);

  using Vector::operator[];
  using Vector::begin;
  using Vector::end;
  using Vector::size;
  friend class Matrix;
};
*/

class Matrix : private std::vector<Vector> {
public:
  Matrix(const size_type& count = 0, const real& default_value = 1);
  void resize(const size_type& count, const real& default_value = 1);

  using vector::operator[];
  using vector::begin;
  using vector::end;
  using vector::size;
  using vector::size_type;
};

const Vector operator*(const Matrix& lhs, const Vector& rhs);
const Vector operator*(const Vector& lhs, const Matrix& rhs);
const Matrix operator*(const Matrix& lhs, const real rhs);

const Matrix operator-(const Matrix& lhs, const Matrix& rhs);
const Matrix operator*(const Matrix& lhs, const Matrix& rhs);

std::ostream& operator<<(std::ostream& output, const Matrix& data);

typedef unsigned int sizeType;

class SimpleStore;

namespace SimpleStoreStuff {

class Cell {
public:
  Cell(
    SimpleStore& simpleStore, sizeType row, sizeType column
  );
  Cell& operator=(real data);
  Cell& operator=(const Cell& data);
  operator real();
  operator real() const;
  SimpleStore& getSimpleStore();
  const SimpleStore& getSimpleStore() const;
  sizeType& getRow();
  const sizeType& getRow() const;
  sizeType& getColumn();
  const sizeType& getColumn() const;

private:
  SimpleStore* simpleStore_;
  sizeType row_, column_;
};

bool operator==(const Cell& lhs, const Cell& rhs);

class Column {
public:
  Column(SimpleStore& simpleStore, sizeType column);
  Cell operator[](sizeType row);
  SimpleStore& getSimpleStore();

  class iterator: public std::iterator<
    std::input_iterator_tag, // iterator_category
           Cell , // value_type
           Cell , // difference_type
     const Cell*, // pointer
           Cell   // reference
  > {
  private:
    Cell cell_;

  public:
    explicit iterator(const Cell& cell);
    iterator& operator++();
    iterator operator++(int);
    bool operator==(iterator other) const;
    bool operator!=(iterator other) const;
    reference operator*() const;
  };
  iterator begin();
  iterator end();

private:
  SimpleStore& simpleStore_;
  sizeType column_;
};

class Row {
public:
  Row(SimpleStore& simpleStore, sizeType row);
  Cell operator[](sizeType column);

private:
  SimpleStore& simpleStore_;
  sizeType row_;
};

}

class SimpleStore : public Gtk::TreeModel {
protected:
  SimpleStore(sizeType size = 3);
  virtual ~SimpleStore();

public:
  Gtk::TreeModelColumn<real>& get_model_column(sizeType column);
  void resize(sizeType n);
  const sizeType columnsSize();
  const sizeType columnsSize() const;
  const sizeType rowsSize();
  const sizeType rowsSize() const;
  const real getData(sizeType row, sizeType column);
  const real getData(sizeType row, sizeType column) const;
  void setData(sizeType row, sizeType column, real data);
  SimpleStoreStuff::Row operator[](sizeType row);
  SimpleStoreStuff::Column getColumn(sizeType column);
  SimpleStore& getReference();

protected:
  virtual void resizeVirtual(sizeType n);
  virtual const sizeType columnsSizeVirtual(sizeType n) const;
  virtual const sizeType rowsSizeVirtual() const;
  virtual const real getDataVirtual(sizeType row, sizeType column) const;
  virtual void setDataVirtual(sizeType row, sizeType column, real data);

private:
  Gtk::TreeModelFlags get_flags_vfunc() const override;
  int get_n_columns_vfunc() const override;
  GType get_column_type_vfunc(int index) const override;
  bool iter_next_vfunc(
    const iterator& iter,
    iterator& iter_next
  ) const override;
  bool get_iter_vfunc(const Path& path, iterator& iter) const override;
  bool iter_children_vfunc(
    const iterator& parent,
    iterator& iter
  ) const override;
  bool iter_parent_vfunc(
    const iterator& child,
    iterator& iter
  ) const override;
  bool iter_nth_child_vfunc(
    const iterator& parent,
    int n, iterator& iter
  ) const override;
  bool iter_nth_root_child_vfunc(int n, iterator& iter) const override;
  bool iter_has_child_vfunc(const iterator& iter) const override;
  int iter_n_children_vfunc(const iterator& iter) const override;
  int iter_n_root_children_vfunc() const override;
  Path get_path_vfunc(const iterator& iter) const override;
  void get_value_vfunc(
    const TreeModel::iterator& iter,
    int column, Glib::ValueBase& value
  ) const override;
  void set_value_impl(
    const iterator& row,
    int column, const Glib::ValueBase& value
  ) override;

  bool check_treeiter_validity(const const_iterator& iter) const;

  ColumnRecord columnRecord;
  std::vector< Gtk::TreeModelColumn<real> > modelColumns;
  int stamp;
  sizeType columnsSize_;
};

std::ostream& operator<<(
  std::ostream& output, const Glib::RefPtr<SimpleStore>& store
);

std::istream& operator>>(
  std::istream& output, const Glib::RefPtr<SimpleStore>& store
);

class SleStore : public SimpleStore::SimpleStore, public Glib::Object {
protected:
  SleStore(sizeType size = 3);

public:
  static Glib::RefPtr<SleStore> create(sizeType size = 3);
  const Matrix& getA();
  const Vector& getF();

private:
  void resizeVirtual(sizeType n);
  const sizeType columnsSizeVirtual(sizeType n) const;
  const sizeType rowsSizeVirtual() const;
  const real getDataVirtual(sizeType row, sizeType column) const;
  void setDataVirtual(sizeType row, sizeType column, real data);

  Matrix a_;
  Vector f_;
};

class ColumnStore : public SimpleStore::SimpleStore, public Glib::Object {
protected:
  ColumnStore(sizeType size = 3);

public:
  static Glib::RefPtr<ColumnStore> create(sizeType size = 3);
  const Vector& getCol(sizeType index);

private:
  void resizeVirtual(sizeType n);
  const sizeType columnsSizeVirtual(sizeType n) const;
  const sizeType rowsSizeVirtual() const;
  const real getDataVirtual(sizeType row, sizeType column) const;
  void setDataVirtual(sizeType row, sizeType column, real data);

  std::vector<Vector> vector_;
};

#endif //TREEMODEL_H
