#include <Kernel.h>
#include <Shell.h>

extern ICanvas *Canvas;

bool OnLeave(CWindow *win, CEvent *evt) {
    Logger(" OnLeave(%X, %X)\n", win, evt);
    if (!win || !evt) return false;
    bool status = true;

    CCanvas *canvas = win->Canvas;
    CWindow *child = (canvas) ? canvas->Hover   : nullptr;
    CWindow *frame = (canvas) ? canvas->BtmMost : nullptr;

    if (child && child->CallBack) {
        status = child->CallBack(child, evt);
    } else {
        status = win->CallBack(win, evt);
    }
    Canvas->Blit(win->Canvas, 0, 0, win->Wide, win->High);
    return status;
}