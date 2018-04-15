#include <Kernel.h>
#include <Shell.h>

extern ICanvas *Canvas;

bool OnHide(CWindow *win, CEvent *evt) {
    Logger(" OnHide(win=%X, evt=%X)\n", win, evt);
    if (!win || !evt) return false;

    win->Visible = false;
    CCanvas *canvas = win->Canvas;
    if (canvas) {
        Canvas->Hide(canvas, win);
        CWindow *frame = canvas->BtmMost;
        if (frame == win) {
            Canvas->Hide(Canvas->GetScreen(), win);
        }
    }
    return true;
}