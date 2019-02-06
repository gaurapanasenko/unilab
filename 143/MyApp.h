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

class MyApp : public wxApp
{
    public:
        virtual bool OnInit();
};

#endif // MYAPP_H
