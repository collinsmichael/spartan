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

bool TextBoxCaret(CWindow *win, CEvent *evt);
bool TextBoxDrawText(CWindow *win);

static bool TextBoxOnLoad(CWindow *win, CEvent *evt) {
    static int clut[32] = {
        0x005B5B5B,0x00717171,0x00818181,0x009B9B9B, 0x00000000,0x00000000,0x00000000,0x00000000, // normal
        0x00656565,0x008B8B8B,0x009B9B9B,0x00B5B5B5, 0x00000000,0x00000000,0x00000000,0x00000000, // onenter
        0x00515151,0x00656565,0x00717171,0x00919191, 0x00000000,0x00000000,0x00000000,0x00000000, // onclick
        0x00512121,0x00713131,0x00813131,0x009B3B3B, 0x00000000,0x00000000,0x00000000,0x00000000 // onhanging
    };

    if (!Window->SetFont(win, "Media/Fonts/courier.bmp")) return false;
    movsd(win->Clut, clut, elementsof(clut));
    win->BackGround[0] = 0x00E1E1E1;
    win->BackGround[1] = 0x00C4E8FD;
    win->BackGround[2] = 0x000078D7;
    win->ForeGround[0] = 0x001F1F1F;
    win->ForeGround[1] = 0x001F1F1F;
    win->ForeGround[2] = 0x00F1F1F1;
    win->Lut = 0;
    win->CustomData = win->Extra;

    CTextBox *data = (CTextBox*)win->Extra;
    data->NumCols = 0;
    data->NumRows = 0;
    data->MaxCols = 256;
    data->MaxRows = 1024;
    data->Text = Memory->Alloc(data->MaxCols*data->MaxRows);
    if (!data->Text) return false;
    win->Text = data->Text;

    CWindow *vscroll = Widget->Scroll(win, WIN_ORIENT_VERT);
    if (!vscroll) return false;
    vscroll->WindowID = 0x1001;
    Scroll->SetUnitCount(vscroll, 0);
    Scroll->SetUnitSize(vscroll, win->Font->FontH);

    CWindow *hscroll = Widget->Scroll(win, WIN_ORIENT_HORZ);
    if (!hscroll) return false;
    hscroll->WindowID = 0x1002;
    Scroll->SetUnitCount(hscroll, 0);
    Scroll->SetUnitSize(hscroll, win->Font->FontW);

    CWindow *parent = win->Parent;
    if (!parent) return false;
    win->PosX = parent->PosX;
    win->PosY = parent->PosY;
    win->Wide = parent->Wide;
    win->High = parent->High;
    return true;
}

static bool TextBoxOnSize(CWindow *win, CEvent *evt) {
    CEvtArea *area = (CEvtArea*)evt;
    win->PosX = area->PosX;
    win->PosY = area->PosY;
    win->Wide = area->Wide;
    win->High = area->High;

    int x1 = area->PosX;
    int y1 = area->PosY;
    int x2 = area->PosX + area->Wide - 24;
    int y2 = area->PosY + area->High - 24;
    int x3 = area->PosX + area->Wide;
    int y3 = area->PosY + area->High;

    int x = area->PosX + area->Wide - 24;
    int y = area->PosY + area->High - 24;
    CWindow *vscroll = win->Tail;
    CWindow *hscroll = win->Head;
    Scroll->Resize(hscroll, x1, y2, x2, y3);
    Scroll->Resize(vscroll, x2, y1, x3, y2);
    return true;
}

static bool TextBoxOnDraw(CWindow *win, CEvent *evt) {
    if (!win->Visible) return true;
    TextBoxDrawText(win);
    return true;
}

static bool TextBoxOnKeyDown(CWindow *win, CEvent *evt) {
    TextBoxCaret(win, evt);
    TextBoxDrawText(win);
    return true;
}

static bool TextBoxOnClick(CWindow *win, CEvent *evt) {
    CEvtMouse *mouse = (CEvtMouse*)evt;
    int x = mouse->NewMouse.PosX;
    int y = mouse->NewMouse.PosY;

    int x1 = win->PosX;
    int y1 = win->PosY;
    int x2 = win->PosX + win->Wide - 24;
    int y2 = win->PosY + win->High - 24;
    if (x < x1 || y < y1) return true;
    if (x > x2 || y > y2) return true;

    CWindow  *hscroll = win->Head;
    CWindow  *vscroll = win->Tail;
    if (!hscroll || !vscroll) return false;

    CTextBox *data = (CTextBox*)win->CustomData;
    CScroll  *hdata = (CScroll*)hscroll->CustomData;
    CScroll  *vdata = (CScroll*)vscroll->CustomData;
    CFont    *font = win->Font;
    if (!data || !font || !hdata || !vdata) return false;

    int fontw = MAX(font->FontW, 1);
    int fonth = MAX(font->FontH, 1);
    int col = hdata->TrackPos*data->NumCols/hdata->TrackMax;
    int row = vdata->TrackPos*data->NumRows/vdata->TrackMax;

    win->CaretX = col + (x-x1)/fontw;
    win->CaretY = row + (y-y1)/fonth;

    TextBoxDrawText(win);
    return true;
}

