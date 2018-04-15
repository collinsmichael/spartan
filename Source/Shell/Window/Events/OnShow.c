#include <Kernel.h>
#include <Shell.h>

extern ICanvas *Canvas;

bool OnShow(CWindow *win, CEvent *evt) {
    if (!win || !evt) return false;

    win->Visible = true;
    CCanvas *canvas = win->Canvas;
    if (canvas) {
        Canvas->Show(canvas, win);
        CWindow *frame = canvas->BtmMost;
        if (frame == win) {
            Canvas->Show(Canvas->GetScreen(), win);
        }
    }
    return true;
}