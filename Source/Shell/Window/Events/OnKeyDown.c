#include <Kernel.h>
#include <Shell.h>

extern ICanvas *Canvas;

bool OnKeyDown(CWindow *win, CEvent *evt) {
    Logger(" OnKeyDown(win=%X, evt=%X)\n", win, evt);
    if (!win || !evt) return false;
    bool status = true;

    CCanvas *canvas = win->Canvas;
    CWindow *child = (canvas) ? canvas->Focus   : nullptr;
    CWindow *frame = (canvas) ? canvas->BtmMost : nullptr;

    if (child && child->CallBack) {
        status = child->CallBack(child, evt);
    } else {
        status = win->CallBack(win, evt);
    }
    //Canvas->Redraw(canvas);
    return status;
}