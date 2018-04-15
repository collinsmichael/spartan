#include <Kernel.h>
#include <Shell.h>
#include <Desktop.h>

import IThread *Thread;
import ICanvas *Canvas;
import IWindow *Window;
import IEvent  *Event;

extern ICompositor *Compositor;
extern ICaret      *Caret;

bool IThread_Killer(void *a, void *b, void *c);

CCanvas *screen;
CWindow *desktop;
CWindow *cursor;
CWindow *taskbar;

bool IIdleThead_Run(void) {
    CEvent evt;

    screen = Canvas->GetScreen();
    if (!screen) return 0;

    desktop = Window->CreateFrame(DeskProc);
    if (!desktop) return 0;
    screen->BtmMost = desktop;
    screen->Focus = desktop;

    cursor = Window->CreateFrame(CursorProc);
    if (!cursor) return false;
    screen->TopMost = cursor;

    CaretInstall();
    Compositor->Install();

    IThread_Killer(Canvas, screen, cursor);
    for (;;) {
        Thread->Yield();
        if (Event->PeekEvent(&evt)) {
            Event->HandleEvent(&evt);
        }
        IThread_Killer(Canvas, screen, cursor);
    }
    return true;
}

static IIdleThread idlethread = {
    IIdleThead_Run
};

IIdleThread *IdleThread = &idlethread;