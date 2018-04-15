#include <Kernel.h>
#include <Shell.h>

extern ICanvas *Canvas;

bool OnDrag(CWindow *win, CEvent *evt) {
    bool status = true;

    CEvtMouse *mouse = (CEvtMouse*)evt;
    int x = mouse->NewMouse.PosX;
    int y = mouse->NewMouse.PosY;

    CCanvas *canvas = win->Canvas;
    if (canvas) {
        CWindow *dragged = canvas->Dragged;
        if (dragged && dragged->CallBack) {
            status = dragged->CallBack(dragged, evt);
        } else {
            status = win->CallBack(win, evt);
        }

        CWindow *child = Canvas->FindChild(win, x, y);
        CWindow *hover = canvas->Hover;
        if (hover != child) {
            if (hover && hover) {
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

        if (child != dragged) {
            if (child && child->CallBack) {
                evt->Code = EVT_ONHOVER;
                status = child->CallBack(child, evt);
            }
        }
    }
    return status;
}