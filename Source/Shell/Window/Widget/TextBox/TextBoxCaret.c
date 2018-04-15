#include <Kernel.h>
#include <Shell.h>

extern IFont     *Font;
extern ICanvas   *Canvas;
extern IEvent    *Event;
extern IWindow   *Window;
extern IWidget   *Widget;
extern IScroll   *Scroll;

static bool TextBoxOnNavigate(CWindow *win, CEvtKey *key) {
    CTextBox *data = (CTextBox*)win->CustomData;
    CFont    *font = win->Font;
    if (!data || !font) return false;
    char *text = &data->Text[win->CaretY*data->MaxCols];
    if (!text) return true;

    int x1 = win->PosX;
    int y1 = win->PosY;
    int x2 = win->PosX + win->Wide - 24;
    int y2 = win->PosY + win->High - 24;
    int fontw = MAX(font->FontW, 1);
    int fonth = MAX(font->FontH, 1);
    int pagex = (x2 - x1 + fontw - 1) / fontw;
    int pagey = (y2 - y1 + fonth - 1) / fonth;
    pagex = MIN(MAX(0, pagex), data->NumCols);
    pagey = MIN(MAX(0, pagey), data->NumRows);

    switch (key->Key) {
    case KEY_UP:    win->CaretY = MAX(win->CaretY - 1, 0);                     break;
    case KEY_DOWN:  win->CaretY = MIN(win->CaretY + 1, data->NumRows);         break;
    case KEY_LEFT:  win->CaretX = MAX(win->CaretX - 1, 0);                     break;
    case KEY_RIGHT: win->CaretX = MIN(win->CaretX + 1, data->NumCols);         break;

    case KEY_PGUP:
        if (key->Ctrl) win->CaretY = 0;
        else win->CaretY = MAX(win->CaretY - (pagey-1), 0);
        break;

    case KEY_PGDN:
        if (key->Ctrl) win->CaretY = data->NumRows;
        else win->CaretY = MIN(win->CaretY + (pagey-1), data->NumRows);
        break;

    case KEY_HOME:
        if (key->Ctrl) win->CaretY = 0;
        else win->CaretX = 0;
        break;

    case KEY_END:
        if (key->Ctrl) win->CaretY = data->NumRows;
        else {
            for (win->CaretX = 0; win->CaretX < data->MaxCols; win->CaretX++) {
                char c = text[win->CaretX];
                if (c == '\0' || c == '\n') break;
            }
        }
        break;
    }
    return true;
}

static bool TextBoxRemove(CWindow *win) {
    if (!win) return false;
    CTextBox *data = (CTextBox*)win->CustomData;
    if (!data) return false;

    char *text = &data->Text[win->CaretY*data->MaxCols];
    for (int x = win->CaretX; x < data->MaxCols-1; x++) {
        text[x] = text[x+1];
    }
    char c = text[win->CaretX];
    if (c == 0 || c == '\n') {
        int y = win->CaretY;
        char *target = &data->Text[y*data->MaxCols + win->CaretX];
        for (;;) {
            char *source = &data->Text[(++y)*data->MaxCols];
            if (y >= data->MaxRows-1) break;
            movsd(target, source, data->MaxCols/4);
            target = source;
        }
    }
    return true;
}

static bool TextBoxNewLine(CWindow *win) {
    if (!win) return false;
    CTextBox *data = (CTextBox*)win->CustomData;
    if (!data) return false;

    for (int y = data->MaxRows-2; y > win->CaretY; y--) {
        char *source = &data->Text[(y  )*data->MaxCols];
        char *target = &data->Text[(y+1)*data->MaxCols];
        movsd(target, source, data->MaxCols/4);
    }
    char *next = &data->Text[(win->CaretY+1)*data->MaxCols];
    stosd(next, 0, data->MaxCols/4);

    char *tail = &data->Text[win->CaretY*data->MaxCols + win->CaretX];
    movsb(next, tail, data->MaxCols-win->CaretX);
    stosb(tail, 0,    data->MaxCols-win->CaretX);
    win->CaretX = 0;
    win->CaretY++;
    return true;
}

static bool TextBoxInsert(CWindow *win, char c) {
    if (!win) return false;
    CTextBox *data = (CTextBox*)win->CustomData;
    if (!data) return false;

    char *text = &data->Text[win->CaretY*data->MaxCols];
    for (int x = data->MaxCols-2; x >= win->CaretX; x--) {
        text[x+1] = text[x];
    }
    text[win->CaretX++] = c;

    // deal with overflowing lines
    if (c == '\n' || text[data->MaxCols - 2]) {
        TextBoxNewLine(win);
    }
    return true;
}

