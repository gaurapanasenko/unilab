/***************************************************************
 * Name:      MyMain.cpp
 * Purpose:   Code for Application Frame
 * Author:    Egor Panasenko (gaura.panasenko@gmail.com)
 * Created:   2019-01-20
 * Copyright: Egor Panasenko (elfiny.top)
 * License:   GPLv3
 **************************************************************/

#include "MyMain.h"
#include <wx/msgdlg.h>
#include <wx/dcgraph.h>

//(*InternalHeaders(MyFrame)
#include <wx/string.h>
#include <wx/intl.h>
//*)

//helper functions
enum wxbuildinfoformat {
    short_f, long_f
};

wxString wxbuildinfo(wxbuildinfoformat format) {
    wxString wxbuild(wxVERSION_STRING);

    if (format == long_f )
    {
#if defined(__WXMSW__)
        wxbuild << _T("-Windows");
#elif defined(__UNIX__)
        wxbuild << _T("-Linux");
#endif

#if wxUSE_UNICODE
        wxbuild << _T("-Unicode build");
#else
        wxbuild << _T("-ANSI build");
#endif // wxUSE_UNICODE
    }

    return wxbuild;
}

//(*IdInit(MyFrame)
const long MyFrame::ID_ADDTRIANGLE = wxNewId();
const long MyFrame::ID_ADDRECTANGLE = wxNewId();
const long MyFrame::ID_TOGGLETRACE = wxNewId();
const long MyFrame::ID_RESETCOLOR = wxNewId();
const long MyFrame::ID_CHANGECOLOR = wxNewId();
const long MyFrame::ID_TOGGLEVISIBILITY = wxNewId();
const long MyFrame::ID_CLONE = wxNewId();
const long MyFrame::ID_DELETE = wxNewId();
const long MyFrame::ID_PANEL1 = wxNewId();
const long MyFrame::ID_MENUITEM1 = wxNewId();
const long MyFrame::idMenuAbout = wxNewId();
const long MyFrame::ID_STATUSBAR1 = wxNewId();
//*)

BEGIN_EVENT_TABLE(MyFrame,wxFrame)
    //(*EventTable(MyFrame)
    //*)
END_EVENT_TABLE()

wxDEFINE_EVENT(wxEVT_COMMAND_MYTHREAD_UPDATE, wxThreadEvent);

MyThread::MyThread(MyFrame& handler) : wxThread(wxTHREAD_DETACHED),
myFrame(handler) {}

MyThread::~MyThread() {
    wxCriticalSectionLocker enter(myFrame.myThreadCS);
    myFrame.myThread = NULL;
}

wxThread::ExitCode MyThread::Entry() {
    while (!TestDestroy()) {
        wxQueueEvent(&myFrame,
                     new wxThreadEvent(wxEVT_COMMAND_MYTHREAD_UPDATE));
        wxThread::This()->Sleep(10);
    }
    return (wxThread::ExitCode)0;
}

