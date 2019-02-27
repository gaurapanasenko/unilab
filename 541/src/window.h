#ifndef WINDOW_H
#define WINDOW_H

#include <gtkmm.h>
#include <vector>
#include <memory>

typedef long double real;

class Vector : public std::vector<real> {
};

class Matrix : public std::vector<Vector> {
public:
  void resize(size_type count);
};

class Columns : public Gtk::TreeModelColumnRecord,
  private std::vector< Gtk::TreeModelColumn<real> > {
public:
	Columns();
  void resize(const size_t& sz);
  using vector::operator[];
  Gtk::TreeModelColumn<real> y;

private:
  typedef std::vector< Gtk::TreeModelColumn<real> > array_;
};

class Window;

class ColumnSignals {
public:
  ColumnSignals(Window& window, const size_t& index);
  void edited(const Glib::ustring& path_string,
              const Glib::ustring& new_text);
  void set_connection(Gtk::CellRendererText& cell);
  ~ColumnSignals();
private:
  Window& window_;
  size_t index_;
  sigc::connection connection_;
};

class Window : public Gtk::ApplicationWindow {
public:
	Window(BaseObjectType* cobject,
	              const Glib::RefPtr<Gtk::Builder>& builder);
  ~Window();

  void resize_matrix();
  void edit(const size_t& i, const Glib::ustring& path_string,
            const Glib::ustring& new_text);
  void quit();

private:
  Matrix a_;
  Vector y_;
  Vector x_;
	Glib::RefPtr<Gtk::Builder> builder_;
  Glib::RefPtr<Gtk::Adjustment> adjustmentN_;
	Glib::RefPtr<Gtk::ListStore> listStoreData_;
	Glib::RefPtr<Gtk::ListStore> listStoreResult_;
	Glib::RefPtr<Gtk::TreeView> treeViewData_;
	Glib::RefPtr<Gtk::TreeView> treeViewResult_;
	Columns columns_;
	std::vector<ColumnSignals> columnSignals_;
};

#endif //WINDOW_H
