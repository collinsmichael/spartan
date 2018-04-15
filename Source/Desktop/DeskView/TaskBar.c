#include <Kernel.h>
#include <Shell.h>
#include <Desktop.h>

import IProcess *Process;
import ICanvas  *Canvas;
import IClock   *Clock;
import IBitmap  *Bitmap;
import IFont    *Font;
import IWindow  *Window;
import IWidget  *Widget;

char *buttons[] = {
    0, 0,
    "Media/Icons/dir.bmp",      "Apps/Dir.exe",
    "Media/Icons/sh.bmp",       "Apps/Console.exe",
    "Media/Icons/cfg.bmp",      "Apps/Settings/Settings.exe",
    "Apps/Txt/Txt.bmp",         "Apps/Txt/Txt.exe",
    "Apps/Chat/Chat.bmp",       "Apps/Chat/Chat.exe",
    "Apps/Calc/Calc.bmp",       "Apps/Calc/Calc.exe",
    "Apps/Blocker/Blocker.bmp", "Apps/Blocker/Blocker.exe",
};
CBitmap  bitmap[elementsof(buttons)/2 + 1];

bool TaskBarOnLoad(CWindow *taskbar, CEvent *evt) {
    Window->SetTheme(taskbar, "Grey");
    CCanvas *canvas = Canvas->GetCanvas();
    taskbar->PosX = taskbar->NewX = taskbar->OldX = 0;
    taskbar->PosY = taskbar->NewY = taskbar->OldY = canvas->ResY - 48;
    taskbar->Wide = canvas->ResX;
    taskbar->High = 48;

    for (int i = 1; i < elementsof(buttons)/2; i++) {
        if (!Bitmap->LoadSprite(&bitmap[i], buttons[i*2+0])) {
            return false;
        }

        CWindow *child = Widget->Button(taskbar);
        if (!child) return false;
        Window->SetTheme(child, "Grey");
        child->Wide = 48;
        child->High = 48;
        child->PosX = taskbar->PosX + (i-1)*48;
        child->PosY = taskbar->PosY;
        child->NewX = child->OldX = child->PosX;
        child->NewY = child->OldY = child->PosY;
        child->WindowID = i;
        child->Bitmap = &bitmap[i];
    }
    return true;
}

bool TaskBarOnKeyDown(CWindow *win, CEvent *evt) {
    CEvtKey *key = (CEvtKey*)evt;
    if (key->Key == KEY_ENTER || key->Key == KEY_SPACE) {
    }
    return true;
}

bool TaskBarOnClick(CWindow *win, CEvent *evt) {
    CEvtMouse *mouse = (CEvtMouse*)evt;
    return true;
}

bool TaskBarOnAction(CWindow *win, CEvent *evt) {
    CWindow *child = evt->Source;
    if (!child) return false;
    int id = child->WindowID;

    char *cmdline = buttons[id*2 + 1];
    Debug(" Process->Create(%d %s)\n", id, cmdline);
    CPcb *pcb = Process->Create(cmdline);
    return true;
}

bool PutDateTime(CWindow *win) {
/*
    CFont *font = win->Font;
    if (!font) return false;

    int w  = font->FontW;
    int h  = font->FontH;
    int x  = win->PosX + win->Wide - 12*w;
    int y1 = win->PosY + win->High - h*1/2;
    int y2 = win->PosY + win->High - h*3/2;

    u8 now[8];
    Clock->Now(now);
    
    char time[64];
    Format(&time[0], (now[3] < 10) ? "0%d:" : "%d:", now[3]);
    Format(&time[3], (now[2] < 10) ? "0%d:" : "%d:", now[2]);
    Format(&time[6], (now[1] < 10) ? "0%d"  : "%d",  now[1]);

    char date[64];
    Format(&date[0], (now[4] < 10) ? "0%d/" : "%d/", now[4]);
    Format(&date[3], (now[5] < 10) ? "0%d/" : "%d/", now[5]);
    Format(&date[6], (now[7] < 10) ? "0%d"  : "%d",  now[7]);
    Format(&date[8], (now[6] < 10) ? "0%d"  : "%d",  now[6]);

    Window->TextOut(win, x, y1, "%s", time);
    Window->TextOut(win, x, y2, "%s", date);
*/
    return true;
}

bool TaskBarOnDraw(CWindow *win, CEvent *evt) {
    if (!win->Visible) return true;
    CCanvas *canvas = win->Canvas;
    int v2 = win->High;
    for (int v = 0; v < v2; v++) {
        int y = v + win->PosY;
        char *image = &canvas->Draw[y*canvas->ScanLine];

        u8 r = 48 + (v2-v)*32/win->High;
        u32 c = r*0x010101;
        stosd(image, c, win->Wide);
    }
    PutDateTime(win);
    return true;
}

bool TaskBarProc(CWindow *win, CEvent *evt) {
    switch (evt->Code) {
    case EVT_ONLOAD:    return TaskBarOnLoad(win, evt);
    case EVT_ONDRAW:    return TaskBarOnDraw(win, evt);
    case EVT_ONKEYDOWN: return TaskBarOnKeyDown(win, evt);
    case EVT_ONCLICK:   return TaskBarOnClick(win, evt);
    case EVT_ONACTION:  return TaskBarOnAction(win, evt);
    }
    return true;
}