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



static bool ButtonOnLoad(CWindow *win, CEvent *evt) {
    Window->Inherit(win);

    win->Lut = 0;
    win->BackGround[0] = 0x009B9B9B;
    win->ForeGround[0] = 0x001F1F1F;
    return true;
}

static bool ButtonOnDraw(CWindow *win, CEvent *evt) {
    //Debug("Button->Draw(%D)\n", win->Visible);
    if (!win->Visible) return true;

    CArea area;
    CCanvas *canvas = win->Canvas;
    int x1 = area.X1 = win->PosX;
    int y1 = area.Y1 = win->PosY;
    int x2 = area.X2 = win->PosX + win->Wide;
    int y2 = area.Y2 = win->PosY + win->High;

    int lut = MIN(MAX(0, win->Lut), 4);
    int *clut = &win->Clut[lut*8];

    static int poly[] = {
        1,  0, 0,-2, 0, COLOUR_3DLITE,
        1,  1, 1,-3, 1, COLOUR_HILITE,
        2,  0, 1, 1,-2, COLOUR_3DLITE,
        2,  1, 2, 2,-3, COLOUR_HILITE,
        3,  2, 2,-3,-3, COLOUR_FOREGROUND,
        2, -2, 1,-2,-2, COLOUR_3DSHADOW,
        2, -1, 0,-1,-1, COLOUR_SHADOW,
        1,  1,-2,-2,-2, COLOUR_3DSHADOW,
        1,  0,-1,-1,-1, COLOUR_SHADOW
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
        }
    }

    if (win->Bitmap) {
        int x1 = win->PosX;
        int y1 = win->PosY;
        int x2 = win->PosX + win->Bitmap->ResX;
        int y2 = win->PosY + win->Bitmap->ResY;

        CArea area;
        area.X1 = win->PosX;
        area.Y1 = win->PosY;
        area.X2 = win->PosX + win->Wide;
        area.Y2 = win->PosY + win->High;
        Graphics->Poly4tClip(&area, x1, y1, x2, y2, win->Bitmap);
    }

    if (win->Text) {
        if (win->Bitmap == nullptr) {
            int cx = win->PosX + win->Wide/2;
            int cy = win->PosY + win->High/2;

            int w = win->Font->FontW*StrLen(win->Text);
            int h = win->Font->FontH;

            int x1 = cx - w/2;
            int y1 = cy - h/2;
            Window->TextOut(win, x1, y1, "%s", win->Text);
        } else {
            int cy = win->PosY + win->High/2;
            int h = win->Font->FontH;
            int y1 = cy - h/2;
            int x1 = win->PosX + win->Bitmap->ResX + win->Font->FontW;
            Window->TextOut(win, x1, y1, "%s", win->Text);
        }
    }


    Canvas->Blit(canvas, x1, y1, x2, y2);
    //CCanvas *screen = Canvas->GetScreen();
    //Canvas->Show(screen, canvas->BtmMost);
    return true;
}

static bool ButtonOnEnter(CWindow *win, CEvent *evt) {
    Logger(" [info] Button.OnEnter(%X)\n", win);
    win->Lut = 1;
    ButtonOnDraw(win, evt);
    return true;
}

static bool ButtonOnLeave(CWindow *win, CEvent *evt) {
    Logger(" [info] Button.OnLeave(%X)\n", win);
    win->Active = 0;
    win->Lut = 0;
    ButtonOnDraw(win, evt);
    return true;
}

static bool ButtonOnClick(CWindow *win, CEvent *evt) {
    Logger(" [info] Button.OnClick(%X)\n", win);
    win->Active = (int)win;
    win->Lut = 2;
    ButtonOnDraw(win, evt);
    return true;
}

static bool ButtonOnRelease(CWindow *win, CEvent *evt) {
    Logger(" [info] Button.OnRelease(%X)\n", win);
    win->Lut = 1;
    ButtonOnDraw(win, evt);

    if (win->Active == (int)win) {
        CEvent action;
        stosd(&action, 0, sizeof(CEvent)/4);
        action.Source = win;
        action.Target = win->Parent;
        action.Code = EVT_ONACTION;
        Event->RaiseEvent(&action);
    }
    return true;
}

bool ButtonProc(CWindow *win, CEvent *evt) {
    switch (evt->Code) {
    case EVT_ONLOAD:    return ButtonOnLoad(win, evt);
    case EVT_ONDRAW:    return ButtonOnDraw(win, evt);
    case EVT_ONENTER:   return ButtonOnEnter(win, evt);
    case EVT_ONLEAVE:   return ButtonOnLeave(win, evt);
    case EVT_ONCLICK:   return ButtonOnClick(win, evt);
    case EVT_ONRELEASE: return ButtonOnRelease(win, evt);
    case EVT_ONDROP:    return ButtonOnRelease(win, evt);
    }
    return true;
}