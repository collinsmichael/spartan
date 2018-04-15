#include <Kernel.h>
#include <Shell.h>

extern ICanvas *Canvas;

static bool PropagateOnDraw(CWindow *win, CEvent *evt) {
    if (!win || !evt) return false;
    if (!win->Visible) return true;

    if (win->CallBack) win->CallBack(win, evt);
    for (CWindow *child = win->Tail; child; child = child->Prev) {
        if (child->Visible) PropagateOnDraw(child, evt);
    }
    return true;
}

bool OnDraw(CWindow *win, CEvent *evt) {
    Logger(" OnDraw(%X, %X)\n", win, evt);
    if (!win || !evt) return false;
    if (!win->Visible) return true;
    PropagateOnDraw(win, evt);

    int x1 = win->PosX;
    int y1 = win->PosY;
    int x2 = win->Wide + x1;
    int y2 = win->High + y1;
    Canvas->Blit(win->Canvas, x1, y1, x2, y2);
    //Canvas->Flip(win->Canvas);
    return true;
}