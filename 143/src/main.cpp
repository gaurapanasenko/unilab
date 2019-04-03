/***************************************************************
 * Name:      main.cpp
 * Purpose:   Main file to start program
 * Author:    Egor Panasenko (gaura.panasenko@gmail.com)
 * Created:   2019-01-20
 * Copyright: Egor Panasenko (elfiny.top)
 * License:   GPLv3
 **************************************************************/
#include "window.h"
#include <gtkmm/main.h>
#include <glibmm/fileutils.h>
#include <iostream>

/* For testing propose use the local (not installed) ui file */
/* #define UI_FILE PACKAGE_DATA_DIR"/ui/main.ui" */
#define UI_FILE "resources/main.ui"

int main (int argc, char *argv[]) {
  Gtk::Main kit(argc, argv);


  // Load the Glade file and instiate its widgets:
  Glib::RefPtr<Gtk::Builder> builder;
  try {
    builder = Gtk::Builder::create_from_file(UI_FILE);
  } catch (const Glib::FileError & ex) {
    std::cerr << ex.what() << std::endl;
    return 1;
  }

  Window* window = nullptr;
  builder->get_widget_derived("main_window", window);
  if (window) {
    Gtk::Main::run(*window);
  }
  delete window;

  return 0;
}
