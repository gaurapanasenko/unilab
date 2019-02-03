/***************************************************************
 * Name:      MyApp.h
 * Purpose:   Defines Application Class
 * Author:    Egor Panasenko (gaura.panasenko@gmail.com)
 * Created:   2019-01-20
 * Copyright: Egor Panasenko (elfiny.top)
 * License:   GPLv3
 **************************************************************/

#ifndef MYAPP_H
#define MYAPP_H

#include <wx/app.h>

/// Represents the application itself
class MyApp : public wxApp {
public:
  /// \brief Creates the application's main window
  /// \return true
  virtual bool OnInit();
};

#endif // MYAPP_H
