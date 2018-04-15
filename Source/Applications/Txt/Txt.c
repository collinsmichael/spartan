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

static char *line; // points to the a list of newline pointers
static char *text; // points to the text buffer
static int   size; // length of text buffer
static CWindow *textbox;

bool OnLoad(CWindow *win, CEvent *evt) {
    CCanvas *canvas = win->Canvas;
    if (!canvas) return false;
    CWindow *frame = canvas->BtmMost;
    if (!frame) return false;
    frame->Title = path;

    textbox = Widget->TextBox(win);
    if (!textbox) return 0;
    TextBox->SetText(textbox, text, size);
    return true;
}

bool OnMove(CWindow *win, CEvent *evt) {
    CEvtArea *area = (CEvtArea*)evt;
    win->PosX = area->PosX;
    win->PosY = area->PosY;
    return true;
}

bool OnSize(CWindow *win, CEvent *evt) {
    CEvtArea *area = (CEvtArea*)evt;
    win->Wide = area->Wide;
    win->High = area->High;
    Event->Forward(textbox, evt);
    return true;
}

bool OnDraw(CWindow *win, CEvent *evt) {
    if (!win->Visible) return true;

    int x1 = win->PosX;
    int y1 = win->PosY;
    int x2 = win->PosX + win->Wide - 24;
    int y2 = win->PosY + win->High - 24;
    Graphics->Poly4f(x1, y1, x2, y2, 0x00E1E1E1);

    CCanvas *canvas = win->Canvas;
    Canvas->Blit(canvas, x1, y1, x2, y2);

    return true;
}

bool TxtProc(CWindow *win, CEvent *evt) {
    switch (evt->Code) {
    case EVT_ONLOAD: return OnLoad(win, evt);
    case EVT_ONMOVE: return OnMove(win, evt);
    case EVT_ONSIZE: return OnSize(win, evt);
    case EVT_ONDRAW: return OnDraw(win, evt);
    }
    return true;
}

int main(int argc, char *argv[]) {
    StrCpy(path, "Documents/ReadMe.txt");
    if (argc > 1) StrCpy(path, argv[1]);

    Debug(" [info] Bmp.exe %s\n", path);
    CFile *file = FileIO->Open(path, "-rw-");
    if (!file) return 0;
    char *data = FileIO->Base(file);
    size = FileIO->Size(file);
    FileIO->Close(file);

    text = Memory->Alloc(256*KB);
    if (!text) return 0;
    stosd(text, 0, 256*KB/4);
    movsd(text, data, (size+3)/4);

    CWindow *frame = Window->Create(nullptr, TxtProc);
    if (!frame) return -1;
    for (CEvent evt;;) {
        if (Event->GetEvent(&evt)) {
            Event->HandleEvent(&evt);
        }
    }
    return 0;
}