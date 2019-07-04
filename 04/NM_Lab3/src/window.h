#ifndef WINDOW_H
#define WINDOW_H

#include "model.h"

class Window : public Gtk::ApplicationWindow {
public:
	Window(
    BaseObjectType* cobject,
    const Glib::RefPtr<Gtk::Builder>& builder
  );
  ~Window();

  void resize_matrix();
  void edit(
    const size_t& i, const Glib::ustring& path_string,
    const Glib::ustring& new_text
  );
  void load();
  void save();
  void run();
  void quit();

private:
	Glib::RefPtr<Gtk::Builder> builder_;
  Glib::RefPtr<Gtk::Adjustment> adjustmentN_;
  Glib::RefPtr<Gtk::Adjustment> adjustmentAlpha_;
  Glib::RefPtr<Gtk::CheckButton> checkButtonUseSecond_;
	Glib::RefPtr<SleStore> sleStoreData_;
	Glib::RefPtr<ColumnStore> columnStoreResult_;
	Glib::RefPtr<Gtk::TreeView> treeViewData_;
	Glib::RefPtr<Gtk::TreeView> treeViewResult_;
	Glib::RefPtr<Gtk::Statusbar> statusbar_;
};

#endif //WINDOW_H
