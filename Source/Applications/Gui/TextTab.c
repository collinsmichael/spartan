#include <Kernel.h>
#include <Shell.h>

import IMemory   *Memory;
import IFileIO   *FileIO;
import IThread   *Thread;
import IProcess  *Process;

import ICanvas   *Canvas;
import IGraphics *Graphics;
import IEvent    *Event;
import IWindow   *Window;
import IWidget   *Widget;
import ITextBox  *TextBox;

static char path[256]; // the name of the file

static char *text; // points to the text buffer
static int   size; // length of text buffer

extern CWindow *tabbar;
static CWindow *textbox;

static bool TextOnLoad(CWindow *win, CEvent *evt) {
    CWindow *parent = win->Parent;
    win->PosX = parent->PosX;
    win->PosY = parent->PosY;
    win->Wide = parent->Wide;
    win->High = parent->High;

    StrCpy(path, "Documents/ReadMe.txt");
    CFile *file = FileIO->Open(path, "-rw-");
    if (!file) return 0;
    char *data = FileIO->Base(file);
    size = FileIO->Size(file);
    FileIO->Close(file);

    text = Memory->Alloc(256*KB);
    if (!text) return 0;
    stosd(text, 0, 256*KB/4);
    movsd(text, data, (size+3)/4);

    textbox = Widget->TextBox(win);
    if (!textbox) return 0;
    TextBox->SetText(textbox, text, size);
    textbox->PosX = win->PosX;
    textbox->PosY = win->PosY;
    textbox->Wide = win->Wide;
    textbox->High = win->High;
    textbox->Visible = false;
    return true;
}

static bool TextOnDraw(CWindow *win, CEvent *evt) {
    if (!win->Visible) return true;
    CCanvas *canvas = win->Canvas;
    if (canvas) canvas->Focus = textbox;
    return true;
}

static bool TextOnAction(CWindow *win, CEvent *evt) {
    return true;
}

static bool TextOnSize(CWindow *win, CEvent *evt) {
    CEvtArea *area = (CEvtArea*)evt;
    win->Wide = area->Wide;
    win->High = area->High;
    Event->Forward(textbox, evt);
    return true;
}

bool TextProc(CWindow *win, CEvent *evt) {
    switch (evt->Code) {
    case EVT_ONLOAD:   return TextOnLoad(win, evt);
    case EVT_ONDRAW:   return TextOnDraw(win, evt);
    case EVT_ONSIZE:   return TextOnSize(win, evt);
    case EVT_ONACTION: return TextOnAction(win, evt);
    }
    return true;
}