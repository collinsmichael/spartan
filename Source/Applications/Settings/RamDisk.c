#include <Kernel.h>
#include <Shell.h>

import IThread   *Thread;
import IMemory   *Memory;
import IProcess  *Process;
import IGraphics *Graphics;
import IBitmap   *Bitmap;
import ICanvas   *Canvas;
import IEvent    *Event;
import IWindow   *Window;
import IWidget   *Widget;
import ITabBar   *TabBar;

CBitmap icon;

CWindow *image;
CWindow *less;
CWindow *more;
CWindow *progress;

bool OnLoad(CWindow *win, CEvent *evt) {
    CWindow *parent = win->Parent;
    if (parent) {
        parent->Title = "Cpu";
    }
    win->BackGround[0] = 0x00E0E0E0;
    win->ForeGround[0] = 0x001E1E1E;

    if (!Bitmap->LoadSprite(&icon, "Apps/Settings/RamDisk.bmp")) {
        return false;
    }
    image    = Widget->Button(win);
    less     = Widget->Button(win);
    more     = Widget->Button(win);
    progress = Widget->Progress(win);
    if (!image || !less || !more || !progress) return false;
    less->WindowID = 0x1001;
    more->WindowID = 0x1002;
    progress->WindowID = 0x1003;

    if (image) image->Bitmap = &icon;
    if (less) less->Text = "<";
    if (more) more->Text = ">";
    if (progress) {
        progress->ForeGround[0] = 0x003FFF3F;
        progress->MinValue  = 0;
        progress->CurValue  = 128;
        progress->MaxValue  = 256;
        progress->StepValue = 8;
    }

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
    int tx = progress->PosX;
    int ty = progress->PosY - 3*font->FontH/2;
    Window->TextOut(win, tx, ty, "%s", "speed");

    //Event->Forward(less, evt);
    //Event->Forward(more, evt);
    //Event->Forward(progress, evt);
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

    if (progress) {
        switch (child->WindowID) {
        case 0x1001: // less
            progress->CurValue = MAX(progress->CurValue-progress->StepValue, progress->MinValue);
            break;
        case 0x1002: // more
            progress->CurValue = MIN(progress->CurValue+progress->StepValue, progress->MaxValue);
            break;
        }
    }
    Window->Redraw(image);
    Window->Redraw(less);
    Window->Redraw(more);
    Window->Redraw(progress);
    return true;
}

bool OnSize(CWindow *win, CEvent *evt) {
    static int pos[] = {
         16, 64,  48,  48, // image
         80, 64,  32,  32, // less
        112, 64, 128,  32, // progress
        240, 64,  32,  32  // more
    };

    image->PosX    = pos[0*4 + 0] + win->PosX;
    image->PosY    = pos[0*4 + 1] + win->PosY;
    image->Wide    = pos[0*4 + 2];
    image->High    = pos[0*4 + 3];
    less->PosX     = pos[1*4 + 0] + win->PosX;
    less->PosY     = pos[1*4 + 1] + win->PosY;
    less->Wide     = pos[1*4 + 2];
    less->High     = pos[1*4 + 3];
    progress->PosX = pos[2*4 + 0] + win->PosX;
    progress->PosY = pos[2*4 + 1] + win->PosY;
    progress->Wide = pos[2*4 + 2];
    progress->High = pos[2*4 + 3];
    more->PosX     = pos[3*4 + 0] + win->PosX;
    more->PosY     = pos[3*4 + 1] + win->PosY;
    more->Wide     = pos[3*4 + 2];
    more->High     = pos[3*4 + 3];
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