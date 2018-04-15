#include <Kernel.h>
#include <Shell.h>
#include <Desktop.h>

import IProcess *Process;
import ICanvas  *Canvas;
import IWindow  *Window;

static bool DeskOnLoad(CWindow *win, CEvent *evt) {
    CWindow *taskbar = Window->Create(win, TaskBarProc);
    if (!taskbar) return false;
    return true;
}

static bool DeskOnKeyDown(CWindow *win, CEvent *evt) {
    Window->SetTheme(win, "Grey");

    CEvtKey *key = (CEvtKey*)evt;
    if (key->Key == KEY_F12) {
        CCanvas *screen = Canvas->GetScreen();
        Canvas->Debug(screen, screen->Debug ^ true);
    } else if (key->Key == KEY_F1) {
        Process->Fork("Apps/Console.exe");
    } else if (key->Key == KEY_F2) {
        Process->Fork("Apps/Dir.exe");
    } else if (key->Key == KEY_F3) {
        Process->Fork("Apps/Gui.exe");
    } else if (key->Key == KEY_F4) {
        Process->Fork("Apps/Calc.exe");
    } else if (key->Key == KEY_F5) {
        Process->Fork("Apps/Blocker/Blocker.exe");
    } else if (key->Key == KEY_F6) {
        Process->Fork("Apps/Fps.exe");
    } else if (key->Key == KEY_F7) {
        Process->Fork("Apps/Plasma.exe");
    } else if (key->Key == KEY_F8) {
        Process->Fork("Apps/CiP.exe");
    }
    return true;
}

static bool DeskOnDraw(CWindow *win, CEvent *evt) {
    CCanvas *canvas = win->Canvas;
    Logger(" DeskOnDraw(canvas.Draw = %X)\n", canvas->Draw);

    win->PosX = win->NewX = win->OldX = 0;
    win->PosY = win->NewY = win->OldY = 0;
    win->Wide = win->NewW = canvas->ResX;
    win->High = win->NewH = canvas->ResY;

    int x2 = win->Wide - 1;
    int y2 = win->High - 48 - 1;
    for (int y = 0; y < y2; y++) {
        int *image = (int*)&canvas->Draw[y*canvas->ScanLine];
        for (int x = 0; x < win->Wide; x++) {
            u8 r = 112 + (x2-x)*128/win->Wide;
            u8 g =  16 + (x+y)*16/(win->Wide+win->High);
            u8 b = 112 + (y2-y)*128/win->High;
            image[x] = (r<<16) | (g<<8) | (b);
        }
    }
    return true;
}

bool DeskProc(CWindow *win, CEvent *evt) {
    switch (evt->Code) {
    case EVT_ONLOAD:    return DeskOnLoad(win, evt);
    case EVT_ONDRAW:    return DeskOnDraw(win, evt);
    case EVT_ONKEYDOWN: return DeskOnKeyDown(win, evt);
    }
    return true;
}