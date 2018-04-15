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

static bool ProgressOnLoad(CWindow *win, CEvent *evt) {
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

    Logger(" [info] Button.OnLoad(%X)\n", win);
    movsd(win->Clut, clut, elementsof(clut));
    win->Lut = 0;
    win->BackGround[0] = 0x009B9B9B;
    win->ForeGround[0] = 0x003FEF3F;
    return true;
}

static bool ProgressOnDraw(CWindow *win, CEvent *evt) {
    if (!win->Visible) return true;
    static int poly4[] = {
        0,  0,  0,  0, 0,
        1,  1, -1, -1, 1,
        2,  2, -2, -2, 2
    };
    for (int i = 0; i < elementsof(poly4); i += 5) {
        int x1 = poly4[i+0], y1 = poly4[i+1];
        int x2 = poly4[i+2], y2 = poly4[i+3];
        int c  = poly4[i+4];
        x1 += win->PosX;
        y1 += win->PosY;
        x2 += win->PosX + win->Wide;
        y2 += win->PosY + win->High;
        Graphics->Poly4(x1, y1, x2, y2, win->Clut[win->Lut*8 + c]);
    }

    static int poly4f[] = {
        3,  3, -3, -3, 3
    };
    int x1 = poly4f[0], y1 = poly4f[1];
    int x2 = poly4f[2], y2 = poly4f[3];
    int c  = poly4f[4];
    x1 += win->PosX;
    y1 += win->PosY;
    x2 += win->PosX + win->Wide;
    y2 += win->PosY + win->High;


    int w = x2 - x1;
    int p = x1 + win->CurValue*w/MAX(win->MaxValue-win->MinValue, 1);
    Graphics->Poly4f(x1, y1, p,  y2, win->ForeGround[0]);
    Graphics->Poly4f(p,  y1, x2, y2, win->BackGround[0]);

    CCanvas *canvas = win->Canvas;
    x1 = win->PosX;
    y1 = win->PosY;
    x2 = win->PosX + win->Wide;
    y2 = win->PosY + win->High;
    Canvas->Blit(canvas, x1, y1, x2, y2);
    return true;
}

bool ProgressProc(CWindow *win, CEvent *evt) {
    switch (evt->Code) {
    case EVT_ONLOAD:    return ProgressOnLoad(win, evt);
    case EVT_ONDRAW:    return ProgressOnDraw(win, evt);
    }
    return true;
}