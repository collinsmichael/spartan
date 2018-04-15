#include <Kernel.h>
#include <Shell.h>

bool OnTimer(CWindow *win, CEvent *evt) {
    Logger(" OnTimer(%X, %X)\n", win, evt);
    if (!win || !evt) return false;

    bool status = true;
    if (win->CallBack) status = win->CallBack(win, evt);
    return status;
}