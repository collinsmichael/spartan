#include <Kernel.h>
#include <Shell.h>
#include <Desktop.h>

import IDevice  *Device;
import IProcess *Process;
import ICanvas  *Canvas;
import IWindow  *Window;
import IEvent   *Event;
import IBitmap  *Bitmap;
extern ICompositor *Compositor;

extern CWindow *cursor;
extern CWindow *desktop;
extern CCanvas *screen;

static CCanvas  canvas;
static CWindow *dragged;
static CBitmap  bitmap;
static CMouse   oldms;
static CMouse   newms;
static CMouse   curms;
static int dragx;
static int dragy;

static bool AdjustAndSend(CWindow *window, int code, CEvtMouse *evt) {
    // Framebuffer Co-Ordinates differ from Private Screens
    // We must cross this boundary and we must ensure the
    // co-ordinates match on both sides of the boundary
    evt->NewMouse.PosX = newms.PosX - window->PosX;
    evt->NewMouse.PosY = newms.PosY - window->PosY;
    evt->OldMouse.PosX = oldms.PosX - window->PosX;
    evt->OldMouse.PosY = oldms.PosY - window->PosY;
    evt->Target = window;
    evt->Code = code;
    return Event->RaiseEvent(evt);
}

static bool AdjustAndUpdate(CWindow *window, int code, CEvtArea *evt) {
    // Framebuffer Co-Ordinates differ from Private Screens
    // We must cross this boundary and we must ensure the
    // co-ordinates match on both sides of the boundary
    evt->PosX = newms.PosX - window->PosX;
    evt->PosY = newms.PosY - window->PosY;
    evt->Wide = window->Wide;
    evt->High = window->High;
    evt->Target = window;
    evt->Code = code;
    return Event->UpdateEvent(evt);
}

static bool ICursor_DispatchEvents(CWindow *hover, CWindow *leave) {
    static CEvent temp;
    CEvtMouse *evt = (CEvtMouse*)&temp;
    movsd(&evt->NewMouse, &newms, sizeof(CMouse)/4);
    movsd(&evt->OldMouse, &oldms, sizeof(CMouse)/4);

    if (leave != hover) {
        if (leave != 0) AdjustAndSend(leave, EVT_ONLEAVE, evt);
        if (hover != 0) AdjustAndSend(hover, EVT_ONENTER, evt);
    }

    int buttons = (curms.Left|curms.Middle|curms.Right) & 3;
    if (curms.Left == MOUSE_CLICKED && screen->Focus != hover) {
        AdjustAndSend(screen->Focus, EVT_ONFOCUSLOST, evt);
        AdjustAndSend(hover, EVT_ONFOCUS, evt);
        screen->Focus = hover;
    }
    if (buttons == MOUSE_CLICKED) {
        AdjustAndSend(hover, EVT_ONCLICK, evt);
    }
    if (buttons == MOUSE_RELEASE) {
        AdjustAndSend(hover, EVT_ONRELEASE, evt);
    }
    if (curms.Wheel) {
        AdjustAndSend(hover, EVT_ONWHEEL, evt);
    }

    if (hover != dragged) {
        AdjustAndSend(hover, EVT_ONHOVER, evt);
    }
    if (dragged) {
        int dragdrop = EVT_ONDRAG;
        int before = (oldms.Left & 3);
        int after  = (curms.Left & 3);
        if (before == MOUSE_DRAGGED && after != MOUSE_DRAGGED) {
            dragdrop = EVT_ONDROP;
        }
        AdjustAndSend(dragged, dragdrop, evt);
    }
    return true;
}

bool ICursor_DragWindow(CWindow *dragged) {
    if (!dragged) return true;

    int ox1 = dragged->PosX;
    int oy1 = dragged->PosY;
    int ox2 = dragged->Wide + ox1;
    int oy2 = dragged->High + oy1;

    int dx = newms.PosX - ox1 - dragx;
    int dy = newms.PosY - oy1 - dragy;
    int nx1 = ox1;
    int ny1 = oy1;
    int nx2 = ox2;
    int ny2 = oy2;

    bool moved = false;
    bool sized = false;
    if (dragy < 32) {
        nx1 = ox1 + dx;
        ny1 = oy1 + dy;
        nx2 = ox2 + dx;
        ny2 = oy2 + dy;
        moved = true;
    }

    if (dragx < 8) {
        nx1 = ox1 + dx;
        moved = true;
        sized = true;
    }

    if (dragx >= ox2 - ox1 - 8) {
        nx2 = ox2 + dx;
        dragx += dx;
        sized = true;
    }

    if (dragy >= oy2 - oy1 - 8) {
        ny2 = oy2 + dy;
        dragy += dy;
        sized = true;
    }

    dragged->Wide = MIN(MAX(128, nx2 - nx1), screen->ResX);
    dragged->High = MIN(MAX(128, ny2 - ny1), screen->ResY);
    dragged->PosX = MIN(MAX(0, nx1), screen->ResX - dragged->Wide);
    dragged->PosY = MIN(MAX(0, ny1), screen->ResY - dragged->High);
    dragged->Dirty = 2;

    dragx = nx1 + dragx - dragged->PosX;
    dragy = ny1 + dragy - dragged->PosY;

    if (moved) {
        Canvas->Move(screen, dragged, dragged->PosX, dragged->PosY);
    }

    if (sized) {
        static CEvent evt;
        AdjustAndUpdate(dragged, EVT_ONSIZE, (CEvtArea*)&evt);
        Canvas->Size(screen, dragged, dragged->Wide, dragged->High);
    }
    return (moved || sized);
}

