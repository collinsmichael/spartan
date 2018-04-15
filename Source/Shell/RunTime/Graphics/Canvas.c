#include <Kernel.h>
#include <Shell.h>

import IVesa    *Vesa;
import IProcess *Process;
extern IRender  *Render;

static CCanvas screen;

static void ICanvas_Debug(CCanvas *canvas, bool toggle);
static CCanvas *ICanvas_GetScreen(void);
static CCanvas *ICanvas_GetCanvas(void);
static CWindow *ICanvas_FindWindow(CWindow *head, int x, int y);
static CWindow *ICanvas_FindChild(CWindow *parent, int x, int y);
static CWindow *ICanvas_FindById(CWindow *parent, int id);
static bool ICanvas_DrawDirtyScanLine(int *pixel, CWindow *list, int y, int x1, int x4);
static void ICanvas_FindDirtyRegions(CCanvas *canvas, int y);
static void ICanvas_DebugDirtyScanLine(CCanvas *canvas, int y);

static bool ICanvas_Push(CCanvas *canvas, CWindow *window);
static bool ICanvas_Pop(CCanvas *canvas, CWindow *window);
static bool ICanvas_Show(CCanvas *canvas, CWindow *window);
static bool ICanvas_Hide(CCanvas *canvas, CWindow *window);
static bool ICanvas_Move(CCanvas *canvas, CWindow *window, int x, int y);
static bool ICanvas_Size(CCanvas *canvas, CWindow *window, int w, int h);
static bool ICanvas_Flip(CCanvas *canvas);
static bool ICanvas_Blit(CCanvas *canvas, int x1, int y1, int x2, int y2);

static ICanvas canvas = {
    ICanvas_Debug,
    ICanvas_GetScreen,
    ICanvas_GetCanvas,
    ICanvas_FindWindow,
    ICanvas_FindChild,
    ICanvas_FindById,

    ICanvas_DrawDirtyScanLine,
    ICanvas_FindDirtyRegions,
    ICanvas_DebugDirtyScanLine,

    ICanvas_Push,
    ICanvas_Pop,
    ICanvas_Show,
    ICanvas_Hide,
    ICanvas_Move,
    ICanvas_Size,
    ICanvas_Flip,
    ICanvas_Blit
};

export ICanvas *Canvas = &canvas;

static void ICanvas_Debug(CCanvas *canvas, bool toggle) {
    canvas->Debug = toggle;
    for (CWindow *list = canvas->TopMost; list; list = list->Next) {
        list->Dirty = 2;
    }
}

static CCanvas *ICanvas_GetScreen(void) {
    screen.ResX     = Vesa->ResX();
    screen.ResY     = Vesa->ResY();
    screen.ScanLine = Vesa->ScanLine();
    screen.Disp     = Vesa->Pixels();
    screen.Draw     = screen.Disp;
    return &screen;
}

static CCanvas *ICanvas_GetCanvas(void) {
    return Process->GetCanvas();
}

static CWindow *ICanvas_FindWindow(CWindow *head, int x, int y) {
    for (CWindow *list = head; list; list = list->Next) {
        if (!list->Visible) continue;

        if (x < list->PosX || x >= list->PosX + list->Wide) continue;
        if (y < list->PosY || y >= list->PosY + list->High) continue;
        //Logger(" Find(%d %d) = %X\n", x, y, list);
        return list;
    }
    return 0;
}

static CWindow *ICanvas_FindChild(CWindow *parent, int x, int y) {
    //Debug(" FindChild(%d %d)\n", x, y);
    for (;;) {
        CWindow *child = ICanvas_FindWindow(parent->Head, x, y);
        if (!child) return parent;
        else parent = child;
    }
}

static CWindow *ICanvas_FindById(CWindow *parent, int id) {
    if (!parent) return 0;
    for (CWindow *list = parent->Head; list; list = list->Next) {
        if (list->WindowID == id) {
            Logger(" FindById(%X %d) = %X\n", parent, id, list);
            return list;
        }

        CWindow *child = ICanvas_FindById(list, id);
        if (!child) continue;
        if (child->WindowID == id) {
            Logger(" FindById(%X %d) = %X\n", parent, id, child);
            return child;
        }
    }
    return 0;
}

static bool ICanvas_DrawDirtyScanLine(int *pixel, CWindow *list, int y, int x1, int x4) {
    if (!list || x1 >= x4) return true;

    // split span into three segments
    CWindow *next = list->Next;
    int x2 = MIN(MAX(x1, list->NewX), x4);
    int x3 = MIN(MAX(x1, list->NewX + list->NewW), x4);

    // the leftmost segment is before the current window
    ICanvas_DrawDirtyScanLine(pixel, next, y, x1, x2);

    // the middle segment is either the current window (or the region exposed because it moved)
    bool intersects = (y >= list->NewY && y < list->NewY + list->NewH);
    if (list->Visible && intersects) {
        int u = MIN(MAX(0, x2 - list->NewX), list->NewW);
        int v = MIN(MAX(0, y - list->NewY), list->NewH);
        if (list->Bitmap) {
            CBitmap *bmp = list->Bitmap;
            int *merged = (int*)(&bmp->Merged[-x2*4]); // (negative index corrects for x2 offset)
            ICanvas_DrawDirtyScanLine(merged, next, y, x2, x3);
            int *img = (int*)&bmp->Image[v*bmp->ScanLine];
            int *and = (int*)&bmp->Mask[v*bmp->ScanLine];
            for (int u = 0; u < x3 - x2; u++) {
                pixel[x2+u] = (merged[x2+u] & and[u]) | img[u];
            }
        } else {
            CCanvas *canvas = list->Canvas;
            int *source = (int*)&canvas->Disp[v*canvas->ScanLine];
            movsd(&pixel[x2], &source[u], x3 - x2);
        }
    } else ICanvas_DrawDirtyScanLine(pixel, next, y, x2, x3);
    
    // the rightmost segment is after the current window
    ICanvas_DrawDirtyScanLine(pixel, next, y, x3, x4);
    return true;
}

