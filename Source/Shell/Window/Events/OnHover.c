#include <Kernel.h>
#include <Shell.h>

extern ICanvas *Canvas;

bool OnHover(CWindow *win, CEvent *evt) {
    //Logger(" OnHover(%X, %X)\n", win, evt);
    if (!win || !evt) return false;
    bool status = true;

    CEvtMouse *mouse = (CEvtMouse*)evt;
    int x = mouse->NewMouse.PosX;
    int y = mouse->NewMouse.PosY;

    CCanvas *canvas = win->Canvas;
    CWindow *child = Canvas->FindChild(win, x, y);
    if (canvas) {
        CWindow *hover = canvas->Hover;
        if (hover != child) {
            if (hover) {
                CEvent leave;
                stosd(&leave, 0, sizeof(leave)/4);
                leave.Source = win;
                leave.Target = hover;
                leave.Code   = EVT_ONLEAVE;
                hover->CallBack(hover, &leave);
            }

            CWindow *frame = (canvas) ? canvas->BtmMost : nullptr;
            canvas->Hover = (child) ? child : frame;
            if (child) {
                CEvent enter;
                stosd(&enter, 0, sizeof(enter)/4);
                enter.Source = win;
                enter.Target = child;
                enter.Code   = EVT_ONENTER;
                child->CallBack(child, &enter);
            }
        }
    }

    if (child && child->CallBack) {
        status = child->CallBack(child, evt);
    } else {
        status = win->CallBack(win, evt);
    }
    return status;
}