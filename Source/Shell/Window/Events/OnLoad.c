#include <Kernel.h>
#include <Shell.h>

bool OnLoad(CWindow *win, CEvent *evt) {
    Logger(" OnLoad(%X, %X)\n", win, evt);
    if (!win || !evt) return false;

    bool status = true;
    if (win->CallBack) status = win->CallBack(win, evt);
    return status;
}