static bool TextBoxOnWheel(CWindow *win, CEvent *evt) {
    CEvtMouse *mouse = (CEvtMouse*)evt;
    int x = mouse->NewMouse.PosX;
    int y = mouse->NewMouse.PosY;
    if (x < win->PosX || y < win->PosY) return true;

    CWindow  *hscroll = win->Head;
    if (y > win->PosY + win->High - 24) {
        return Event->Forward(hscroll, evt);
    } else {
        CWindow  *vscroll = win->Tail;
        return Event->Forward(vscroll, evt);
    }
    return true;
}

bool TextBoxProc(CWindow *win, CEvent *evt) {
    switch (evt->Code) {
    case EVT_ONLOAD:    return TextBoxOnLoad(win, evt);
    case EVT_ONSIZE:    return TextBoxOnSize(win, evt);
    case EVT_ONDRAW:    return TextBoxOnDraw(win, evt);
    case EVT_ONKEYDOWN: return TextBoxOnKeyDown(win, evt);
    case EVT_ONCLICK:   return TextBoxOnClick(win, evt);
    case EVT_ONDRAG:    return TextBoxOnClick(win, evt);
    case EVT_ONWHEEL:   return TextBoxOnWheel(win, evt);
    case EVT_ONACTION:  return TextBoxOnDraw(win, evt);
    }
    return true;
}

bool ITextBox_GetText(CWindow *textbox, char *text, int size) {
    if (!textbox) return false;
    CTextBox *data = (CTextBox*)textbox->CustomData;
    if (!data) return false;
    //data->Text = text;
    //data->Size = size;
    return true;
}

bool ITextBox_SetText(CWindow *textbox, char *text, int size) {
    if (!textbox) return false;
    CTextBox *data = (CTextBox*)textbox->CustomData;
    if (!data) return false;

    data->Size = size;
    data->NumRows = 0;
    data->NumCols = 0;
    data->MaxRows = MAX(data->MaxRows, 1);
    data->MaxCols = MAX(data->MaxCols, 1);

    int col = 0;
    int row = 0;

    for (char *src = text; *src; src++) {
        char *trg = &data->Text[row*data->MaxCols + col];

        trg[0] = *src;
        trg[1] = 0;

        switch (*src) {
        default:
            col = col + 1;
            break;
        case '\b':
            col = MAX(col - 1, 0);
            break;
        case '\n':
            col = 0;
            row = row + 1;
            break;
        case '\t':
            do {
                col = col + 1;
            } while (col % 4 != 0);
            break;
        }

        data->NumCols = MAX(col, data->NumCols);
        data->NumRows = MAX(row, data->NumRows);
        if (col == data->MaxCols - 1) {
            col = 0;
            row++;
        }
        if (row == data->MaxRows) break;
    }

    CWindow *vscroll = textbox->Tail;
    CWindow *hscroll = textbox->Head;

    Scroll->SetUnitSize(hscroll, ABS(textbox->Font->FontW));
    Scroll->SetUnitSize(vscroll, ABS(textbox->Font->FontH));
    Scroll->SetUnitCount(hscroll, data->NumCols);
    Scroll->SetUnitCount(vscroll, data->NumRows);

    int hx1 = hscroll->PosX;
    int hy1 = hscroll->PosY;
    int hx2 = hscroll->PosX + hscroll->Wide;
    int hy2 = hscroll->PosY + hscroll->High;
    Scroll->Resize(hscroll, hx1, hy1, hx2, hy2);

    int vx1 = vscroll->PosX;
    int vy1 = vscroll->PosY;
    int vx2 = vscroll->PosX + vscroll->Wide;
    int vy2 = vscroll->PosY + vscroll->High;
    Scroll->Resize(vscroll, vx1, vy1, vx2, vy2);
    return true;
}

static ITextBox itextbox = {
    ITextBox_GetText,
    ITextBox_SetText
};

export ITextBox *TextBox = &itextbox;