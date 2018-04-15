#include <Kernel.h>
#include <Shell.h>

import IMemory   *Memory;
extern IEvent    *Event;
extern IWindow   *Window;
extern IListView *ListView;

bool ListViewOnDraw(CWindow *win, CEvent *evt);

static bool ListViewOnLoad(CWindow *win, CEvent *evt) {
    if (!Window->SetFont(win, "Media/Fonts/terminal.bmp")) return false;
    win->ForeGround[0] = 0x00E0E0E0;
    win->ForeGround[1] = 0x001F1F1F;
    win->BackGround[0] = 0x00F1F1F1;
    win->BackGround[1] = 0x00C1C1C1;
    win->BackGround[2] = 0x00CCEBFF;
    win->BackGround[3] = 0x00AACBEF;

    char *base = Memory->Alloc(64*KB);
    CListView *data = (CListView*)base;
    if (!data) return false;
    win->CustomData = data;
    win->CustomSize = 64*KB;

    stosd(data, 0, 64*KB/4);
    data->MaxRows  = (64*KB - sizeof(CListView))/4;
    data->Hover    = -1;
    data->Selected = -1;
    return true;
}

static bool ListViewOnQuit(CWindow *win, CEvent *evt) {
    char *data = win->CustomData;
    if (data) Memory->Free(data);
    win->CustomData = 0;
    win->CustomSize = 0;
    return true;
}

static bool ListViewOnWheel(CWindow *win, CEvent *evt) {
    CEvtMouse *mouse = (CEvtMouse*)evt;
    CListView *data = win->CustomData;
    if (!data) return false;

    data->Scroll += 16*mouse->NewMouse.Wheel;
    Logger(" Mouse.Wheel = %d %d\n\n", mouse->NewMouse.Wheel, data->Scroll);
    evt->Code = EVT_ONDRAW;
    return ListViewOnDraw(win, evt);
}

static bool ListViewOnHover(CWindow *win, CEvent *evt) {
    CEvtMouse *mouse = (CEvtMouse*)evt;
    CListView *data = win->CustomData;
    if (!data) return false;

    CCanvas *canvas = win->Canvas;
    if (!canvas) return false;

    CWindow *frame = canvas->BtmMost;
    if (!frame) return false;

    int y = mouse->NewMouse.PosY;
    int v = y - win->PosY;
    v = v - data->HeadSize + data->Scroll;

    int row = (data->RowSize) ? v/data->RowSize : -1;
    Logger(" Mouse.Hover = %d x %d (v=%d row=%d)\n\n", mouse->NewMouse.PosX, mouse->NewMouse.PosY, v, row);
    if (data->Hover == row) return true;
    data->Hover = row;
    Logger(" OnHover = %d %d\n", v, y);
    return ListViewOnDraw(win, evt);
}

static bool ListViewOnLeave(CWindow *win, CEvent *evt) {
    CListView *data = win->CustomData;
    if (!data) return false;
    data->Hover = -1;
    return ListViewOnDraw(win, evt);
}

static bool ListViewOnClick(CWindow *win, CEvent *evt) {
    CEvtMouse *mouse = (CEvtMouse*)evt;
    CListView *data = win->CustomData;
    if (!data) return false;

    Logger(" ListView.OnClick(%X)\n", data->Hover);
    if (data->Hover >= 0) {
        data->Selected = data->Hover;
        char **item = ListView->Retrieve(win, data->Selected);
        if (item) {
            CWindow *parent = win->Parent;
            if (parent) {
                CEvent action;
                stosd(&action, 0, sizeof(CEvent)/4);
                action.Source = win;
                action.Target = parent;
                action.Code = EVT_ONACTION;
                Event->RaiseEvent(&action);
            }
        }
    }
    return ListViewOnDraw(win, evt);
}

static bool ListViewOnRelease(CWindow *win, CEvent *evt) {
    CEvtMouse *mouse = (CEvtMouse*)evt;
    CListView *data = win->CustomData;
    if (!data) return false;
    return ListViewOnDraw(win, evt);
}

static bool ListViewOnKeyDown(CWindow *win, CEvent *evt) {
    CEvtKey *key = (CEvtKey*)evt;
    CListView *data = win->CustomData;
    if (!data) return false;

    switch (key->Key) {
    case KEY_ESC:  data->Selected = -1; break;
    case KEY_BACK: data->Selected = -1; break;
    case KEY_UP:
        if (data->Selected >= 0) {
            data->Selected = MAX(0, data->Selected-1);
        }
        data->Scroll = MAX(0, data->Scroll - data->RowSize);
        break;
    case KEY_DOWN:
        if (data->Selected >= 0) {
            data->Selected = MIN(data->NumRows-1, data->Selected+1);
        }
        data->Scroll = MIN(data->RowSize*(data->NumRows-1), data->Scroll + data->RowSize);
        break;
    case KEY_ENTER:
        if (data->Selected >= 0) {
            CWindow *parent = win->Parent;
            if (parent) {
                CEvent action;
                stosd(&action, 0, sizeof(CEvent)/4);
                action.Source = win;
                action.Target = parent;
                action.Code = EVT_ONACTION;
                Event->RaiseEvent(&action);
            }
        }
        break;
    }
    return ListViewOnDraw(win, evt);
}

static bool ListViewOnKeyUp(CWindow *win, CEvent *evt) {
    CEvtKey *key = (CEvtKey*)evt;
    CListView *data = win->CustomData;
    if (!data) return false;

    CWindow *parent = win->Parent;
    if (parent && parent->CallBack) {
        evt->Source = win;
        evt->Target = parent;
        parent->CallBack(parent, evt);
    }
    return true;
}

bool ListViewProc(CWindow *win, CEvent *evt) {
    switch (evt->Code) {
    case EVT_ONLOAD:    return ListViewOnLoad(win, evt);
    case EVT_ONQUIT:    return ListViewOnQuit(win, evt);
    case EVT_ONDRAW:    return ListViewOnDraw(win, evt);
    case EVT_ONWHEEL:   return ListViewOnWheel(win, evt);
    case EVT_ONHOVER:   return ListViewOnHover(win, evt);
    case EVT_ONLEAVE:   return ListViewOnLeave(win, evt);
    case EVT_ONCLICK:   return ListViewOnClick(win, evt);
    case EVT_ONRELEASE: return ListViewOnRelease(win, evt);
    case EVT_ONKEYDOWN: return ListViewOnKeyDown(win, evt);
    case EVT_ONKEYUP:   return ListViewOnKeyUp(win, evt);
    }
    return true;
}