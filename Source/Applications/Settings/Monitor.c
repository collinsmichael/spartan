#include <Kernel.h>
#include <Shell.h>

import IThread   *Thread;
import IMemory   *Memory;
import IProcess  *Process;
import IVesa     *Vesa;
import IGraphics *Graphics;
import IBitmap   *Bitmap;
import ICanvas   *Canvas;
import IEvent    *Event;
import IWindow   *Window;
import IWidget   *Widget;
import ITabBar   *TabBar;
import INoteBox  *NoteBox;

CBitmap icon;

CWindow *image;
CWindow *resolution;
CWindow *bitdepth;
CWindow *refresh;

char text[256];
int ResX;
int ResY;
int Bits;
int Refresh;

bool OnLoad(CWindow *win, CEvent *evt) {
    CWindow *parent = win->Parent;
    if (parent) {
        parent->Title = "Cpu";
    }
    win->BackGround[0] = 0x00E0E0E0;
    win->ForeGround[0] = 0x001E1E1E;

    ResX = Vesa->ResX();
    ResY = Vesa->ResY();
    Bits = Vesa->Bits();
    Refresh = 60;

    if (!Bitmap->LoadSprite(&icon, "Apps/Settings/Monitor.bmp")) {
        return false;
    }
    image      = Widget->Button(win);
    resolution = Widget->NoteBox(win);
    bitdepth   = Widget->NoteBox(win);
    refresh    = Widget->NoteBox(win);
    if (!image || !resolution || !bitdepth || !refresh) return false;
    image->Bitmap = &icon;
    NoteBox->SetReadOnly(resolution, true);
    NoteBox->SetReadOnly(bitdepth, true);
    NoteBox->SetReadOnly(refresh, true);
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

    ResX = Vesa->ResX();
    ResY = Vesa->ResY();
    Bits = Vesa->Bits();
    Refresh = 60;

    Format(&text[  0], "%d x %d", ResX, ResY);
    Format(&text[ 64], "%d bpp", Bits);
    Format(&text[128], "%d Hz", Refresh);
    NoteBox->SetText(resolution, &text[0], 64);
    NoteBox->SetText(bitdepth, &text[64], 64);
    NoteBox->SetText(refresh, &text[128], 64);

    CFont *font = win->Font;
    int tx = resolution->PosX;
    int ty = resolution->PosY - font->FontH;
    Window->TextOut(win, tx, ty, "%s", "resolution");
    tx = bitdepth->PosX;
    ty = bitdepth->PosY - font->FontH;
    Window->TextOut(win, tx, ty, "%s", "bit depth");
    tx = refresh->PosX;
    ty = refresh->PosY - font->FontH;
    Window->TextOut(win, tx, ty, "%s", "refresh rate");

    Window->Redraw(image);
    Window->Redraw(resolution);
    Window->Redraw(bitdepth);
    Window->Redraw(refresh);
    CCanvas *canvas = win->Canvas;
    Canvas->Blit(canvas, x1, y1, x2, y2);
    return true;
}

bool ShowHide(CWindow *child, bool show) {
    return true;
}

bool OnSize(CWindow *win, CEvent *evt) {
    static int pos[] = {
         32,  64,  48, 48, // image
        128,  64, 128, 32, // resolution
        128, 128, 128, 32, // bitdepth
        128, 192, 128, 32  // refresh
    };

    image->PosX      = pos[0*4 + 0] + win->PosX;
    image->PosY      = pos[0*4 + 1] + win->PosY;
    image->Wide      = pos[0*4 + 2];
    image->High      = pos[0*4 + 3];
    resolution->PosX = pos[1*4 + 0] + win->PosX;
    resolution->PosY = pos[1*4 + 1] + win->PosY;
    resolution->Wide = pos[1*4 + 2];
    resolution->High = pos[1*4 + 3];
    bitdepth->PosX   = pos[2*4 + 0] + win->PosX;
    bitdepth->PosY   = pos[2*4 + 1] + win->PosY;
    bitdepth->Wide   = pos[2*4 + 2];
    bitdepth->High   = pos[2*4 + 3];
    refresh->PosX    = pos[3*4 + 0] + win->PosX;
    refresh->PosY    = pos[3*4 + 1] + win->PosY;
    refresh->Wide    = pos[3*4 + 2];
    refresh->High    = pos[3*4 + 3];
    return true;
}

bool WndProc(CWindow *win, CEvent *evt) {
    switch (evt->Code) {
    case EVT_ONLOAD:   return OnLoad(win, evt);
    case EVT_ONDRAW:   return OnDraw(win, evt);
    case EVT_ONSIZE:   return OnSize(win, evt);
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