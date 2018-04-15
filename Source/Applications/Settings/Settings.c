#include <Kernel.h>
#include <Shell.h>

import IThread   *Thread;
import IMemory   *Memory;
import IProcess  *Process;
import IWindow   *Window;
import IWidget   *Widget;
import IGraphics *Graphics;
import IBitmap   *Bitmap;
import ICanvas   *Canvas;
import IEvent    *Event;
import ITabBar   *TabBar;

static char *text[] = {
    "keyboard", "Apps/Settings/KeyBoard.bmp", "Apps/Settings/KeyBoard.exe", "", // 0
    "mouse",    "Apps/Settings/Mouse.bmp",    "Apps/Settings/Mouse.exe",    "", // 1
    "monitor",  "Apps/Settings/Monitor.bmp",  "Apps/Settings/Monitor.exe",  "", // 2
    "speaker",  "Apps/Settings/Speaker.bmp",  "Apps/Settings/Speaker.exe",  "", // 3
    "network",  "Apps/Settings/Serial.bmp",   "Apps/Settings/Network.exe",  "", // 4
    "ramdisk",  "Apps/Settings/RamDisk.bmp",  "Apps/Settings/RamDisk.exe",  "", // 5
    "cpu",      "Apps/Settings/Cpu.bmp",      "Apps/Settings/Cpu.exe",      ""  // 6
};
CBitmap icons[elementsof(text)/4];
CWindow *button[elementsof(text)/4];

bool OnLoad(CWindow *win, CEvent *evt) {
    CWindow *parent = win->Parent;
    if (parent) parent->Title = "Settings";
    CFont *font = win->Font;
    if (!font) return true;

    for (int i = 0; i < elementsof(text)/4; i++) {
        if (!Bitmap->LoadSprite(&icons[i], text[4*i + 1])) {
            return false;
        }

        int x = i / 4;
        int y = i % 4;
        int w = icons[i].ResX + 12*font->FontW;
        int h = icons[i].ResY;

        button[i] = Widget->Button(win);
        if (!button[i]) return false;
        button[i]->WindowID = 0x8000+i;
        button[i]->Bitmap = &icons[i];
        button[i]->Text   = text[4*i + 0];
        button[i]->PosX   = win->PosX + x*w;
        button[i]->PosY   = win->PosY + y*h;
        button[i]->Wide   = w;
        button[i]->High   = h;
    }
    return true;
}

bool OnMove(CWindow *win, CEvent *evt) {
    CEvtArea *area = (CEvtArea*)evt;
    win->PosX = area->PosX;
    win->PosY = area->PosY;
    return true;
}

bool OnSize(CWindow *win, CEvent *evt) {
    CEvtArea *area = (CEvtArea*)evt;
    win->Wide = area->Wide;
    win->High = area->High;
    return true;
}

bool OnDraw(CWindow *win, CEvent *evt) {
    if (!win->Visible) return true;

    CCanvas *canvas = win->Canvas;
    if (!canvas) return false;

    CArea client;
    client.X1 = win->PosX;
    client.Y1 = win->PosY;
    client.X2 = win->PosX + win->Wide;
    client.Y2 = win->PosY + win->High;

    Graphics->Poly4f(client.X1, client.Y1, client.X2, client.Y2, win->BackGround[0]);

    for (int i = 0; i < elementsof(text)/4; i++) {
        if (button[i]) {
            Window->Redraw(button[i]);
        }
    }
    Canvas->Blit(win->Canvas, client.X1, client.Y1, client.X2, client.Y2);
    return true;
}

bool OnAction(CWindow *win, CEvent *evt) {
    CWindow *child = evt->Source;
    if (!child) return true;

    int id = child->WindowID - 0x8000;
    if (id >= 0 && id < elementsof(text)/4) {
        char *path = text[id*4 + 2];
        Process->Create(path);
    }
    return true;
}

bool SettingsProc(CWindow *win, CEvent *evt) {
    switch (evt->Code) {
    case EVT_ONLOAD:   return OnLoad(win, evt);
    case EVT_ONMOVE:   return OnMove(win, evt);
    case EVT_ONSIZE:   return OnSize(win, evt);
    case EVT_ONDRAW:   return OnDraw(win, evt);
    case EVT_ONACTION: return OnAction(win, evt);
    }
    return true;
}

int main(int argc, char *argv[]) {
    CWindow *frame = Window->Create(nullptr, SettingsProc);
    if (!frame) return -1;
    for (CEvent evt;;) {
        if (Event->GetEvent(&evt)) {
            Event->HandleEvent(&evt);
        }
    }
    return 0;
}