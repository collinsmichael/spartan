#include <Kernel.h>
#include <Shell.h>

extern IEvent *Event;

bool OnFocusLost(CWindow *win, CEvent *evt) {
    if (!win || !evt) return false;

    bool status = true;
    if (win->CallBack) {
        status = win->CallBack(win, evt);
        static CEvent draw;
        draw.Source = evt->Source;
        draw.Target = win;
        draw.Code   = EVT_ONDRAW;
        Event->Forward(win, &draw);
    }
    return status;
}