#include <Kernel.h>
#include <Shell.h>

import IMemory   *Memory;
extern IGraphics *Graphics;
extern IFont     *Font;
extern ICanvas   *Canvas;
extern ICaret    *Caret;
extern IEvent    *Event;
extern IWindow   *Window;
extern IWidget   *Widget;
extern IScroll   *Scroll;

bool TextBoxDrawText(CWindow *win) {
    if (!win) return false;

    CCanvas  *canvas = win->Canvas;
    CFont    *font = win->Font;
    CTextBox *data = (CTextBox*)win->CustomData;
    if (!canvas || !font || !data) return false;

    CWindow  *hscroll = win->Head;
    CScroll *hdata = (CScroll*)hscroll->CustomData;
    if (!hscroll || !hdata) return false;

    CWindow  *vscroll = win->Tail;
    CScroll *vdata = (CScroll*)vscroll->CustomData;
    if (!vscroll || !vdata) return false;

    char *text = data->Text;
    if (!text) return true;

    CArea area;
    int x1 = area.X1 = win->PosX;
    int y1 = area.Y1 = win->PosY;
    int x2 = area.X2 = win->PosX + win->Wide - 24;
    int y2 = area.Y2 = win->PosY + win->High - 24;
    int bg  = win->BackGround[0];
    int fg  = win->ForeGround[0];

    int fontw = MAX(font->FontW, 1);
    int fonth = MAX(font->FontH, 1);
    int pagex = (x2 - x1 + fontw - 1) / fontw;
    int pagey = (y2 - y1 + fonth - 1) / fonth;
    pagex = MIN(MAX(0, pagex), data->NumCols);
    pagey = MIN(MAX(0, pagey), data->NumRows);

    int col = hdata->TrackPos*data->NumCols/hdata->TrackMax;
    int row = vdata->TrackPos*data->NumRows/vdata->TrackMax;
    int u1 = x1 - col*fontw;
    int v1 = y1 - row*fonth;

    int cx1 = u1 + win->CaretX*fontw;
    int cy1 = v1 + win->CaretY*fonth;
    int cx2 = cx1 + fontw;
    int cy2 = cy1 + fonth;
    int cbg = win->BackGround[1];
    int cfg = win->ForeGround[1];
    int sbg = win->BackGround[2];
    int sfg = win->ForeGround[2];
    Graphics->Poly4fClip(&area, x1, y1, x2, y2, bg);
    Graphics->Poly4f(x2, y2, x2+24, y2+24, bg);
    Graphics->Poly4fClip(&area, x1,  cy1, x2,  cy2, cbg);
    Graphics->Poly4fClip(&area, cx1, cy1, cx2, cy2, sbg);

    for (int v = -1; v < pagey+1; v++) {
        if (v + row < 0) continue;
        if (v + row > data->NumRows) break;

        char *line = &data->Text[(v+row)*data->MaxCols];
        for (int u = -1; u < pagex+1; u++) {
            if (u + col < 0) continue;
            if (u + col > data->NumCols) break;

            char c = line[col+u];
            if (!c || c == '\n') break;
            int x = u1 + (u+col)*fontw;
            int y = v1 + (v+row)*fonth;
            Font->PutCharClip(&area, font, canvas, x, y, fg, c);
        }
    }

    char c = data->Text[win->CaretY*data->MaxCols + win->CaretX];
    if (c) {
        Font->PutCharClip(&area, font, canvas, cx1, cy1, sfg, c);
    }
    Canvas->Blit(win->Canvas, x1, y1, x2, y2);
    return true;
}