bool ICursor_WindowStack(CWindow *hover) {
    switch (curms.Left) {
    case MOUSE_CLICKED:
        if (hover != screen->TopMost && hover != screen->BtmMost) {
            dragged = hover;
        }
        if (dragged) {
            dragx = newms.PosX - dragged->PosX;
            dragy = newms.PosY - dragged->PosY;
            if (dragged != cursor->Next) {
                Canvas->Pop(screen, dragged);
                Canvas->Pop(screen, cursor);
                Canvas->Push(screen, dragged);
                Canvas->Push(screen, cursor);
            }
        }
        screen->Dragged = dragged;
        screen->Focus = hover;
        break;

    case MOUSE_DRAGGED:
        //Logger("Mouse OnDragged %X\n", hover);
        return ICursor_DragWindow(dragged);

    case MOUSE_RELEASE: //Logger("Mouse OnRelease %X\n", hover);
        dragged = 0;
        screen->Dragged = dragged;
        break;
    }
    return false;
}


static bool ICursor_Hook(CPipe *pipe) {
    if (pipe->Consumed < sizeof(CMouse)) return true;

    movsd(&oldms, &newms, sizeof(CMouse)/4);
    char *buffer = (char*)&newms;
    for (u32 i = 0; i < pipe->Consumed; i++) {
        buffer[i] = pipe->Pointer[(pipe->Head + i) % pipe->Capacity];
    }
    pipe->Head = pipe->Tail;
    pipe->Consumed = 0;

    curms.PosX   = MIN(MAX(0, newms.PosX), screen->ResX);
    curms.PosY   = MIN(MAX(0, newms.PosY), screen->ResY);
    curms.Wheel  = newms.Wheel;
    curms.Left   = 2*oldms.Left   + newms.Left;
    curms.Middle = 2*oldms.Middle + newms.Middle;
    curms.Right  = 2*oldms.Right  + newms.Right;

    CWindow *leave = Canvas->FindWindow(cursor->Next, oldms.PosX, oldms.PosY);
    CWindow *hover = Canvas->FindWindow(cursor->Next, newms.PosX, newms.PosY);
    screen->Hover = hover;
    if (!ICursor_WindowStack(hover)) {
        ICursor_DispatchEvents(hover, leave);
    }

    cursor->PosX = curms.PosX;
    cursor->PosY = curms.PosY;
    cursor->Dirty = 2;
    return true;
}

bool CursorProc(CWindow *win, CEvent *evt) {
    char *path = "Media/SysIcons/arrow.bmp";
    if (!Bitmap->LoadSprite(&bitmap, path)) {
        Logger(" [FAIL] Failed to open cursor file %s\n", path);
        return false;
    }

    CCanvas *shared = Canvas->GetCanvas();
    if (!shared) return false;

    cursor = win;
    movsd(&canvas, shared, sizeof(CCanvas)/4);
    shared->Disp = shared->Draw;
    canvas.Draw = canvas.Disp;
    cursor->Canvas = &canvas;

    cursor->Next    = screen->BtmMost;
    cursor->Wide    = bitmap.ResX;
    cursor->High    = bitmap.ResY;
    cursor->Canvas  = &canvas;
    cursor->Bitmap  = &bitmap;

    curms.PosX = oldms.PosX = newms.PosX = screen->ResX/2;
    curms.PosY = oldms.PosY = newms.PosY = screen->ResY/2;
    cursor->PosX = cursor->NewX = cursor->OldX = curms.PosX;
    cursor->PosY = cursor->NewY = cursor->OldY = curms.PosY;
    cursor->NewW = cursor->OldW = cursor->Wide;
    cursor->NewH = cursor->OldH = cursor->High;

    Device->Hook('MOUS', ICursor_Hook);
    return true;
}