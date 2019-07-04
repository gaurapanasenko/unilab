/***************************************************************
 * Name:      window.h
 * Purpose:   Declaration of Window
 * Author:    Egor Panasenko (gaura.panasenko@gmail.com)
 * Created:   2019-01-20
 * Copyright: Egor Panasenko (elfiny.top)
 * License:   GPLv3
 **************************************************************/
#ifndef WINDOW_H
#define WINDOW_H

#include "treemodel.h"

#include <gtkmm/applicationwindow.h>
#include <gtkmm/builder.h>
#include <gtkmm/statusbar.h>
#include <gtkmm/button.h>
#include <gtkmm/treeview.h>
#include <gtkmm/adjustment.h>

class Window : public Gtk::ApplicationWindow {
public:
  Window(BaseObjectType* cobject, Glib::RefPtr<Gtk::Builder> builder);
  ~Window() override = default;

private:
  Window(const Window&);
  Window operator=(const Window&);
  Window(const Window&&) noexcept;
  Window operator=(const Window&&) noexcept;

  void resizeMatrix();
  void edit(const size_t& i, const Glib::ustring& path_string,
            const Glib::ustring& new_text);
  void load();
  void save();
  void run();
  void quit[[noreturn]]();

  Glib::RefPtr<Gtk::Builder>    builder_;
  Glib::RefPtr<Gtk::Adjustment> nAdjustment_;
  Glib::RefPtr<TwoStore>        dataStore_;
  Glib::RefPtr<OneStore>        resultStore_;
  Glib::RefPtr<Gtk::TreeView>   dataTreeView_;
  Glib::RefPtr<Gtk::TreeView>   resultTreeView_;
  Glib::RefPtr<Gtk::Statusbar>  statusbar_;
};

#endif //WINDOW_H
