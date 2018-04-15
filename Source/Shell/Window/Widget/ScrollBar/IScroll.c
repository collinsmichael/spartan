#include <Kernel.h>
#include <Shell.h>

extern IWindow   *Window;
extern IEvent    *Event;

bool ScrollProc(CWindow *win, CEvent *evt);

CWindow *IWidget_Scroll(CWindow *parent, int Orientation) {
    CWindow *scroll = Window->Create(parent, ScrollProc);
    if (!scroll) return 0;
    scroll->Orientation = Orientation;

    CScroll *data = (CScroll*)scroll->CustomData;
    data->TrackMin  = 0;
    data->TrackPos  = 0;
    data->TrackMax  = 0;
    data->ThumbSize = 24;
    data->PageSize  = 0;
    data->UnitCount = 0;
    data->UnitSize  = 0;
    return scroll;
}

bool IScroll_SetTrackMin(CWindow *scroll, int TrackMin) {
    if (!scroll) return false;
    CScroll *data = (CScroll*)scroll->CustomData;
    if (!data) return false;
    data->TrackMin = TrackMin;
    data->TrackPos = MIN(MAX(data->TrackMin, data->TrackPos), data->TrackMax);
    return true;
}

bool IScroll_SetTrackPos(CWindow *scroll, int TrackPos) {
    if (!scroll) return false;
    CScroll *data = (CScroll*)scroll->CustomData;
    if (!data) return false;
    data->TrackPos = TrackPos;
    data->TrackPos = MIN(MAX(data->TrackMin, data->TrackPos), data->TrackMax);
    return true;
}

bool IScroll_SetTrackMax(CWindow *scroll, int TrackMax) {
    if (!scroll) return false;
    CScroll *data = (CScroll*)scroll->CustomData;
    if (!data) return false;
    data->TrackMax = TrackMax;
    data->TrackPos = MIN(MAX(data->TrackMin, data->TrackPos), data->TrackMax);
    return true;
}

bool IScroll_SetThumbSize(CWindow *scroll, int ThumbSize) {
    if (!scroll) return false;
    CScroll *data = (CScroll*)scroll->CustomData;
    if (!data) return false;
    data->ThumbSize = ThumbSize;
    return true;
}

bool IScroll_SetPageSize(CWindow *scroll, int PageSize) {
    if (!scroll) return false;
    CScroll *data = (CScroll*)scroll->CustomData;
    if (!data) return false;
    data->PageSize = PageSize;
    return true;
}

bool IScroll_SetUnitCount(CWindow *scroll, int UnitCount) {
    if (!scroll) return false;
    CScroll *data = (CScroll*)scroll->CustomData;
    if (!data) return false;
    data->UnitCount = UnitCount;
    return true;
}

bool IScroll_SetUnitSize(CWindow *scroll, int UnitSize) {
    if (!scroll) return false;
    CScroll *data = (CScroll*)scroll->CustomData;
    if (!data) return false;
    data->UnitSize = UnitSize;
    return true;
}


bool IScroll_Resize(CWindow *win, int x1, int y1, int x2, int y2) {
    if (!win) return false;
    CScroll *data = (CScroll*)win->CustomData;
    if (!data) return false;

    win->PosX = x1;
    win->PosY = y1;
    win->Wide = x2 - x1;
    win->High = y2 - y1;

    int p1 = 0;
    int p2 = 0;
    if (win->Orientation == WIN_ORIENT_HORZ) {
        p1 = win->PosX + 24;
        p2 = win->PosX + win->Wide - 24;
    }
    if (win->Orientation == WIN_ORIENT_VERT) {
        p1 = win->PosY + 24;
        p2 = win->PosY + win->High - 24;
    }

    int unit  = MAX(1, data->UnitSize);
    int count = MAX(1, data->UnitCount);
    data->PageSize = (p2 - p1 + unit-1)/unit;
    data->ThumbSize = data->PageSize*(p2 - p1)/count;
    data->ThumbSize = MAX(24, data->ThumbSize);

    int oldmin = data->TrackMin;
    int oldpos = data->TrackPos;
    int oldmax = data->TrackMax;
    data->TrackMin = 0;
    data->TrackMax = p2 - p1 - data->ThumbSize;
    data->TrackPos = data->TrackMin + oldpos*data->TrackMax/MAX(1, oldmax-oldmin);
    data->TrackPos = MIN(MAX(data->TrackMin, data->TrackPos), data->TrackMax);
    return true;
}

bool IScroll_Goto(CWindow *win, int item) {
    if (!win) return false;
    CScroll *data = (CScroll*)win->CustomData;
    if (!data) return false;

    item = MIN(MAX(0, item), data->UnitCount-1);
    win->CurValue   = MIN(MAX(win->MinValue, win->CurValue), win->MaxValue);
    win->StepValue  = 1;
    return true;
}

int IScroll_FirstVisible(CWindow *win) {
    if (!win) return false;
    CScroll *data = (CScroll*)win->CustomData;
    if (!data) return -1;
    //data->Count = count;
    return 0;
}

int IScroll_LastVisible(CWindow *win) {
    if (!win) return false;
    CScroll *data = (CScroll*)win->CustomData;
    if (!data) return -1;
    //data->Count = count;
    return 0;
}

static IScroll iscroll = {
    IScroll_SetTrackMin,
    IScroll_SetTrackPos,
    IScroll_SetTrackMax,
    IScroll_SetThumbSize,
    IScroll_SetPageSize,
    IScroll_SetUnitCount,
    IScroll_SetUnitSize,
    IScroll_Resize,
    IScroll_Goto,
    IScroll_FirstVisible,
    IScroll_LastVisible
};

export IScroll *Scroll = &iscroll;