#include <Kernel.h>
#include <Shell.h>

import IThread   *Thread;
import IMemory   *Memory;
import IProcess  *Process;
import IKeyboard *Keyboard;

import IGraphics *Graphics;
import IBitmap   *Bitmap;
import ICanvas   *Canvas;
import IEvent    *Event;
import IWindow   *Window;
import IWidget   *Widget;
import ITabBar   *TabBar;

CBitmap icon;
CWindow *image;
CWindow *textbox;

bool OnLoad(CWindow *win, CEvent *evt) {
    CWindow *parent = win->Parent;
    if (parent) {
        parent->Title = "Cpu";
    }
    win->BackGround[0] = 0x00E0E0E0;
    win->ForeGround[0] = 0x001E1E1E;

    if (!Bitmap->LoadSprite(&icon, "Apps/Settings/KeyBoard.bmp")) {
        return false;
    }
    image    = Widget->Button(win);
    if (!image) return false;
    image->WindowID = 0x1001;
    image->Text = Keyboard->GetKeyMap();

    textbox = Widget->NoteBox(win);

    return true;
}

bool OnDraw(CWindow *win, CEvent *evt) {
    CArea area;
    int x1 = area.X1 = win->PosX;
    int y1 = area.Y1 = win->PosY;
    int x2 = area.X2 = win->Wide + x1;
    int y2 = area.Y2 = win->High + y1;
    int rgba = win->BackGround[0];
    Graphics->Poly4fClip(&area, x1, y1, x2, y2, rgba);

    CFont *font = win->Font;
    int tx = textbox->PosX;
    int ty = textbox->PosY - 3*font->FontH/2;
    Window->TextOut(win, tx, ty, "%s", "test textbox");

    //Event->Forward(less, evt);
    //Event->Forward(more, evt);
    //Event->Forward(progress, evt);


    Window->Redraw(image);
    Window->Redraw(textbox);
    CCanvas *canvas = win->Canvas;
    Canvas->Blit(canvas, x1, y1, x2, y2);
    return true;
}

bool ShowHide(CWindow *child, bool show) {
    return true;
}

bool OnAction(CWindow *win, CEvent *evt) {

    CWindow *child = evt->Source;
    if (!child) return false;

    if (child->WindowID == image->WindowID) {
        char *layout = Keyboard->GetKeyMap();
        if      (StrCmp(layout, "Dvorak") == 0) layout = "Qwerty";
        else if (StrCmp(layout, "Qwerty") == 0) layout = "Dvorak";
        Keyboard->SetKeyMap(layout);
        layout = Keyboard->GetKeyMap();
        image->Text = layout;
    }
    Window->Redraw(image);
    Window->Redraw(textbox);
    return true;
}

bool OnSize(CWindow *win, CEvent *evt) {
    static int pos[] = {
        48,  32, 320, 48, // image
        48, 128, 320, 32  // textbox
    };

    image->PosX    = pos[0*4 + 0] + win->PosX;
    image->PosY    = pos[0*4 + 1] + win->PosY;
    image->Wide    = pos[0*4 + 2];
    image->High    = pos[0*4 + 3];

    textbox->PosX   = pos[1*4 + 0] + win->PosX;
    textbox->PosY   = pos[1*4 + 1] + win->PosY;
    textbox->Wide   = pos[1*4 + 2];
    textbox->High   = pos[1*4 + 3];
    return true;
}

bool WndProc(CWindow *win, CEvent *evt) {
    switch (evt->Code) {
    case EVT_ONLOAD:   return OnLoad(win, evt);
    case EVT_ONDRAW:   return OnDraw(win, evt);
    case EVT_ONSIZE:   return OnSize(win, evt);
    case EVT_ONACTION: return OnAction(win, evt);
    }
    return true;
}

int main(void) {
    CWindow *frame = Window->Create(nullptr, WndProc);
    if (!frame) return -1;
    for (CEvent evt;;) {
        if (Event->GetEvent(&evt)) {
            Event->HandleEvent(&evt);
        }
    }
    return 0;
}