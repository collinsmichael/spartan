#include <Kernel.h>
#include <Shell.h>

import IClock    *Clock;
import IThread   *Thread;
import IMemory   *Memory;
import IProcess  *Process;
extern IGraphics *Graphics;
extern IBitmap   *Bitmap;
extern IWindow   *Window;
extern IEvent    *Event;
extern ICanvas   *Canvas;

static bool TabBarOnLoad(CWindow *win, CEvent *evt) {
    static int clut[32] = {
        0x005B5B5B,0x00717171,0x00818181,0x009B9B9B, // normal
        0x00000000,0x00000000,0x00000000,0x00000000,
        0x00656565,0x008B8B8B,0x009B9B9B,0x00B5B5B5, // onenter
        0x00000000,0x00000000,0x00000000,0x00000000,
        0x00515151,0x00656565,0x00717171,0x00919191, // onclick
        0x00000000,0x00000000,0x00000000,0x00000000,
        0x00512121,0x00713131,0x00813131,0x009B3B3B, // onhanging
        0x00000000,0x00000000,0x00000000,0x00000000
    };

    movsd(win->Clut, clut, elementsof(clut));
    win->Lut = 0;

    CWindow *parent = win->Parent;
    if (!parent) return false;

    int size = 4*KB + 4*KB + 256*32 + 256*align(sizeof(CBitmap), 64);
    char *base = Memory->Alloc(size);
    CTabBar *data = (CTabBar*)base;
    if (!data) return false;
    win->CustomData = data;
    win->CustomSize = size;
    stosd(data, 0, size/4);
    data->Count    = 0;
    data->Selected = -1;
    data->Items    = (CTabItem*)&data[4*KB];
    data->Texts    = (char    *)&data[8*KB];
    data->Icons    = (CBitmap *)&data[8*KB + 256*32];

    win->PosX = parent->PosX;
    win->PosY = parent->PosY;
    win->Wide = parent->Wide;
    win->High = 32;
    return true;
}

static bool TabBarOnDraw(CWindow *win, CEvent *evt) {
    if (!win->Visible) return true;
    int x1 = win->PosX;
    int y1 = win->PosY;
    int x2 = win->PosX + win->Wide;
    int y2 = win->PosY + 32;
    int c = win->Clut[win->Lut*8 + 3];
    Graphics->Poly4f(x1, y1, x2, y2, c);
    return true;
}

static bool TabBarOnSize(CWindow *win, CEvent *evt) {
    CEvtArea *area = (CEvtArea*)evt;
    win->Wide = area->Wide;
    win->High = MIN(32, area->High);
    return true;
}

static bool TabBarOnAction(CWindow *win, CEvent *evt) {
    CWindow *panel = evt->Source;
    if (!panel) return false;
    CTabBar *data = (CTabBar*)win->CustomData;
    if (!data) return false;

    int id = panel->WindowID;
    if (id < 0x100 || id >= 0x100 + data->Count) return false;
    data->Selected = id-0x100;

    CEvent action;
    stosd(&action, 0, sizeof(CEvent)/4);
    action.Code = EVT_ONACTION;
    action.Source = win;
    action.Target = win->Parent;
    Event->RaiseEvent(&action);
    action.Code = EVT_ONDRAW;
    Event->RaiseEvent(&action);
    return true;
}

bool TabBarProc(CWindow *win, CEvent *evt) {
    switch (evt->Code) {
    case EVT_ONLOAD:   return TabBarOnLoad(win, evt);
    case EVT_ONDRAW:   return TabBarOnDraw(win, evt);
    case EVT_ONSIZE:   return TabBarOnSize(win, evt);
    case EVT_ONACTION: return TabBarOnAction(win, evt);
    }
    return true;
}