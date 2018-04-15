#include <Kernel.h>
#include <Shell.h>

import IMemory   *Memory;
extern IWindow   *Window;
extern IGraphics *Graphics;
extern ICanvas   *Canvas;
extern ICaret    *Caret;

static bool ConsoleOnLoad(CWindow *win, CEvent *evt) {
    if (!win->Visible) return true;

    win->Text = Memory->Alloc(64*KB);

    CCanvas *canvas = win->Canvas;
    int x1 = win->PosX;
    int y1 = win->PosY;
    int x2 = win->PosX + win->Wide;
    int y2 = win->PosY + win->High;
    int c  = win->BackGround[0];
    Graphics->Poly4f(x1, y1, x2, y2, c);
    Canvas->Blit(win->Canvas, x1, y1, x2, y2);
    Graphics->Poly4f(x1, y1, x2, y2, c);
    Canvas->Blit(win->Canvas, x1, y1, x2, y2);
    return true;
}

static bool ConsoleOnDraw(CWindow *win, CEvent *evt) {
    if (!win->Visible) return true;
    CCanvas *canvas = win->Canvas;
    int x1 = win->PosX;
    int y1 = win->PosY;
    int x2 = win->PosX + win->Wide;
    int y2 = win->PosY + win->High;
    Caret->Show(win);
    Canvas->Blit(win->Canvas, x1, y1, x2, y2);
    return true;
}

static bool ConsoleOnKeyDown(CWindow *win, CEvent *evt) {
    CWindow *parent = win->Parent;
    if (parent) {
        CWindow *source = evt->Source;
        CWindow *target = evt->Target;
        evt->Source = win;
        evt->Target = parent;
        bool status = parent->CallBack(parent, evt);
        evt->Source = source;
        evt->Target = target;

        if (!status) return false;
    }

    CEvtKey *key = (CEvtKey*)evt;
    if (key->Ascii) Window->PutC(win, key->Ascii);

    CCanvas *canvas = win->Canvas;
    int x1 = win->PosX;
    int y1 = win->PosY;
    int x2 = win->PosX + win->Wide;
    int y2 = win->PosY + win->High;
    Canvas->Blit(win->Canvas, x1, y1, x2, y2);
    return true;
}

bool ConsoleProc(CWindow *win, CEvent *evt) {
    switch (evt->Code) {
    case EVT_ONLOAD:    return ConsoleOnLoad(win, evt);
    case EVT_ONDRAW:    return ConsoleOnDraw(win, evt);
    case EVT_ONKEYDOWN: return ConsoleOnKeyDown(win, evt);
    }
    return true;
}