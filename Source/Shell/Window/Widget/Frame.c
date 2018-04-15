#include <Kernel.h>
#include <Shell.h>

import IThread   *Thread;
import IMemory   *Memory;
import IProcess  *Process;
extern IGraphics *Graphics;
extern IBitmap   *Bitmap;
extern ICanvas   *Canvas;
extern IWindow   *Window;
extern IEvent    *Event;
extern IWidget   *Widget;

static bool SetTitle(CWindow *win) {
    CPeb *peb = Process->GetPeb();
    if (!peb) return false;
    if (peb->ArgC <= 0) return false;

    win->Title = peb->ArgV[0];
    for (int i = StrLen(win->Title)-1; i >= 0; i--) {
        if (win->Title[i] == '/') {
            win->Title = &win->Title[i+1];
            break;
        }
    }
    win->Text = win->Title;
    return true;
}

static int FrameOnQuit(CWindow *win, CEvent *evt) {
    Process->Exit(0);
    return true;
}

static int FrameOnLoad(CWindow *win, CEvent *evt) {
    Logger(" [info] Frame.OnLoad(win=%X, evt=%X)\n", win, evt);
    win->BackGround[0] = 0x007F7F7F;
    win->ForeGround[0] = 0x00FFFFFF;

    Window->SetTheme(win, "Any");
    SetTitle(win);

    CWindow *close = Widget->Button(win);
    if (!close) return false;

    close->Wide = 32;
    close->High = 32;
    close->PosX = win->Wide-32;
    close->PosY = 0;
    close->NewX = close->OldX = close->PosX;
    close->NewY = close->OldY = close->PosY;
    close->WindowID = -1;
    close->Bitmap = Memory->Alloc(sizeof(CBitmap));
    Bitmap->LoadSprite(close->Bitmap, "Media/SysIcons/close.bmp");
    return true;
}

static bool FrameOnDraw(CWindow *win, CEvent *evt) {
    if (!win->Visible) return true;
    Canvas->Flip(win->Canvas);

    CArea area;
    CCanvas *canvas = win->Canvas;
    int x1 = area.X1 = 0;
    int y1 = area.Y1 = 0;
    int x2 = area.X2 = win->Wide;
    int y2 = area.Y2 = win->High;

    int lut = MIN(MAX(0, win->Lut), 4);
    int *clut = &win->Clut[lut*8];

    static int poly[] = {
        1,  0, 0, -2, 0, COLOUR_3DLITE,
        1,  1, 1, -3, 1, COLOUR_HILITE,
        2,  0, 1,  1,-2, COLOUR_3DLITE,
        2,  1, 2,  2,-3, COLOUR_HILITE,

        4,  5, 30, -5, -5, COLOUR_3DSHADOW,
        4,  6, 31, -6, -6, COLOUR_SHADOW,
        4,  7, 32, -7, -7, COLOUR_BORDER,

        3,  2, 2,-33,30, COLOUR_FOREGROUND,
        3,  2,30,  5,-5, COLOUR_FOREGROUND,
        3, -4,32, -3,-5, COLOUR_FOREGROUND,
        3,  2,-4, -3,-3, COLOUR_FOREGROUND,

        2, -2, 1, -2,-2, COLOUR_3DSHADOW,
        2, -1, 0, -1,-1, COLOUR_SHADOW,
        1,  1,-2, -2,-2, COLOUR_3DSHADOW,
        1,  0,-1, -1,-1, COLOUR_SHADOW
    };

    for (int i = 0; i < elementsof(poly); i += 6) {
        int u1 = poly[i + 1];
        int v1 = poly[i + 2];
        int u2 = poly[i + 3];
        int v2 = poly[i + 4];

        u1 = (u1 >= 0) ? x1 + u1 : x2 + u1;
        v1 = (v1 >= 0) ? y1 + v1 : y2 + v1;
        u2 = (u2 >= 0) ? x1 + u2 : x2 + u2 + 1;
        v2 = (v2 >= 0) ? y1 + v2 : y2 + v2 + 1;

        int type = poly[i + 0];
        int rgba = poly[i + 5];
        rgba = clut[rgba];

        switch (type) {
        case 1: Graphics->Poly2hClip(&area, v1, u1, u2, rgba);     break;
        case 2: Graphics->Poly2vClip(&area, u1, v1, v2, rgba);     break;
        case 3: Graphics->Poly4fClip(&area, u1, v1, u2, v2, rgba); break;
        case 4: Graphics->Poly4Clip (&area, u1, v1, u2, v2, rgba); break;
        }
    }

    char *title = (win->Title) ? win->Title : win->Text;
    int tx = win->Font->FontW;
    int ty = 32/2 - win->Font->FontH/2;
    Window->TextOut(win, tx, ty, "%s", title);
    //Canvas->Blit(canvas, x1, y1, x2, y2);
    //Canvas->Flip(canvas);
    return true;
}

static int FrameOnEnter(CWindow *win, CEvent *evt) {
    win->Lut = 0;
    FrameOnDraw(win, evt);
    return true;
}

static int FrameOnLeave(CWindow *win, CEvent *evt) {
    win->Lut = 0;
    FrameOnDraw(win, evt);
    return true;
}

static int FrameOnClick(CWindow *win, CEvent *evt) {
    win->Lut = 0;
    FrameOnDraw(win, evt);
    return true;
}

