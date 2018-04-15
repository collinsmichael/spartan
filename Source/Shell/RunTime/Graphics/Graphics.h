#ifndef SHELL_GRAPHICS_H
#define SHELL_GRAPHICS_H
#include <oop.h>
#include <Kernel\Device\Drivers\Vesa.h>
#include <Shell\Window\Window.h>
#include <Shell\RunTime\Graphics\Area.h>
#include <Shell\RunTime\Graphics\Canvas.h>
#include <Shell\RunTime\Graphics\Bitmap.h>

typedef struct IGraphics {
    bool (*Poly2v)(int x, int y1, int y2, int rgba);
    bool (*Poly2h)(int y, int x1, int x2, int rgba);
    bool (*Poly2 )(int x1, int y1, int x2, int y2, int rgba);

    bool (*Poly4  )(int x1, int y1, int x2, int y2, int rgba);
    bool (*Poly4f )(int x1, int y1, int x2, int y2, int rgba);
    bool (*Poly4gv)(int x1, int y1, int x2, int y2, int rgba[]);
    bool (*Poly4gh)(int x1, int y1, int x2, int y2, int rgba[]);
    bool (*Poly4t)(int x1, int y1, int x2, int y2, CBitmap *bitmap);

    bool (*Poly4Clip )(CArea *area, int x1, int y1, int x2, int y2, int rgba);
    bool (*Poly4fClip)(CArea *area, int x1, int y1, int x2, int y2, int rgba);
    bool (*Poly4tClip)(CArea *area, int x1, int y1, int x2, int y2, CBitmap *bitmap);
    bool (*Poly2vClip)(CArea *area, int x, int y1, int y2, int rgba);
    bool (*Poly2hClip)(CArea *area, int y, int x1, int x2, int rgba);
} IGraphics;

#endif SHELL_GRAPHICS_H