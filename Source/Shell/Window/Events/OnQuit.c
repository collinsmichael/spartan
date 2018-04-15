#include <Kernel.h>
#include <Shell.h>

import IProcess *Process;

bool OnQuit(CWindow *win, CEvent *evt) {
    Logger(" OnQuit(%X, %X)\n", win, evt);
    if (!win || !evt) return false;

    bool status = true;
    if (win->CallBack) status = win->CallBack(win, evt);
    Process->Exit(0);
    return status;
}