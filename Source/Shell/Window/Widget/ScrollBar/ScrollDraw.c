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

static CBitmap *icon[4];

bool DrawScrollButton(CWindow *win, int x1, int y1, int x2, int y2) {
    CArea area;
    area.X1 = x1;
    area.Y1 = y1;
    area.X2 = x2;
    area.Y2 = y2;

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
    return true;
}

bool DrawScrollBar(CWindow *win) {
    CCanvas *canvas = win->Canvas;
    if (!canvas || !win->Visible) return true;

    CScroll *data = (CScroll*)win->CustomData;
    if (!data) return false;

    if (!icon[0]) icon[0] = IWindow_GetIcon("Media/SysIcons/up.bmp");
    if (!icon[1]) icon[1] = IWindow_GetIcon("Media/SysIcons/down.bmp");
    if (!icon[2]) icon[2] = IWindow_GetIcon("Media/SysIcons/left.bmp");
    if (!icon[3]) icon[3] = IWindow_GetIcon("Media/SysIcons/right.bmp");

    CWindow *parent = win->Parent;
    if (parent) {
        if (win->Orientation == WIN_ORIENT_HORZ) {
            win->PosX = parent->PosX;
            win->PosY = parent->PosY + parent->High - 24;
            win->Wide = parent->Wide - 24;
            win->High = 24;
        }
        if (win->Orientation == WIN_ORIENT_VERT) {
            win->PosX = parent->PosX + parent->Wide - 24;
            win->PosY = parent->PosY;
            win->Wide = 24;
            win->High = parent->High - 24;
        }
    }

    CArea area;
    int x1 = area.X1 = win->PosX;
    int y1 = area.Y1 = win->PosY;
    int x2 = area.X2 = win->PosX + win->Wide;
    int y2 = area.Y2 = win->PosY + win->High;

    int lut = MIN(MAX(0, win->Lut), 4);
    int *clut = &win->Clut[lut*8];

    DrawScrollButton(win, x1, y1, x1+24, y1+24);
    DrawScrollButton(win, x2-24, y2-24, x2, y2);

    if (win->Orientation == WIN_ORIENT_HORZ) {
        Graphics->Poly4tClip(&area, x1, y1, x1+24, y1+24, icon[2]);
        Graphics->Poly4tClip(&area, x2-24, y2-24, x2, y2, icon[3]);
        x1 = x1 + 24;
        x2 = x2 - 24;
    }
    if (win->Orientation == WIN_ORIENT_VERT) {
        Graphics->Poly4tClip(&area, x1, y1, x1+24, y1+24, icon[0]);
        Graphics->Poly4tClip(&area, x2-24, y2-24, x2, y2, icon[1]);
        y1 = y1 + 24;
        y2 = y2 - 24;
    }

    int u1 = x1;
    int v1 = y1;
    int u2 = x2;
    int v2 = y2;
    if (win->Orientation == WIN_ORIENT_HORZ) {
        u1 = x1 + data->TrackPos;
        u2 = u1 + data->ThumbSize;
    }
    if (win->Orientation == WIN_ORIENT_VERT) {
        v1 = y1 + data->TrackPos;
        v2 = v1 + data->ThumbSize;
    }

    if (win->Orientation == WIN_ORIENT_HORZ) {
        Graphics->Poly4fClip(&area, x1, y1, u1, y2, clut[COLOUR_BACKGROUND]);
        Graphics->Poly4fClip(&area, u2, y1, x2, y2, clut[COLOUR_BACKGROUND]);
    }
    if (win->Orientation == WIN_ORIENT_VERT) {
        Graphics->Poly4fClip(&area, x1, y1, x2, v1, clut[COLOUR_BACKGROUND]);
        Graphics->Poly4fClip(&area, x1, v2, x2, y2, clut[COLOUR_BACKGROUND]);
    }
    DrawScrollButton(win, u1, v1, u2, v2);

    Canvas->Blit(canvas, area.X1, area.Y1, area.X2, area.Y2);
    return true;
}