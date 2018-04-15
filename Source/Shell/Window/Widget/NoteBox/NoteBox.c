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


bool NoteBoxCaret(CWindow *win, CEvent *evt);

static bool NoteBoxOnLoad(CWindow *win, CEvent *evt) {
    if (!Window->SetFont(win, "Media/Fonts/terminal.bmp")) return false;

    win->BackGround[0] = 0x00E1E1E1;
    win->BackGround[1] = 0x00C4E8FD;
    win->BackGround[2] = 0x000078D7;
    win->ForeGround[0] = 0x001F1F1F;
    win->ForeGround[1] = 0x001F1F1F;
    win->ForeGround[2] = 0x00F1F1F1;
    win->Lut = 0;
    win->CustomData = win->Extra;

    CNoteBox *data = (CNoteBox*)win->Extra;
    data->Text     = Memory->Alloc(256);
    data->Size     = 255;
    data->Format   = "%s";
    data->Caret    = 0;
    data->ReadOnly = false;
    data->Numeric  = false;
    data->Password = false;

    win->MinValue = -100;
    win->CurValue =    0;
    win->MaxValue =  100;
    return true;
}

static bool NoteBoxOnSize(CWindow *win, CEvent *evt) {
    CEvtArea *area = (CEvtArea*)evt;
    win->PosX = area->PosX;
    win->PosY = area->PosY;
    win->Wide = area->Wide;
    win->High = area->High;
    return true;
}

bool NoteBoxDrawText(CWindow *win) {
    if (!win) return false;
    CCanvas *canvas = win->Canvas;
    CFont *font = win->Font;
    CNoteBox *data = (CNoteBox*)win->CustomData;
    if (!canvas || !font || !data) return false;

    CArea area;
    int x1 = area.X1 = win->PosX;
    int y1 = area.Y1 = win->PosY;
    int x2 = area.X2 = win->PosX + win->Wide;
    int y2 = area.Y2 = win->PosY + win->High;
    int fontw = MAX(font->FontW, 1);
    int fonth = MAX(font->FontH, 1);

    int bg  = win->BackGround[0];
    int fg  = win->ForeGround[0];
    int lut = win->Lut;
    int border = win->Clut[lut*8 + COLOUR_BORDER];

    Graphics->Poly4Clip (&area, x1,   y1,   x2,   y2, border);
    Graphics->Poly4fClip(&area, x1+1, y1+1, x2-1, y2-1, bg);

    int u1 = x1 + fontw/2;
    int v1 = y1 + (y2-y1)/2 - fonth/2;
    int v2 = y1 + (y2-y1)/2 + fonth/2;

    char *line = data->Text;
    for (int i = 0; i < data->Size; i++) {
        char c = line[i];
        if (!c || c == '\n') break;
        int x = u1 + (i)*fontw;
        int y = v1;
        if (data->Password) c = '*';
        Font->PutCharClip(&area, font, canvas, x, y, fg, c);
    }

    if (canvas->Focus == win) {
        int cx = u1 + (data->Caret)*fontw;
        int sbg = win->BackGround[2];
        int sfg = win->ForeGround[2];
        Graphics->Poly4fClip(&area, cx, v1, cx+fontw, v2, sbg);
        char c = line[data->Caret];
        if (c > ' ') {
            if (data->Password) c = '*';
            Font->PutCharClip(&area, font, canvas, cx, v1, sfg, c);
        }
    }
    Canvas->Blit(canvas, x1, y1, x2, y2);
    return true;
}

static bool NoteBoxOnDraw(CWindow *win, CEvent *evt) {
    if (!win->Visible) return true;
    NoteBoxDrawText(win);
    return true;
}

static bool NoteBoxOnKeyDown(CWindow *win, CEvent *evt) {
    NoteBoxCaret(win, evt);
    NoteBoxDrawText(win);
    return true;
}

static bool NoteBoxOnClick(CWindow *win, CEvent *evt) {
    CEvtMouse *mouse = (CEvtMouse*)evt;
    int x = mouse->NewMouse.PosX;
    int y = mouse->NewMouse.PosY;

    int x1 = win->PosX;
    int y1 = win->PosY;
    int x2 = win->PosX + win->Wide;
    int y2 = win->PosY + win->High;
    if (x < x1 || y < y1) return true;
    if (x > x2 || y > y2) return true;

    CNoteBox *data = (CNoteBox*)win->CustomData;
    CFont *font = win->Font;
    if (!data || !font) return false;

    int length = StrLen(data->Text);
    int fontw = MAX(font->FontW, 1);
    int fonth = MAX(font->FontH, 1);
    data->Caret = (x - x1 - fontw/2) / fontw;
    data->Caret = MIN(MAX(0, data->Caret), length);
    data->Caret = MIN(MAX(0, data->Caret), data->Size);
    NoteBoxDrawText(win);
    return true;
}

bool NoteBoxProc(CWindow *win, CEvent *evt) {
    switch (evt->Code) {
    case EVT_ONLOAD:    return NoteBoxOnLoad(win, evt);
    case EVT_ONSIZE:    return NoteBoxOnSize(win, evt);
    case EVT_ONDRAW:    return NoteBoxOnDraw(win, evt);
    case EVT_ONKEYDOWN: return NoteBoxOnKeyDown(win, evt);
    case EVT_ONCLICK:   return NoteBoxOnClick(win, evt);
    case EVT_ONDRAG:    return NoteBoxOnClick(win, evt);
    case EVT_ONACTION:  return NoteBoxOnDraw(win, evt);
    }
    return true;
}