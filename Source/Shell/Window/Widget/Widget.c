#include <Kernel.h>
#include <Shell.h>

extern IWindow  *Window;

bool PanelProc(CWindow *win, CEvent *evt);
bool ButtonProc(CWindow *win, CEvent *evt);
bool ProgressProc(CWindow *win, CEvent *evt);
bool SliderProc(CWindow *win, CEvent *evt);
bool ScrollProc(CWindow *win, CEvent *evt);
bool NoteBoxProc(CWindow *win, CEvent *evt);
bool ConsoleProc(CWindow *win, CEvent *evt);
bool TextBoxProc(CWindow *win, CEvent *evt);
bool ListViewProc(CWindow *win, CEvent *evt);
bool TabBarProc(CWindow *win, CEvent *evt);

CWindow *IWidget_Panel(CWindow *parent) {
    return Window->Create(parent, PanelProc);
}

CWindow *IWidget_Button(CWindow *parent) {
    return Window->Create(parent, ButtonProc);
}

CWindow *IWidget_Progress(CWindow *parent) {
    return Window->Create(parent, ProgressProc);
}

CWindow *IWidget_Slider(CWindow *parent) {
    return Window->Create(parent, SliderProc);
}

CWindow *IWidget_NoteBox(CWindow *parent) {
    return Window->Create(parent, NoteBoxProc);
}

CWindow *IWidget_Console(CWindow *parent) {
    return Window->Create(parent, ConsoleProc);
}

CWindow *IWidget_TextBox(CWindow *parent) {
    return Window->Create(parent, TextBoxProc);
}

CWindow *IWidget_ListView(CWindow *parent) {
    return Window->Create(parent, ListViewProc);
}

CWindow *IWidget_TabBar(CWindow *parent) {
    return Window->Create(parent, TabBarProc);
}

CWindow *IWidget_Scroll(CWindow *parent, int Orientation);

static IWidget widget = {
    IWidget_Panel,
    IWidget_Button,
    IWidget_Progress,
    IWidget_Slider,
    IWidget_Scroll,
    IWidget_Console,
    IWidget_TextBox,
    IWidget_ListView,
    IWidget_TabBar,
    IWidget_NoteBox
};

export IWidget *Widget = &widget;