#include <Kernel.h>
#include <Shell.h>

extern ICanvas *Canvas;

bool OnDrop(CWindow *win, CEvent *evt) {
    Logger(" OnDrop(%X, %X)\n", win, evt);
    if (!win || !evt) return false;
    bool status = true;

    CEvtMouse *mouse = (CEvtMouse*)evt;
    int x = mouse->NewMouse.PosX;
    int y = mouse->NewMouse.PosY;

    CCanvas *canvas = win->Canvas;
    CWindow *child = Canvas->FindChild(win, x, y);
    CWindow *frame = (canvas) ? canvas->BtmMost : nullptr;

    if (child && child->CallBack) {
        status = child->CallBack(child, evt);
    } else {
        status = win->CallBack(win, evt);
    }

    if (canvas) {
        canvas->Dragged = nullptr;
    }
    //Canvas->Redraw(canvas);
    return status;
}