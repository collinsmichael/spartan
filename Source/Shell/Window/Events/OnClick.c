#include <Kernel.h>
#include <Shell.h>

extern ICanvas *Canvas;
extern IEvent  *Event;

bool OnFocus(CWindow *win, CEvent *evt);
bool OnFocusLost(CWindow *win, CEvent *evt);

bool OnClick(CWindow *win, CEvent *evt) {
    Logger(" OnClick(%X, %X)\n", win, evt);
    if (!win || !evt) return false;
    bool status = true;

    CEvtMouse *mouse = (CEvtMouse*)evt;
    int x = mouse->NewMouse.PosX;
    int y = mouse->NewMouse.PosY;

    CCanvas *canvas = win->Canvas;
    CWindow *child = Canvas->FindChild(win, x, y);
    CWindow *frame = (canvas) ? canvas->BtmMost : nullptr;
    if (canvas) {
        canvas->Dragged = child;

        CWindow *focus = canvas->Focus;
        if (focus != child) {
             canvas->Focus = nullptr;

            if (focus) {
                CEvent lost;
                stosd(&lost, 0, sizeof(lost)/4);
                lost.Source = win;
                lost.Target = focus;
                lost.Code   = EVT_ONFOCUSLOST;
                OnFocusLost(focus, &lost);
            }

            if (child) {
                CEvent gain;
                stosd(&gain, 0, sizeof(gain)/4);
                gain.Source = win;
                gain.Target = child;
                gain.Code   = EVT_ONFOCUS;
                OnFocus(child, &gain);
            }
        }
    }

    if (child && child->CallBack) {
        status = child->CallBack(child, evt);
    } else {
        status = win->CallBack(win, evt);
    }
    //Canvas->Blit(win->Canvas, 0, 0, win->Wide, win->High);
    return status;
}