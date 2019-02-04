/***************************************************************
 * Name:      MyMain.h
 * Purpose:   Defines Application Frame
 * Author:    Egor Panasenko (gaura.panasenko@gmail.com)
 * Created:   2019-01-20
 * Copyright: Egor Panasenko (elfiny.top)
 * License:   GPLv3
 **************************************************************/

#ifndef MYMAIN_H
#define MYMAIN_H

#include "wx_pch.h"

#include "libgaura.h"

#include <wx/wx.h>
#include <wx/dcbuffer.h>
//(*Headers(MyFrame)
#include <wx/sizer.h>
#include <wx/menu.h>
#include <wx/panel.h>
#include <wx/button.h>
#include <wx/frame.h>
#include <wx/statusbr.h>
//*)

class Shapes;

wxDECLARE_EVENT(wxEVT_COMMAND_MYTHREAD_UPDATE, wxThreadEvent);

class MyFrame;

/// Class to run thread, used in MyFrame to process animation
class MyThread : public wxThread {
    public:
        explicit MyThread(MyFrame& myFrame);
        ~MyThread();

    private:
        virtual ExitCode Entry();
        MyFrame& myFrame;
};

/// \brief Class to support gui, usually code generated automatically
/// by wxSmith
class MyFrame: public wxFrame {
    public:
        MyFrame(wxWindow* parent, wxWindowID id = -1);
        virtual ~MyFrame();

    private:
        void OnThreadUpdate(wxThreadEvent&);
        //(*Handlers(MyFrame)
        void OnQuit(wxCommandEvent& event);
        void OnAbout(wxCommandEvent& event);
        void OnPanel1Paint(wxPaintEvent& event);
        void OnClose(wxCloseEvent& event);
        void OnAddTriangleClick(wxCommandEvent& event);
        void OnDeleteClick(wxCommandEvent& event);
        void OnPanel1LeftDown(wxMouseEvent& event);
        void OnPanel1LeftUp(wxMouseEvent& event);
        void OnPanel1MouseMove(wxMouseEvent& event);
        void OnAddRectangleClick(wxCommandEvent& event);
        void OnCloneClick(wxCommandEvent& event);
        void OnToggleVisibilityClick(wxCommandEvent& event);
        void OnChangeColorClick(wxCommandEvent& event);
        void OnResetColorClick(wxCommandEvent& event);
        void OnToggleTraceClick(wxCommandEvent& event);
        void OnPanel1EraseBackground(wxEraseEvent& event);
        //*)

        //(*Identifiers(MyFrame)
        static const long ID_ADDTRIANGLE;
        static const long ID_ADDRECTANGLE;
        static const long ID_TOGGLETRACE;
        static const long ID_RESETCOLOR;
        static const long ID_CHANGECOLOR;
        static const long ID_TOGGLEVISIBILITY;
        static const long ID_CLONE;
        static const long ID_DELETE;
        static const long ID_PANEL1;
        static const long ID_MENUITEM1;
        static const long idMenuAbout;
        static const long ID_STATUSBAR1;
        //*)

        //(*Declarations(MyFrame)
        wxButton* ToggleVisibility;
        wxButton* ToggleTrace;
        wxButton* ChangeColor;
        wxPanel* Panel1;
        wxButton* AddTriangle;
        wxButton* Delete;
        wxButton* ResetColor;
        wxButton* Clone;
        wxStatusBar* StatusBar1;
        wxButton* AddRectangle;
        //*)

        MyThread* myThread;
        wxCriticalSection myThreadCS;
        friend MyThread;
        Pointer<Shapes> shapes;

        DECLARE_EVENT_TABLE()
};

#endif // MYMAIN_H

