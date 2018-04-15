#include <Kernel.h>
#include <Shell.h>

import IThread   *Thread;
import IMemory   *Memory;
import IProcess  *Process;

import IWindow   *Window;
import IWidget   *Widget;
import IGraphics *Graphics;
import ICanvas   *Canvas;
import IEvent    *Event;

bool OnLoad(CWindow *win, CEvent *evt) {
    CWindow *frame = win->Parent;
    frame->Title = "Calc";

    win->ForeGround[0] = 0x001E1E1E;
    win->BackGround[0] = 0x009B9B9B;
    int w = (win->Wide-64) / 4;
    int h = (win->High-64) / 6;

    CWindow *textbox = Widget->Console(win);
    if (!textbox) return false;
    textbox->PosX = win->PosX + 8;
    textbox->PosY = win->PosY + 8;
    textbox->Wide = win->Wide - 16;
    textbox->High = h;
    textbox->Text = Memory->Alloc(64*KB);
    textbox->TextLen = (textbox->Text) ? 64*KB : 0;

    for (int y = 0; y < 4; y++) {
        for (int x = 0; x < 4; x++) {
            CWindow *button = Widget->Button(win);
            if (!button) return false;

            static char ascii[][4] = {
                "7", "8", "9", "+",
                "4", "5", "6", "-",
                "1", "2", "3", "*",
                ".", "0", "=", "/"
            };
            button->WindowID = 0x100 + y*4 + x;
            button->Text = ascii[y*4+x];

            int v = y + 1;
            button->PosX = win->PosX + 8 + x*16 + x*w;
            button->PosY = win->PosY + 8 + v*16 + v*h;
            button->Wide = w;
            button->High = h;
            button->ForeGround[0] = 0x001E1E1E;
        }
    }
    return true;
}

bool OnDraw(CWindow *win, CEvent *evt) {
    if (!win->Visible) return true;
    CCanvas *canvas = win->Canvas;
    int x1 = win->PosX;
    int y1 = win->PosY;
    int x2 = win->PosX + win->Wide;
    int y2 = win->PosY + win->High;
    int c  = win->BackGround[0];
    Graphics->Poly4f(x1, y1, x2, y2, c);

    for (CWindow *list = win->Tail; list; list = list->Prev) {
        list->CallBack(list, evt);
    }
    //Canvas->Blit(win->Canvas, x1, y1, x2, y2);
    return true;
}

bool OnAction(CWindow *win, CEvent *evt) {
    CWindow *source = evt->Source;
    if (!source) return false;

    CWindow *console = win->Tail;
    if (!console) return false;

    char *text = source->Text;
    if (text) {
        Window->PutC(console, text[0]);

        int x1 = console->PosX;
        int y1 = console->PosY;
        int x2 = console->Wide + x1;
        int y2 = console->High + y1;
        Canvas->Blit(win->Canvas, x1, y1, x2, y2);
    }
    return true;
}

bool CalcProc(CWindow *win, CEvent *evt) {
    switch (evt->Code) {
    case EVT_ONLOAD:   return OnLoad(win, evt);
    case EVT_ONDRAW:   return OnDraw(win, evt);
    case EVT_ONACTION: return OnAction(win, evt);
    }
    return true;
}

int main(void) {
    CWindow *frame = Window->Create(nullptr, CalcProc);
    if (!frame) return -1;
    for (CEvent evt;;) {
        if (Event->GetEvent(&evt)) {
            Event->HandleEvent(&evt);
        }
    }
    return 0;
}

void start(void) {
    Thread->Exit(main());
}