MyFrame::MyFrame(wxWindow* parent, wxWindowID id) {
    //(*Initialize(MyFrame)
    wxMenuItem* MenuItem2;
    wxMenu* Menu1;
    wxBoxSizer* BoxSizer2;
    wxMenuItem* idMenuQuit;
    wxBoxSizer* BoxSizer1;
    wxMenuBar* MenuBar1;
    wxMenu* Menu2;

    Create(parent, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxDEFAULT_FRAME_STYLE, _T("wxID_ANY"));
    SetClientSize(wxSize(512,512));
    BoxSizer1 = new wxBoxSizer(wxHORIZONTAL);
    BoxSizer2 = new wxBoxSizer(wxVERTICAL);
    AddTriangle = new wxButton(this, ID_ADDTRIANGLE, _("Add Triangle"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_ADDTRIANGLE"));
    BoxSizer2->Add(AddTriangle, 1, wxALL|wxEXPAND, 5);
    AddRectangle = new wxButton(this, ID_ADDRECTANGLE, _("Add Rectangle"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_ADDRECTANGLE"));
    BoxSizer2->Add(AddRectangle, 1, wxALL|wxEXPAND, 5);
    BoxSizer2->Add(-1,-1,1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    ToggleTrace = new wxButton(this, ID_TOGGLETRACE, _("Toggle trace"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_TOGGLETRACE"));
    BoxSizer2->Add(ToggleTrace, 1, wxALL|wxEXPAND, 5);
    ResetColor = new wxButton(this, ID_RESETCOLOR, _("Reset color"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_RESETCOLOR"));
    BoxSizer2->Add(ResetColor, 1, wxALL|wxEXPAND, 5);
    ChangeColor = new wxButton(this, ID_CHANGECOLOR, _("Change color"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_CHANGECOLOR"));
    BoxSizer2->Add(ChangeColor, 1, wxALL|wxEXPAND, 5);
    ToggleVisibility = new wxButton(this, ID_TOGGLEVISIBILITY, _("Hide/Unhide"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_TOGGLEVISIBILITY"));
    BoxSizer2->Add(ToggleVisibility, 1, wxALL|wxEXPAND, 5);
    Clone = new wxButton(this, ID_CLONE, _("Clone"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_CLONE"));
    BoxSizer2->Add(Clone, 1, wxALL|wxEXPAND, 5);
    Delete = new wxButton(this, ID_DELETE, _("Delete"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_DELETE"));
    BoxSizer2->Add(Delete, 1, wxALL|wxEXPAND, 5);
    BoxSizer1->Add(BoxSizer2, 0, wxALL, 5);
    Panel1 = new wxPanel(this, ID_PANEL1, wxDefaultPosition, wxSize(333,176), wxTAB_TRAVERSAL, _T("ID_PANEL1"));
    Panel1->SetMaxSize(wxSize(-1,-1));
    BoxSizer1->Add(Panel1, 2, wxALL|wxEXPAND, 5);
    SetSizer(BoxSizer1);
    MenuBar1 = new wxMenuBar();
    Menu1 = new wxMenu();
    idMenuQuit = new wxMenuItem(Menu1, ID_MENUITEM1, _("Quit\tAlt-F4"), _("Quit the application"), wxITEM_NORMAL);
    Menu1->Append(idMenuQuit);
    MenuBar1->Append(Menu1, _("&File"));
    Menu2 = new wxMenu();
    MenuItem2 = new wxMenuItem(Menu2, idMenuAbout, _("About\tF1"), _("Show info about this application"), wxITEM_NORMAL);
    Menu2->Append(MenuItem2);
    MenuBar1->Append(Menu2, _("Help"));
    SetMenuBar(MenuBar1);
    StatusBar1 = new wxStatusBar(this, ID_STATUSBAR1, 0, _T("ID_STATUSBAR1"));
    int __wxStatusBarWidths_1[1] = { -1 };
    int __wxStatusBarStyles_1[1] = { wxSB_NORMAL };
    StatusBar1->SetFieldsCount(1,__wxStatusBarWidths_1);
    StatusBar1->SetStatusStyles(1,__wxStatusBarStyles_1);
    SetStatusBar(StatusBar1);
    SetSizer(BoxSizer1);
    Layout();

    Connect(ID_ADDTRIANGLE,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&MyFrame::OnAddTriangleClick);
    Connect(ID_ADDRECTANGLE,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&MyFrame::OnAddRectangleClick);
    Connect(ID_TOGGLETRACE,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&MyFrame::OnToggleTraceClick);
    Connect(ID_RESETCOLOR,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&MyFrame::OnResetColorClick);
    Connect(ID_CHANGECOLOR,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&MyFrame::OnChangeColorClick);
    Connect(ID_TOGGLEVISIBILITY,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&MyFrame::OnToggleVisibilityClick);
    Connect(ID_CLONE,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&MyFrame::OnCloneClick);
    Connect(ID_DELETE,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&MyFrame::OnDeleteClick);
    Panel1->Connect(wxEVT_PAINT,(wxObjectEventFunction)&MyFrame::OnPanel1Paint,0,this);
    Panel1->Connect(wxEVT_LEFT_DOWN,(wxObjectEventFunction)&MyFrame::OnPanel1LeftDown,0,this);
    Panel1->Connect(wxEVT_LEFT_UP,(wxObjectEventFunction)&MyFrame::OnPanel1LeftUp,0,this);
    Panel1->Connect(wxEVT_MOTION,(wxObjectEventFunction)&MyFrame::OnPanel1MouseMove,0,this);
    Connect(ID_MENUITEM1,wxEVT_COMMAND_MENU_SELECTED,(wxObjectEventFunction)&MyFrame::OnQuit);
    Connect(idMenuAbout,wxEVT_COMMAND_MENU_SELECTED,(wxObjectEventFunction)&MyFrame::OnAbout);
    Connect(wxID_ANY,wxEVT_CLOSE_WINDOW,(wxObjectEventFunction)&MyFrame::OnClose);
    //*)
    Connect(wxID_ANY,wxEVT_COMMAND_MYTHREAD_UPDATE,
            (wxObjectEventFunction)&MyFrame::OnThreadUpdate);
    myThread = new MyThread(*this);
    if (myThread->Run() != wxTHREAD_NO_ERROR){
        wxLogError("Can't create the thread!");
        delete myThread;
        myThread = NULL;
        throw;
    }
}

void MyFrame::OnThreadUpdate(wxThreadEvent&) {
    Panel1->Refresh();
}

MyFrame::~MyFrame() {
    //(*Destroy(MyFrame)
    //*)
}

void MyFrame::OnQuit(wxCommandEvent& event) {
    Close();
}

void MyFrame::OnAbout(wxCommandEvent& event) {
    wxString msg = wxbuildinfo(long_f);
    wxMessageBox(msg, _("Welcome to..."));
}

void MyFrame::OnPanel1Paint(wxPaintEvent& event) {
    wxPaintDC dc(Panel1);
    wxGCDC gdc(dc);
    shapes.draw(gdc);
}

void MyFrame::OnClose(wxCloseEvent& event) {
    {
        wxCriticalSectionLocker enter(myThreadCS);
        if (myThread) {
            wxMessageOutputDebug().Printf("MYFRAME: deleting thread");
            if (myThread->Delete() != wxTHREAD_NO_ERROR )
                wxLogError("Can't delete the thread!");
        }
    }
    while (1) {
        {
            wxCriticalSectionLocker enter(myThreadCS);
            if (!myThread) break;
        }
        wxThread::This()->Sleep(1);
    }
    Destroy();
}

void MyFrame::OnAddTriangleClick(wxCommandEvent& event) {
    shapes.add(createTriangle());
}

void MyFrame::OnDeleteClick(wxCommandEvent& event) {
    shapes.erase(shapes.getActiveId());
}

void MyFrame::OnPanel1LeftDown(wxMouseEvent& event) {
    wxPoint p = event.GetLogicalPosition(wxPaintDC(Panel1));
    shapes.activate(p);
}

void MyFrame::OnPanel1MouseMove(wxMouseEvent& event) {
    wxPoint p = event.GetLogicalPosition(wxPaintDC(Panel1));
    shapes.moveActive(p);
}

void MyFrame::OnPanel1LeftUp(wxMouseEvent& event) {
    shapes.release();
}

void MyFrame::OnAddRectangleClick(wxCommandEvent& event) {
    shapes.add(createRectangle());
}

void MyFrame::OnCloneClick(wxCommandEvent& event) {
    try {
        shapes.add(shapes.getActive().clone());
    } catch(const gauraException&) {}
}

void MyFrame::OnToggleVisibilityClick(wxCommandEvent& event)
{
    try {
        shapes.getActive().toggleVisibility();
    } catch(const gauraException&) {}
}

void MyFrame::OnChangeColorClick(wxCommandEvent& event) {
    try {
        shapes.getActive().changeColor();
    } catch(const gauraException&) {}
}

void MyFrame::OnResetColorClick(wxCommandEvent& event) {
    try {
        shapes.getActive().resetColor();
    } catch(const gauraException&) {}
}

void MyFrame::OnToggleTraceClick(wxCommandEvent& event) {
    try {
        shapes.getActive().toggleTrace();
    } catch(const gauraException&) {}
}
