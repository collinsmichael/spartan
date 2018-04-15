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

static bool SliderOnLoad(CWindow *win, CEvent *evt) {
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

    Logger(" [info] Slider.OnLoad(%X)\n", win);
    movsd(win->Clut, clut, elementsof(clut));
    win->Lut = 0;
    win->BackGround[0] = 0x003FFF3F;
    win->BackGround[1] = 0x00EBEBEB;
    win->ForeGround[0] = 0x003FEF3F;
    win->MinValue = -100;
    win->CurValue =    0;
    win->MaxValue =  100;
    win->StepValue =   1;
    win->ThumbWidth = 32;

    return true;
}

static bool SliderOnDraw(CWindow *win, CEvent *evt) {
    if (!win->Visible) return true;

    int x1 = win->PosX;
    int y1 = win->PosY;
    int x2 = win->PosX + win->Wide;
    int y2 = win->PosY + win->High;
    int cx = (x1 + x2) / 2;
    int cy = (y1 + y2) / 2;

    static int poly4[] = {
        0,  0,  0,  0, 0,
        1,  1, -1, -1, 1,
        2,  2, -2, -2, 2
    };
    for (int i = 0; i < elementsof(poly4); i += 5) {
        int u1 = poly4[i+0], v1 = poly4[i+1];
        int u2 = poly4[i+2], v2 = poly4[i+3];
        int c  = poly4[i+4];
        u1 += x1; v1 += y1;
        u2 += x2; v2 += y2;
        Graphics->Poly4(u1, v1, u2, v2, win->Clut[win->Lut*8 + c]);
    }

    static int poly4f[] = {
        3,  3, -3, -3, 3
    };
    int u1 = poly4f[0], v1 = poly4f[1];
    int u2 = poly4f[2], v2 = poly4f[3];
    int c  = poly4f[4];
    u1 += x1; v1 += y1;
    u2 += x2; v2 += y2;
    Graphics->Poly4f(u1, v1,   u2, v2,   win->BackGround[1]);

    int range = win->MaxValue - win->MinValue;
    if (win->Orientation == WIN_ORIENT_HORZ) {
        Graphics->Poly2h(cy-4, u1, u2, win->Clut[win->Lut*8 + 0]);
        Graphics->Poly2h(cy-3, u1, u2, win->Clut[win->Lut*8 + 1]);
        Graphics->Poly2h(cy-2, u1, u2, win->Clut[win->Lut*8 + 2]);
        Graphics->Poly4f(u1, cy-1, u2, cy+2, win->Clut[win->Lut*8 + 3]);
        Graphics->Poly2h(cy+2, u1, u2, win->Clut[win->Lut*8 + 2]);
        Graphics->Poly2h(cy+3, u1, u2, win->Clut[win->Lut*8 + 1]);
        Graphics->Poly2h(cy+4, u1, u2, win->Clut[win->Lut*8 + 0]);

        int minx = x1 + win->ThumbWidth/2;
        int maxx = x2 - win->ThumbWidth/2;
        int thumbx = minx + win->CurValue*(maxx - minx)/MAX(range, 1);
        int thumbx1 = thumbx - win->ThumbWidth/2;
        int thumbx2 = thumbx + win->ThumbWidth/2;
        Graphics->Poly4(thumbx1+0, y1+0, thumbx2-0, y2-0, win->Clut[win->Lut*8 + 0]);
        Graphics->Poly4(thumbx1+1, y1+1, thumbx2-1, y2-1, win->Clut[win->Lut*8 + 1]);
        Graphics->Poly4(thumbx1+2, y1+2, thumbx2-2, y2-2, win->Clut[win->Lut*8 + 2]);
        Graphics->Poly4f(thumbx1+3, y1+3, thumbx2-3, y2-3, win->Clut[win->Lut*8 + 3]);
    }
    if (win->Orientation == WIN_ORIENT_VERT) {
        Graphics->Poly2v(cx-4, v1, v2, win->Clut[win->Lut*8 + 0]);
        Graphics->Poly2v(cx-3, v1, v2, win->Clut[win->Lut*8 + 1]);
        Graphics->Poly2v(cx-2, v1, v2, win->Clut[win->Lut*8 + 2]);
        Graphics->Poly4f(cx-1, v1, cx+2, v2, win->Clut[win->Lut*8 + 3]);
        Graphics->Poly2v(cx+2, v1, v2, win->Clut[win->Lut*8 + 2]);
        Graphics->Poly2v(cx+3, v1, v2, win->Clut[win->Lut*8 + 1]);
        Graphics->Poly2v(cx+4, v1, v2, win->Clut[win->Lut*8 + 0]);

        int miny = y1 + win->ThumbWidth/2;
        int maxy = y2 - win->ThumbWidth/2;
        int thumby = miny + win->CurValue*(maxy - miny)/MAX(range, 1);
        int thumby1 = thumby - win->ThumbWidth/2;
        int thumby2 = thumby + win->ThumbWidth/2;
        Graphics->Poly4(x1+0, thumby1+0, x2-0, thumby2-0, win->Clut[win->Lut*8 + 0]);
        Graphics->Poly4(x1+1, thumby1+1, x2-1, thumby2-1, win->Clut[win->Lut*8 + 1]);
        Graphics->Poly4(x1+2, thumby1+2, x2-2, thumby2-2, win->Clut[win->Lut*8 + 2]);
        Graphics->Poly4f(x1+3, thumby1+3, x2-3, thumby2-3, win->Clut[win->Lut*8 + 3]);
    }

    CCanvas *canvas = win->Canvas;
    Canvas->Blit(canvas, x1, y1, x2, y2);
    return true;
}

