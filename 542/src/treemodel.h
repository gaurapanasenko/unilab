#ifndef TREEMODEL_H
#define TREEMODEL_H

#include "matrix.h"

#include <gtkmm/treemodel.h>

class SimpleStore : public Gtk::TreeModel, Matrix::Wrapper {
protected:
  SimpleStore(sizeType size = 3);
   ~SimpleStore() override;


public:
  SimpleStore(const SimpleStore&) = delete;
  SimpleStore(SimpleStore&&) = delete;
  SimpleStore& operator=(const SimpleStore&) = delete;
  SimpleStore& operator=(SimpleStore&&) = delete;
  Gtk::TreeModelColumn<real>& get_model_column(sizeType column);
  SimpleStore& getReference();

  void resize(sizeType rows, sizeType columns, real data) override;
  sizeType getColumnsSize() const override;
  sizeType getRowsSize() const override;
  real getData(sizeType row, sizeType column) const override;
  void setData(sizeType row, sizeType column, real data) override;

protected:
  virtual void resizeVirtual(sizeType rows, sizeType columns,
                             real data);
  virtual sizeType getColumnsSizeVirtual() const;
  virtual sizeType getRowsSizeVirtual() const;
  virtual real getDataVirtual(sizeType row,
                              sizeType column) const;
  virtual void setDataVirtual(sizeType row,
                              sizeType column, real data);

private:
  Gtk::TreeModelFlags get_flags_vfunc() const override;
  int get_n_columns_vfunc() const override;
  GType get_column_type_vfunc(int index) const override;
  bool iter_next_vfunc(const iterator& iter,
                       iterator& iter_next) const override;
  bool get_iter_vfunc(const Path& path, iterator& iter) const override;
  bool iter_children_vfunc(const iterator& parent,
                           iterator& iter) const override;
  bool iter_parent_vfunc(const iterator& child,
                         iterator& iter) const override;
  bool iter_nth_child_vfunc(const iterator& parent,
                            int n, iterator& iter) const override;
  bool iter_nth_root_child_vfunc(int n, iterator& iter) const override;
  bool iter_has_child_vfunc(const iterator& iter) const override;
  int iter_n_children_vfunc(const iterator& iter) const override;
  int iter_n_root_children_vfunc() const override;
  Path get_path_vfunc(const iterator& iter) const override;
  void get_value_vfunc(const TreeModel::iterator& iter, int column,
                       Glib::ValueBase& value) const override;
  void set_value_impl(const iterator& row, int column,
                      const Glib::ValueBase& value) override;

  bool check_treeiter_validity(const const_iterator& iter) const;

private:
  ColumnRecord columnRecord;
  std::vector< Gtk::TreeModelColumn<real> > modelColumns;
  int stamp;
  sizeType columnsSize_;
};

class TwoStore : public SimpleStore::SimpleStore, public Glib::Object {
protected:
  TwoStore(sizeType size = 3);

public:
  static Glib::RefPtr<TwoStore> create(sizeType size = 3);
  const Matrix::Matrix& getA();
  const Matrix::Matrix& getY();

private:
  void resizeVirtual(sizeType rows, sizeType columns,
                     real data) override;
  sizeType getColumnsSizeVirtual() const override;
  sizeType getRowsSizeVirtual() const override;
  real getDataVirtual(sizeType row, sizeType column) const override;
  void setDataVirtual(sizeType row, sizeType column,
                      real data) override;

  Matrix::Matrix a_;
  Matrix::Matrix y_;
};

class OneStore
    : public SimpleStore::SimpleStore, public Glib::Object {
protected:
  OneStore(sizeType size = 3);

public:
  static Glib::RefPtr<OneStore> create(sizeType size = 3);

private:
  void resizeVirtual(sizeType rows, sizeType columns,
                     real data) override;
  sizeType getColumnsSizeVirtual() const override;
  sizeType getRowsSizeVirtual() const override;
  real getDataVirtual(sizeType row, sizeType column) const override;
  void setDataVirtual(sizeType row, sizeType column,
                      real data) override;

  Matrix::Matrix matrix_;
};

#endif //TREEMODEL_H