static bool TextBoxOnAscii(CWindow *win, CEvtKey *key) {
    if (!win || !key) return false;

    CTextBox *data = (CTextBox*)win->CustomData;
    if (!data) return false;
    char *text = &data->Text[win->CaretY*data->MaxCols];

    switch (key->Key) {
    case KEY_BACK:
        if (win->CaretX == 0 && win->CaretY == 0) {
            return true;
        }

        win->CaretX = win->CaretX - 1;
        if (win->CaretX < 0) {
            win->CaretY = win->CaretY - 1;
            win->CaretY = MAX(win->CaretY, 0);

            text = &data->Text[win->CaretY*data->MaxCols];
            for (win->CaretX = data->MaxCols-1; win->CaretX > 0; win->CaretX--) {
                char c = text[win->CaretX];
                if (c != '\0' && c != '\x0D') break;
            }
        }
        TextBoxRemove(win);
        return true;

    case KEY_DELETE:
        TextBoxRemove(win);
        return true;
    }

    if (!Event->IsAscii(key)) return true;

    switch (key->Ascii) {
    case '\0': break;
    case '\b': break;
    case '\r': win->CaretX = 0; break;
    case '\n':
        TextBoxInsert(win, '\n');
        break;
    case '\t':
        do {
            TextBoxInsert(win, ' ');
            win->CaretX = MIN(win->CaretX, data->NumCols);
        } while (win->CaretX % 4 != 0);
        break;
    default:
        TextBoxInsert(win, key->Ascii);
        break;
    }
    return true;
}

static bool TextBoxRecalc(CWindow *win) {
    CTextBox *data = (CTextBox*)win->CustomData;
    if (!data) return false;

    CWindow  *hscroll = win->Head;
    CWindow  *vscroll = win->Tail;
    if (!hscroll || !vscroll) return false;

    CScroll  *hdata = (CScroll*)hscroll->CustomData;
    CScroll  *vdata = (CScroll*)vscroll->CustomData;
    CFont    *font = win->Font;
    if (!font || !hdata || !vdata) return false;

    int cols = 0;
    int rows = 0;
    for (int y = 0; y < data->MaxRows; y++) {
        char *text = &data->Text[y*data->MaxCols];
        if (!text[0]) break;
        rows = y;
        for (int x = cols; x < data->MaxCols; x++) {
            if (!text[x]) break;
            cols = x;
        }
    }

    data->NumRows = rows;
    data->NumCols = cols;
    Scroll->SetUnitCount(vscroll, rows);
    Scroll->SetUnitCount(hscroll, cols);

    int x1 = win->PosX;
    int y1 = win->PosY;
    int x2 = win->PosX + win->Wide - 24;
    int y2 = win->PosY + win->High - 24;
    int x3 = win->PosX + win->Wide;
    int y3 = win->PosY + win->High;
    Scroll->Resize(hscroll, x1, y2, x2, y3);
    Scroll->Resize(vscroll, x2, y1, x3, y2);

    int fontw = MAX(font->FontW, 1);
    int fonth = MAX(font->FontH, 1);
    int pagex = (x2 - x1 + fontw - 1) / fontw;
    int pagey = (y2 - y1 + fonth - 1) / fonth;
    pagex = MIN(MAX(0, pagex), data->NumCols);
    pagey = MIN(MAX(0, pagey), data->NumRows);
    int col = hdata->TrackPos*data->NumCols/hdata->TrackMax;
    int row = vdata->TrackPos*data->NumRows/vdata->TrackMax;

    CEvent evt;
    evt.Source = win;
    if (win->CaretY < row + 4) {
        int pos = win->CaretY - 4;
        pos = vdata->TrackMax*pos/MAX(1, data->NumRows);
        Scroll->SetTrackPos(vscroll, pos);
        evt.Code = EVT_ONDRAW;
        Event->Forward(vscroll, &evt);
    }
    if (win->CaretY > row + pagey - 4) {
        int pos = win->CaretY - pagey + 4;
        pos = vdata->TrackMax*pos/MAX(1, data->NumRows);
        Scroll->SetTrackPos(vscroll, pos);
        evt.Code = EVT_ONDRAW;
        Event->Forward(vscroll, &evt);
    }
    if (win->CaretX < col + 4) {
        int pos = win->CaretX - 4;
        pos = hdata->TrackMax*pos/MAX(1, data->NumCols);
        Scroll->SetTrackPos(hscroll, pos);
        evt.Code = EVT_ONDRAW;
        Event->Forward(hscroll, &evt);
    }
    if (win->CaretX > col + pagex - 4) {
        int pos = win->CaretX - pagex + 4;
        pos = hdata->TrackMax*pos/MAX(1, data->NumCols);
        Scroll->SetTrackPos(hscroll, pos);
        evt.Code = EVT_ONDRAW;
        Event->Forward(hscroll, &evt);
    }
    return true;
}

bool TextBoxCaret(CWindow *win, CEvent *evt) {
    CWindow *parent = win->Parent;
    if (parent) {
        CWindow *source = evt->Source;
        CWindow *target = evt->Target;
        evt->Source = win;
        evt->Target = parent;
        bool status = parent->CallBack(parent, evt);
        evt->Source = source;
        evt->Target = target;
        if (!status) return false;
    }

    TextBoxOnNavigate(win, (CEvtKey*)evt);
    TextBoxOnAscii(win, (CEvtKey*)evt);
    TextBoxRecalc(win);
    return true;
}