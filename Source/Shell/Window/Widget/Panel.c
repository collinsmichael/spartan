#include <Kernel.h>
#include <Shell.h>

extern IGraphics *Graphics;
extern IBitmap   *Bitmap;
extern IWindow   *Window;
extern IEvent    *Event;
extern ICanvas   *Canvas;

static bool PanelOnLoad(CWindow *win, CEvent *evt) {
    Logger(" [info] Panel.OnLoad(%X)\n", win);
    win->BackGround[0] = 0x00E1E1E1;
    win->ForeGround[0] = 0x001F1F1F;
    return true;
}

static bool PanelOnDraw(CWindow *win, CEvent *evt) {
    if (!win->Visible) return true;
    static int poly4f[] = {
        0,  0, 0, 0, 3
    };
    for (int i = 0; i < elementsof(poly4f); i += 5) {
        int x1 = win->PosX;
        int y1 = win->PosY;
        int x2 = win->PosX + win->Wide;
        int y2 = win->PosY + win->High;
        int c  = win->BackGround[0];
        Graphics->Poly4f(x1, y1, x2, y2, c);
    }

    if (win->Bitmap) {
        int x1 = win->PosX;
        int y1 = win->PosY;
        int x2 = win->PosX + win->Bitmap->ResX;
        int y2 = win->PosY + win->Bitmap->ResY;

        CArea area;
        area.X1 = win->PosX;
        area.Y1 = win->PosY;
        area.X2 = win->PosX + win->Wide;
        area.Y2 = win->PosY + win->High;
        Graphics->Poly4tClip(&area, x1, y1, x2, y2, win->Bitmap);
    }

    if (win->Text) {
        int cx = win->PosX + win->Wide/2;
        int cy = win->PosY + win->High/2;

        int n = StrLen(win->Text);
        int w = n*win->Font->FontW;
        int h = n*win->Font->FontH;

        int x1 = cx - w/2;
        int y1 = cy - h/2;
        int x2 = cx - w/2;
        int y2 = cy - h/2;

        if (win->Bitmap) {
            x1 += win->Bitmap->ResX;
            x2 += win->Bitmap->ResX;
        }
        Window->TextOut(win, x1, y1, "%s", win->Text);
    }
    CCanvas *canvas = win->Canvas;
    int x1 = win->PosX;
    int y1 = win->PosY;
    int x2 = win->PosX + win->Wide;
    int y2 = win->PosY + win->High;
    Canvas->Blit(canvas, x1, y1, x2, y2);
    return true;
}

bool PanelProc(CWindow *win, CEvent *evt) {
    switch (evt->Code) {
    case EVT_ONLOAD:  return PanelOnLoad(win, evt);
    case EVT_ONDRAW:  return PanelOnDraw(win, evt);
    case EVT_ONFOCUS: return false;
    }
    return true;
}