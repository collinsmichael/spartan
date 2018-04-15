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

CBitmap *IWindow_GetIcon(char *path);
bool DrawScrollBar(CWindow *win);

static int clut[32] = {
    0x00C1C1C1,0x00D1D1D1,0x00000000,0x00D9D9D9,0x00E1E1E1,0x003F3F3F,0x001F1F1F,0x00000000, // normal
    0x00D1D1D1,0x00E1E1E1,0x00000000,0x00E9E9E9,0x00F1F1F1,0x004F4F4F,0x002F2F2F,0x00000000, // enter
    0x00B1B1B1,0x00C1C1C1,0x00000000,0x00C9C9C9,0x00D1D1D1,0x002F2F2F,0x000F0F0F,0x00000000, // click
    0x00C17F7F,0x00D17F7F,0x00000000,0x00D97F7F,0x00E17F7F,0x003F7F7F,0x001F7F7F,0x00000000, // hanging
};

static bool ScrollOnLoad(CWindow *win, CEvent *evt) {
    movsd(win->Clut, clut, elementsof(clut));
    win->Lut = 0;
    win->CustomData = win->Extra;

    CScroll *data = (CScroll*)win->CustomData;
    if (data) {
        data->TrackMin  = 0; // pixels
        data->TrackPos  = 0; // pixels
        data->TrackMax  = 0; // pixels
        data->ThumbSize = 0; // pixels
        data->UnitCount = 0; // integer
        data->UnitSize  = 0; // pixels
    }
    return true;
}

static bool ScrollOnDraw(CWindow *win, CEvent *evt) {
    DrawScrollBar(win);
    return true;
}

static int MouseToThumb(CWindow *win, CEvtMouse *mouse) {
    CScroll *data = (CScroll*)win->CustomData;
    if (!data) return false;

    int x1 = win->PosX;
    int y1 = win->PosY;
    int x2 = win->PosX + win->Wide;
    int y2 = win->PosY + win->High;

    if (win->Orientation == WIN_ORIENT_HORZ) {
        int u1 = x1 + 24;
        int u2 = x2 - 24 - data->ThumbSize;
        return mouse->NewMouse.PosX - u1;
    }

    if (win->Orientation == WIN_ORIENT_VERT) {
        int v1 = y1 + 24;
        int v2 = y2 - 24 - data->ThumbSize;
        return mouse->NewMouse.PosY - v1;
    }
    return data->TrackPos;
}

static bool ScrollOnEnter(CWindow *win, CEvent *evt) {
    win->Lut = 1;
    ScrollOnDraw(win, evt);
    return true;
}

static bool ScrollOnLeave(CWindow *win, CEvent *evt) {
    win->Lut = 0;
    ScrollOnDraw(win, evt);
    return true;
}

static bool ScrollOnRelease(CWindow *win, CEvent *evt) {
    win->Lut = 1;
    win->Active = false;
    ScrollOnDraw(win, evt);
    return true;
}

static bool ScrollOnDrop(CWindow *win, CEvent *evt) {
    win->Active = false;
    ScrollOnDraw(win, evt);
    return true;
}

static bool ScrollOnClick(CWindow *win, CEvent *evt) {
    win->Lut = 2;

    CScroll *data = (CScroll*)win->CustomData;
    if (!data) return false;

    CEvtMouse *mouse = (CEvtMouse*)evt;
    int thumb = MouseToThumb(win, mouse);
    if (thumb < data->TrackPos) {
        thumb = data->TrackPos - data->PageSize;
        thumb = MIN(MAX(data->TrackMin, thumb), data->TrackMax);
        data->TrackPos = thumb;
    } else if (thumb > data->TrackPos + data->ThumbSize) {
        thumb = data->TrackPos + data->PageSize;
        thumb = MIN(MAX(data->TrackMin, thumb), data->TrackMax);
        data->TrackPos = thumb;
    } else {
        win->Active = true;
        data->TrackDelta = thumb - data->TrackPos;
    }
    ScrollOnDraw(win, evt);

    CEvent action;
    stosd(&action, 0, sizeof(CEvent)/4);
    action.Source = win;
    action.Target = win->Parent;
    action.Code = EVT_ONACTION;
    Event->RaiseEvent(&action);
    return true;
}

