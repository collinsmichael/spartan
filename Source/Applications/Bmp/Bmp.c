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

static char *path;
static CBitmap bitmap;

bool OnLoad(CWindow *win, CEvent *evt) {
    CCanvas *canvas = win->Canvas;
    if (!canvas) return false;
    CCanvas *screen = Canvas->GetScreen();
    if (!screen) return false;
    CWindow *frame = canvas->BtmMost;
    if (!frame) return false;
    frame->Title = path;

    int w = bitmap.ResX + 16 + 4;
    int h = bitmap.ResY + 40 + 4;
    int x = (canvas->ResX)/2 - w/2;
    int y = (canvas->ResY)/2 - h/2;
    Debug(" Resize (%d %d %d %d %s)\n", x, y, w, h, frame->Title);
    Window->Resize(frame, x, y, w, h);

    win->PosX =  8;
    win->PosY = 32;
    win->Wide = w - 16;
    win->High = h - 40;
    win->ForeGround[0] = 0x009B9B9B;
    win->BackGround[0] = 0x001E1E1E;
    win->BackGround[1] = 0x005E5E5E;
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

    int cx = win->PosX + (win->Wide)/2;
    int cy = win->PosY + (win->High)/2;

    CArea image;
    image.X1 = cx - bitmap.ResX/2;
    image.Y1 = cy - bitmap.ResY/2;
    image.X2 = cx + bitmap.ResX/2;
    image.Y2 = cy + bitmap.ResY/2;

    CArea client;
    client.X1 = win->PosX;
    client.Y1 = win->PosY;
    client.X2 = win->PosX + win->Wide;
    client.Y2 = win->PosY + win->High;

    Graphics->Poly4f(client.X1, client.Y1, client.X2, client.Y2, win->BackGround[0]);
    Graphics->Poly4tClip(&client, image.X1, image.Y1, image.X2, image.Y2, &bitmap);

    Canvas->Blit(win->Canvas, client.X1, client.Y1, client.X2, client.Y2);
    //Canvas->Flip(win->Canvas);
    return true;
}

bool BmpProc(CWindow *win, CEvent *evt) {
    switch (evt->Code) {
    case EVT_ONLOAD:   return OnLoad(win, evt);
    case EVT_ONMOVE:   return OnMove(win, evt);
    case EVT_ONSIZE:   return OnSize(win, evt);
    case EVT_ONDRAW:   return OnDraw(win, evt);
    }
    return true;
}

int main(int argc, char *argv[]) {
    path = (argc > 1) ? argv[1] : "Users/Mike/Image.bmp";
    if (path) {
        if (!Bitmap->LoadSprite(&bitmap, path)) {
            return 0;
        }
    } else {
        if (!Bitmap->CreateSprite(&bitmap, 320, 240, 32)) {
            return 0;
        }
    }

    CWindow *frame = Window->Create(nullptr, BmpProc);
    if (!frame) return -1;
    for (CEvent evt;;) {
        if (Event->GetEvent(&evt)) {
            Event->HandleEvent(&evt);
        }
    }
    return 0;
}