static int FrameOnRelease(CWindow *win, CEvent *evt) {
    win->Lut = 0;
    FrameOnDraw(win, evt);
    return true;
}

static int FrameOnAction(CWindow *win, CEvent *evt) {
    Logger(" [info] Frame.OnAction(%X)\n", win);
    if (!evt->Source) return true;
    if (evt->Source->WindowID == -1) {
        return FrameOnQuit(win, evt);
    }
    FrameOnDraw(win, evt);
    return true;
}

static int FrameOnKeyUp(CWindow *win, CEvent *evt) {
    Logger(" [info] Frame.OnKeyUp(win=%X, evt=%X)\n", win, evt);
    FrameOnDraw(win, evt);
    return true;
}

static int FrameOnKeyDown(CWindow *win, CEvent *evt) {
    Logger(" [info] Frame.OnKeyDown(win=%X, evt=%X)\n", win, evt);
    FrameOnDraw(win, evt);
    return true;
}

static int FrameOnFocus(CWindow *win, CEvent *evt) {
    Logger(" [info] Frame.OnFocus(win=%X, evt=%X)\n", win, evt);
    CCanvas *canvas = win->Canvas;
    if (!canvas) return true;

    CWindow *close = Canvas->FindById(win, -1);
    if (!close) return true;

    CWindow *client = close->Prev;
    if (client) canvas->Focus = client;
    FrameOnDraw(win, evt);
    return true;
}

static int FrameOnSize(CWindow *win, CEvent *evt) {
    CEvtArea *area = (CEvtArea*)evt;
    CCanvas *screen = Canvas->GetScreen();
    if (screen) {
        win->Wide = MIN(MAX(128, area->Wide), screen->ResX);
        win->High = MIN(MAX( 32, area->High), screen->ResY);
    }

    CWindow *close = Canvas->FindById(win, -1);
    if (close) {
        close->PosX = win->Wide - close->Wide;
        close->PosY = 0;
        close->NewX = close->OldX = close->PosX;
        close->NewY = close->OldY = close->PosY;
    }

    CWindow *client = (close) ? close->Prev : nullptr;
    if (client) {
        area->Source = win;
        area->Target = client;
        area->PosX = 8;
        area->PosY = 32;
        area->Wide = win->Wide - 15;
        area->High = win->High - 39;
        if (client->CallBack) {
            if (client->CallBack(client, area)) {
                client->PosX = 8;
                client->PosY = 32;
                client->Wide = win->Wide - 15;
                client->High = win->High - 39;
            }
        }
    }
    FrameOnDraw(win, evt);
    return true;
}

static int FrameOnMove(CWindow *win, CEvent *evt) {
    CEvtArea *area = (CEvtArea*)evt;
    CCanvas *screen = Canvas->GetScreen();
    if (screen) {
        win->PosX = MIN(MAX(0, area->PosX), screen->ResX);
        win->PosY = MIN(MAX(0, area->PosY), screen->ResY);
    }
    return true;
}

static int FrameOnHide(CWindow *win, CEvent *evt) {
    Logger(" [info] Frame.OnHide(%d)\n", Thread->GetPid());
    //Canvas->Hide(Canvas->GetScreen(), win);
    return true;
}

static int FrameOnShow(CWindow *win, CEvent *evt) {
    Logger(" [info] Frame.OnShow(%d)\n", Thread->GetPid());
    //Canvas->Show(Canvas->GetScreen(), win);
    //FrameOnDraw(win, evt);
    return true;
}

int FrameProc(CWindow *win, CEvent *evt) {
    //Event->DebugEvent(evt);
    switch (evt->Code) {
    case EVT_ONQUIT:      return FrameOnQuit(win, evt);
    case EVT_ONLOAD:      return FrameOnLoad(win, evt);
    case EVT_ONHIDE:      return FrameOnHide(win, evt);
    case EVT_ONSHOW:      return FrameOnShow(win, evt);
    case EVT_ONMOVE:      return FrameOnMove(win, evt);
    case EVT_ONSIZE:      return FrameOnSize(win, evt);
    case EVT_ONDRAW:      return FrameOnDraw(win, evt);
    case EVT_ONLEAVE:     return FrameOnLeave(win, evt);
    case EVT_ONENTER:     return FrameOnEnter(win, evt);
    case EVT_ONCLICK:     return FrameOnClick(win, evt);
    case EVT_ONRELEASE:   return FrameOnRelease(win, evt);
    case EVT_ONACTION:    return FrameOnAction(win, evt);
    case EVT_ONKEYUP:     return FrameOnKeyUp(win, evt);
    case EVT_ONKEYDOWN:   return FrameOnKeyDown(win, evt);
    case EVT_ONFOCUS:     return FrameOnFocus(win, evt);
    
    case EVT_ONFOCUSLOST: Logger(" [info] OnFocusLost(%d)\n", Thread->GetPid()); break;
    case EVT_ONMOUSE:     Logger(" [info] OnMouse(%d)\n", Thread->GetPid()); break;
    case EVT_ONWHEEL:     Logger(" [info] OnWheel(%d)\n", Thread->GetPid()); break;
    case EVT_ONDRAG:      Logger(" [info] OnDrag(%d)\n", Thread->GetPid()); break;
    case EVT_ONDROP:      Logger(" [info] OnDrop(%d)\n", Thread->GetPid()); break;
    case EVT_ONTIMER:     Logger(" [info] OnTimer(%d)\n", Thread->GetPid()); break;
    }
    return true;
}