static bool ScrollOnDrag(CWindow *win, CEvent *evt) {
    if (win->Active) {
        CScroll *data = (CScroll*)win->CustomData;
        if (!data) return false;

        CEvtMouse *mouse = (CEvtMouse*)evt;
        int thumb = MouseToThumb(win, mouse);
        thumb = thumb - data->TrackDelta;
        thumb = MIN(MAX(data->TrackMin, thumb), data->TrackMax);
        data->TrackPos = thumb;
        ScrollOnDraw(win, evt);

        CEvent action;
        stosd(&action, 0, sizeof(CEvent)/4);
        action.Source = win;
        action.Target = win->Parent;
        action.Code = EVT_ONACTION;
        Event->RaiseEvent(&action);
    }
    return true;
}

static bool ScrollOnHover(CWindow *win, CEvent *evt) {
    win->Lut = 1;
    ScrollOnDraw(win, evt);
    return true;
}

static bool ScrollOnKeyDown(CWindow *win, CEvent *evt) {
    CEvtKey *key = (CEvtKey*)evt;
    win->Lut = 2;

    CScroll *data = (CScroll*)win->CustomData;
    if (!data) return false;

    if (win->Orientation == WIN_ORIENT_HORZ) {
        if (key->Key == KEY_LEFT) {
            data->TrackPos = data->TrackPos - 1;//data->UnitSize;
            data->TrackPos = MIN(MAX(data->TrackMin, data->TrackPos), data->TrackMax);
        }
        if (key->Key == KEY_RIGHT) {
            data->TrackPos = data->TrackPos + 1;//data->UnitSize;
            data->TrackPos = MIN(MAX(data->TrackMin, data->TrackPos), data->TrackMax);
        }
    }

    if (win->Orientation == WIN_ORIENT_VERT) {
        if (key->Key == KEY_UP) {
            data->TrackPos = data->TrackPos - 1;//data->UnitSize;
            data->TrackPos = MIN(MAX(data->TrackMin, data->TrackPos), data->TrackMax);
        }
        if (key->Key == KEY_DOWN) {
            data->TrackPos = data->TrackPos + 1;//data->UnitSize;
            data->TrackPos = MIN(MAX(data->TrackMin, data->TrackPos), data->TrackMax);
        }
    }

    ScrollOnDraw(win, evt);

    CEvent action;
    stosd(&action, 0, sizeof(CEvent)/4);
    action.Source = win;
    action.Target = win->Parent;
    action.Code = EVT_ONACTION;
    Event->RaiseEvent(&action);
    return true;
}

static bool ScrollOnWheel(CWindow *win, CEvent *evt) {
    CEvtMouse *mouse = (CEvtMouse*)evt;
    CScroll *data = (CScroll*)win->CustomData;
    if (!data) return false;
    int w = mouse->NewMouse.Wheel;
    data->TrackPos = data->TrackPos + w;
    data->TrackPos = MIN(MAX(data->TrackMin, data->TrackPos), data->TrackMax);
    
    ScrollOnDraw(win, evt);

    CEvent action;
    stosd(&action, 0, sizeof(CEvent)/4);
    action.Source = win;
    action.Target = win->Parent;
    action.Code = EVT_ONACTION;
    Event->RaiseEvent(&action);
    return true;
}

bool ScrollProc(CWindow *win, CEvent *evt) {
    switch (evt->Code) {
    case EVT_ONLOAD:    return ScrollOnLoad(win, evt);
    case EVT_ONDRAW:    return ScrollOnDraw(win, evt);
    case EVT_ONENTER:   return ScrollOnEnter(win, evt);
    case EVT_ONLEAVE:   return ScrollOnLeave(win, evt);
    case EVT_ONCLICK:   return ScrollOnClick(win, evt);
    case EVT_ONRELEASE: return ScrollOnRelease(win, evt);
    case EVT_ONHOVER:   return ScrollOnHover(win, evt);
    case EVT_ONDRAG:    return ScrollOnDrag(win, evt);
    case EVT_ONDROP:    return ScrollOnDrop(win, evt);
    case EVT_ONKEYDOWN: return ScrollOnKeyDown(win, evt);
    case EVT_ONWHEEL:   return ScrollOnWheel(win, evt);
    }
    return true;
}