static void ICanvas_FindDirtyRegions(CCanvas *canvas, int y) {
    int x[2];
    x[0] = canvas->ResX;
    x[1] = 0;
    for (CWindow *list = canvas->TopMost; list; list = list->Next) {
        if (!list->Dirty) continue;

        int y1 = MIN(list->OldY, list->NewY);
        int y2 = MAX(list->OldY + list->OldH, list->NewY + list->NewH);
        if (y >= y1 && y < y2) {
            x[0] = MIN(x[0], list->OldX);
            x[0] = MIN(x[0], list->NewX);
            x[1] = MAX(x[1], list->OldX + list->OldW);
            x[1] = MAX(x[1], list->NewX + list->NewW);
        }
    }
    x[0] = MIN(MAX(0, x[0]), canvas->ResX);
    x[1] = MIN(MAX(0, x[1]), canvas->ResX);
    if (x[0] < x[1]) {
        int *pixel = (int*)&screen.Disp[y*screen.ScanLine];
        ICanvas_DrawDirtyScanLine(pixel, canvas->TopMost, y, x[0], x[1]);
    }
}

static void ICanvas_DebugDirtyScanLine(CCanvas *canvas, int y) {
    int overwrite = false;
    for (CWindow *list = canvas->TopMost; list; list = list->Next) {
        if (!list->Visible) continue;
        int y1 = list->PosY;
        int y2 = list->PosY + list->High;
        if (y == y1 || y == y2) {
            overwrite = true;
            break;
        }
    }

    int *pixel = (int*)&screen.Disp[y*screen.ScanLine];
    if (overwrite) {
        stosd(pixel, 0x00FFFF00, canvas->ResX);
    } else {
        ICanvas_DrawDirtyScanLine(pixel, canvas->TopMost, y, 0, canvas->ResX);
        for (CWindow *list = canvas->TopMost; list; list = list->Next) {
            if (!list->Visible) continue;
            int x1 = list->PosX;
            int x2 = list->PosX + list->Wide;
            pixel[x1] = 0x00FFFF00;
            pixel[x2] = 0x00FFFF00;
        }
    }
}

static bool ICanvas_Push(CCanvas *canvas, CWindow *window) {
    if (!canvas || !window) return false;
    return Render->Dispatch(canvas, RENDER_CMD_PUSH, window);
}

static bool ICanvas_Pop(CCanvas *canvas, CWindow *window) {
    if (!canvas || !window) return false;
    return Render->Dispatch(canvas, RENDER_CMD_POP, window);
}

static bool ICanvas_Show(CCanvas *canvas, CWindow *window) {
    if (!canvas || !window) return false;
    return Render->Dispatch(canvas, RENDER_CMD_SHOW, window);
}

static bool ICanvas_Hide(CCanvas *canvas, CWindow *window) {
    if (!canvas || !window) return false;
    return Render->Dispatch(canvas, RENDER_CMD_HIDE, window);
}

static bool ICanvas_Move(CCanvas *canvas, CWindow *window, int x, int y) {
    if (!canvas || !window) return false;
    return Render->Dispatch(canvas, RENDER_CMD_MOVE, window, x, y);
}

static bool ICanvas_Size(CCanvas *canvas, CWindow *window, int w, int h) {
    if (!canvas || !window) return false;
    return Render->Dispatch(canvas, RENDER_CMD_SIZE, window, w, h);
}

static bool ICanvas_Flip(CCanvas *canvas) {
    if (!canvas) return false;
    char *draw = canvas->Draw;
    char *disp = canvas->Disp;
    canvas->Draw = disp;
    canvas->Disp = draw;
    CWindow *window = canvas->BtmMost;
    CCanvas *screen = Canvas->GetScreen();
    return Render->Dispatch(screen, RENDER_CMD_DRAW, window);
}

static bool ICanvas_Blit(CCanvas *canvas, int x1, int y1, int x2, int y2) {
    if (!canvas) return false;

    char *draw = canvas->Draw;
    char *disp = canvas->Disp;
    canvas->Draw = disp;
    canvas->Disp = draw;

    CWindow *window = canvas->BtmMost;
    CCanvas *screen = Canvas->GetScreen();
    bool status = Render->Dispatch(screen, RENDER_CMD_DRAW, window);

    x1 = MIN(MAX(0, x1), canvas->ResX);
    y1 = MIN(MAX(0, y1), canvas->ResY);
    x2 = MIN(MAX(0, x2), canvas->ResX);
    y2 = MIN(MAX(0, y2), canvas->ResY);
    char *source = draw + y1*canvas->ScanLine;
    char *target = disp + y1*canvas->ScanLine;
    for (int y = y1; y < y2; y++) {
        int *src = (int*)source;
        int *trg = (int*)target;
        movsd(&trg[x1], &src[x1], x2 - x1);
        source += canvas->ScanLine;
        target += canvas->ScanLine;
    }
    return status;
}