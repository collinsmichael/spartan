#include <Kernel.h>
#include <Shell.h>

extern IFont     *Font;
extern ICanvas   *Canvas;
extern IEvent    *Event;
extern IWindow   *Window;
extern IWidget   *Widget;
extern IScroll   *Scroll;

static bool NoteBoxOnNavigate(CWindow *win, CEvtKey *key) {
    CNoteBox *data = (CNoteBox*)win->CustomData;
    CFont *font = win->Font;
    if (!data || !font) return false;
    char *text = data->Text;
    if (!text) return true;
    int length = StrLen(text);

    int x1 = win->PosX;
    int y1 = win->PosY;
    int x2 = win->PosX + win->Wide;
    int y2 = win->PosY + win->High;
    int fontw = MAX(font->FontW, 1);
    int fonth = MAX(font->FontH, 1);

    switch (key->Key) {
    case KEY_LEFT:  data->Caret = MAX(data->Caret - 1, 0);      break;
    case KEY_RIGHT: data->Caret = MIN(data->Caret + 1, length); break;
    case KEY_HOME:  data->Caret = 0;                            break;
    case KEY_END:   data->Caret = length;                       break;
    }

    data->Caret = MIN(MAX(0, data->Caret), data->Size);
    return true;
}

static bool NoteBoxRemove(CWindow *win) {
    if (!win) return false;
    CNoteBox *data = (CNoteBox*)win->CustomData;
    if (!data) return false;

    char *text = data->Text;
    for (int x = data->Caret; x < 255; x++) {
        text[x] = text[x+1];
    }
    return true;
}

static bool NoteBoxInsert(CWindow *win, char c) {
    if (!win) return false;
    CNoteBox *data = (CNoteBox*)win->CustomData;
    if (!data) return false;

    char *text = data->Text;
    for (int x = 255; x >= data->Caret; x--) {
        char c = (x >= data->Size) ? 0 : text[x];
        text[x+1] = c;
    }
    text[data->Caret] = c;
    if (data->Caret < data->Size) data->Caret++;
    data->Caret = MIN(MAX(0, data->Caret), data->Size);
    return true;
}

static bool NoteBoxOnAscii(CWindow *win, CEvtKey *key) {
    if (!win || !key) return false;

    CNoteBox *data = (CNoteBox*)win->CustomData;
    if (!data) return false;
    char *text = data->Text;

    switch (key->Key) {
    case KEY_BACK:
        if (data->Caret > 0) {
            data->Caret = data->Caret - 1;
            data->Caret = MAX(data->Caret, 0);
            NoteBoxRemove(win);
        }
        return true;

    case KEY_DELETE:
        NoteBoxRemove(win);
        return true;
    }

    if (!Event->IsAscii(key)) return true;

    switch (key->Ascii) {
    case '\0': break;
    case '\b': break;
    case '\r': data->Caret = 0; break;
    case '\n': break;
    case '\t': NoteBoxInsert(win, ' '); break;
    default:
        if (data->Numeric) {
            if (key->Ascii < '0' || key->Ascii > '9') {
                break;
            }
            NoteBoxInsert(win, key->Ascii);
            int num = 0;
            for (char *txt = data->Text; *txt; txt++) {
                num = num*10 + *txt - '0';
            }
            num = MIN(MAX(win->MinValue, num), win->MaxValue);
            win->CurValue = num;
            Format(data->Text, "%d", win->CurValue);
            data->Caret = StrLen(data->Text);
        } else {
            NoteBoxInsert(win, key->Ascii);
        }
        break;
    }

    if (key->Key == KEY_ENTER || key->Key == KEY_NUMENTER) {
        static CEvent action;
        action.Source = win;
        action.Target = win->Parent;
        action.Code   = EVT_ONACTION;
        Event->Forward(win->Parent, &action);
    }
    return true;
}

bool NoteBoxCaret(CWindow *win, CEvent *evt) {
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

    NoteBoxOnNavigate(win, (CEvtKey*)evt);
    NoteBoxOnAscii(win, (CEvtKey*)evt);
    return true;
}