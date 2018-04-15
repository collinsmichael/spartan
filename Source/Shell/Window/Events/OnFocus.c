#include <Kernel.h>
#include <Shell.h>

extern IEvent *Event;

bool OnFocus(CWindow *win, CEvent *evt) {
    if (!win || !evt) return false;

    bool status = true;
    if (win->CallBack) {
        status = win->CallBack(win, evt);
    }

    if (status) {
        CCanvas *canvas = win->Canvas;
        CWindow *frame = (canvas) ? canvas->BtmMost : nullptr;
        if (canvas && win != frame) {
            canvas->Focus = win;
            static CEvent draw;
            draw.Source = evt->Source;
            draw.Target = win;
            draw.Code   = EVT_ONDRAW;
            Event->Forward(win, &draw);
        }
    }
    return status;
}