static bool SliderOnEnter(CWindow *win, CEvent *evt) {
    Logger(" [info] Slider.OnEnter(%X)\n", win);
    win->Lut = 1;
    win->Active = false;
    SliderOnDraw(win, evt);
    return true;
}

static bool SliderOnLeave(CWindow *win, CEvent *evt) {
    Logger(" [info] Slider.OnLeave(%X)\n", win);
    win->Lut = 0;
    win->Active = false;
    SliderOnDraw(win, evt);
    return true;
}

static bool SliderOnRelease(CWindow *win, CEvent *evt) {
    Logger(" [info] Slider.OnRelease(%X)\n", win);
    win->Lut = 1;
    win->Active = false;
    SliderOnDraw(win, evt);
    return true;
}

static int MouseToThumb(CWindow *win, CEvtMouse *mouse) {
    int range  = win->MaxValue - win->MinValue;

    if (win->Orientation == WIN_ORIENT_HORZ) {
        int u1 = win->PosX + win->ThumbWidth/2;
        int u2 = win->PosX + win->Wide - win->ThumbWidth/2;
        int u = mouse->NewMouse.PosX;
        int thumbx = win->MinValue + (u-u1)*range/MAX(u2-u1, 1);
        thumbx = MIN(MAX(win->MinValue, thumbx), win->MaxValue);
        return thumbx;
    }

    if (win->Orientation == WIN_ORIENT_VERT) {
        int v1 = win->PosY + win->ThumbWidth/2;
        int v2 = win->PosY + win->High - win->ThumbWidth/2;
        int v = mouse->NewMouse.PosY;
        int thumby = win->MinValue + (v-v1)*range/MAX(v2-v1, 1);
        thumby = MIN(MAX(win->MinValue, thumby), win->MaxValue);
        return thumby;
    }

    return win->CurValue;
}

static bool SliderOnClick(CWindow *win, CEvent *evt) {
    Logger(" [info] Slider.OnClick(%X)\n", win);
    win->Lut = 2;
    win->Active = true;

    CEvtMouse *mouse = (CEvtMouse*)evt;
    int thumb = MouseToThumb(win, mouse);
    if (thumb < win->CurValue - win->ThumbWidth/2) {
        thumb = MIN(MAX(win->MinValue, win->CurValue - win->StepValue), win->MaxValue);
        win->CurValue = thumb;
    }
    if (thumb > win->CurValue + win->ThumbWidth/2) {
        thumb = MIN(MAX(win->MinValue, win->CurValue + win->StepValue), win->MaxValue);
        win->CurValue = thumb;
    }
    SliderOnDraw(win, evt);
    return true;
}

static bool SliderOnDrag(CWindow *win, CEvent *evt) {
    win->Lut = 1;
    if (win->Active) {
        CEvtMouse *mouse = (CEvtMouse*)evt;
        int thumb = MouseToThumb(win, mouse);
        win->CurValue = thumb;

        SliderOnDraw(win, evt);

        CEvent action;
        stosd(&action, 0, sizeof(CEvent)/4);
        action.Source = win;
        action.Target = win->Parent;
        action.Code = EVT_ONACTION;
        Event->RaiseEvent(&action);
    }
    return true;
}

static bool SliderOnHover(CWindow *win, CEvent *evt) {
    win->Lut = 1;
    SliderOnDraw(win, evt);
    return true;
}

static bool SliderOnKeyDown(CWindow *win, CEvent *evt) {
    Logger(" [info] Slider.OnClick(%X)\n", win);
    win->Lut = 2;
    win->Active = true;

    CEvtKey *key = (CEvtKey*)evt;

    if (win->Orientation == WIN_ORIENT_HORZ) {
        if (key->Key == KEY_LEFT) {
            int thumb = MIN(MAX(win->MinValue, win->CurValue - win->StepValue), win->MaxValue);
            win->CurValue = thumb;
        }
        if (key->Key == KEY_RIGHT) {
            int thumb = MIN(MAX(win->MinValue, win->CurValue + win->StepValue), win->MaxValue);
            win->CurValue = thumb;
        }
    }

    if (win->Orientation == WIN_ORIENT_VERT) {
        if (key->Key == KEY_UP) {
            int thumb = MIN(MAX(win->MinValue, win->CurValue - win->StepValue), win->MaxValue);
            win->CurValue = thumb;
        }
        if (key->Key == KEY_DOWN) {
            int thumb = MIN(MAX(win->MinValue, win->CurValue + win->StepValue), win->MaxValue);
            win->CurValue = thumb;
        }
    }

    SliderOnDraw(win, evt);
    return true;
}

bool SliderProc(CWindow *win, CEvent *evt) {
    switch (evt->Code) {
    case EVT_ONLOAD:    return SliderOnLoad(win, evt);
    case EVT_ONDRAW:    return SliderOnDraw(win, evt);
    case EVT_ONENTER:   return SliderOnEnter(win, evt);
    case EVT_ONLEAVE:   return SliderOnLeave(win, evt);
    case EVT_ONCLICK:   return SliderOnClick(win, evt);
    case EVT_ONRELEASE: return SliderOnRelease(win, evt);
    case EVT_ONHOVER:   return SliderOnHover(win, evt);
    case EVT_ONDRAG:    return SliderOnDrag(win, evt);
    case EVT_ONKEYDOWN: return SliderOnKeyDown(win, evt);
    }
    return true;
}