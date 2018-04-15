#include <Kernel.h>
#include <Shell.h>

extern IEvent  *Event;
extern ICanvas *Canvas;

bool OnSize(CWindow *win, CEvent *evt) {
    Logger(" OnSize(%X, %X)\n", win, evt);
    if (!win || !evt) return false;

    bool status = true;
    if (win->CallBack) status = win->CallBack(win, evt);

    CCanvas *canvas = win->Canvas;
    CWindow *frame = (canvas) ? canvas->BtmMost : nullptr;

    static CEvent draw;
    stosd(&draw, 0, sizeof(CEvent)/4);
    draw.Target = frame;
    draw.Code = EVT_ONDRAW;
    Event->RaiseEvent(&draw);
    return status;
}