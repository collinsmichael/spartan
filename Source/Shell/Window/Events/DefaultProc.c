#include <Kernel.h>
#include <Shell.h>

extern ICanvas *Canvas;
extern IEvent  *Event;

bool OnQuit     (CWindow *win, CEvent *evt);
bool OnLoad     (CWindow *win, CEvent *evt);
bool OnHide     (CWindow *win, CEvent *evt);
bool OnShow     (CWindow *win, CEvent *evt);
bool OnMove     (CWindow *win, CEvent *evt);
bool OnSize     (CWindow *win, CEvent *evt);
bool OnDraw     (CWindow *win, CEvent *evt);
bool OnAction   (CWindow *win, CEvent *evt);
bool OnFocusLost(CWindow *win, CEvent *evt);
bool OnFocus    (CWindow *win, CEvent *evt);
bool OnKeyUp    (CWindow *win, CEvent *evt);
bool OnKeyDown  (CWindow *win, CEvent *evt);
bool OnMouse    (CWindow *win, CEvent *evt);
bool OnWheel    (CWindow *win, CEvent *evt);
bool OnLeave    (CWindow *win, CEvent *evt);
bool OnEnter    (CWindow *win, CEvent *evt);
bool OnHover    (CWindow *win, CEvent *evt);
bool OnClick    (CWindow *win, CEvent *evt);
bool OnRelease  (CWindow *win, CEvent *evt);
bool OnDrag     (CWindow *win, CEvent *evt);
bool OnDrop     (CWindow *win, CEvent *evt);
bool OnTimer    (CWindow *win, CEvent *evt);

bool (*OnEvent[])(CWindow *win, CEvent *evt) = {
    OnQuit,      OnLoad,     OnHide,     OnShow,
    OnMove,      OnSize,     OnDraw,     OnAction,
    OnFocusLost, OnFocus,    OnKeyUp,    OnKeyDown,
    OnMouse,     OnWheel,    OnLeave,    OnEnter,
    OnHover,     OnClick,    OnRelease,  OnDrag,
    OnDrop,      OnTimer
};

bool DefaultProc(CWindow *win, CEvent *evt) {
    if (!win || !evt) return false;

    bool result = true;
    int code = evt->Code;
    if (code >= EVT_ONQUIT && code <= EVT_DEFAULTMAX) {
        if (code == EVT_ONDRAW) {
            if (win->Visible) {
                result = OnEvent[code](win, evt);
            }
        } else {
            result = OnEvent[code](win, evt);
        }
    }

    CCanvas *canvas = win->Canvas;
    if (canvas) {
        CWindow *frame = canvas->BtmMost;
        Canvas->Show(Canvas->GetScreen(), frame);
    }
    return result;
}