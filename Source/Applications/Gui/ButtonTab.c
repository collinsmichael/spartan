#include <Kernel.h>
#include <Shell.h>

import IGraphics *Graphics;
import ICanvas   *Canvas;
import IEvent    *Event;
import IWindow   *Window;
import IWidget   *Widget;
import ITabBar   *TabBar;
import INoteBox  *NoteBox;

extern CWindow *tabbar;

static CWindow *focus;
static CWindow *progress;
static CWindow *less;
static CWindow *more;
static CWindow *slider;
static CWindow *scroll;
static CWindow *number;
static CWindow *password;
static CWindow *username;

static CWindow *child[8];
static char *names[8] = {
    "scroll", "less",     "more",     "progress",
    "slider", "username", "password", "number"
};
static int pos[] = {
    342,  32,  32, 160, // scroll
    136,  32,  32,  32, // less
    296,  32,  32,  32, // more
    168,  32, 128,  32, // progress
    136,  96, 192,  32, // slider
     16,  32,  96,  32, // username
     16,  96,  96,  32, // password
     16, 160,  96,  32  // number
};

static bool ButtonOnLoad(CWindow *win, CEvent *evt) {
    CWindow *frame = win->Parent;
    if (!frame) return false;
    win->BackGround[0] = 0x00EBEBEB;
    win->ForeGround[0] = 0x001F1F1F;
    win->PosX = frame->PosX;
    win->PosY = frame->PosY + 32;
    win->Wide = frame->Wide;
    win->High = frame->High - 32;

    child[0] = scroll   = Widget->Slider(win);
    child[1] = less     = Widget->Button(win);
    child[2] = more     = Widget->Button(win);
    child[3] = progress = Widget->Progress(win);
    child[4] = slider   = Widget->Slider(win);
    child[5] = username = Widget->NoteBox(win);
    child[6] = password = Widget->NoteBox(win);
    child[7] = number   = Widget->NoteBox(win);
    for (int i = 0; i < elementsof(child); i++) {
        if (!child[i]) return false;
        child[i]->PosX = pos[i*4 + 0] + win->PosX;
        child[i]->PosY = pos[i*4 + 1] + win->PosY;
        child[i]->Wide = pos[i*4 + 2];
        child[i]->High = pos[i*4 + 3];
        child[i]->WindowID = 0x7001 + i;
    }

    if (less) less->Text = "<";
    if (more) more->Text = ">";
    if (progress) {
        progress->ForeGround[0] = 0x003FFF3F;
        progress->MinValue  = 0;
        progress->CurValue  = 128;
        progress->MaxValue  = 256;
        progress->StepValue = 8;
    }
    if (slider) {
        slider->BackGround[0] = 0x001F1F1F;
        slider->BackGround[1] = 0x00EBEBEB;
        slider->Orientation = WIN_ORIENT_HORZ;
        slider->MinValue    = 0;
        slider->CurValue    = 128;
        slider->MaxValue    = 256;
        slider->StepValue   = 8;
        slider->ThumbWidth  = 64;
    }
    if (scroll) {
        scroll->BackGround[0] = 0x001F1F1F;
        scroll->BackGround[1] = 0x00EBEBEB;
        scroll->Orientation = WIN_ORIENT_VERT;
        scroll->MinValue    = 0;
        scroll->CurValue    = 128;
        scroll->MaxValue    = 256;
        scroll->StepValue   = 8;
        scroll->ThumbWidth  = 64;
    }
    NoteBox->SetSize(username, 5);
    NoteBox->SetPassword(password, true);
    NoteBox->SetSize(password, 5);
    NoteBox->SetNumeric(number, true);
    NoteBox->SetSize(number, 5);
    number->MinValue = 0;
    number->MaxValue = 1000;
    return true;
}

static bool ButtonOnDraw(CWindow *win, CEvent *evt) {
    //Debug("ButtonTabOnDraw()\n");
    if (!win->Visible) return true;

    CCanvas *canvas = win->Canvas;
    int x1 = win->PosX;
    int y1 = win->PosY;
    int x2 = win->PosX + win->Wide;
    int y2 = win->PosY + win->High;
    int c  = win->BackGround[0];
    Graphics->Poly4f(x1, y1, x2, y2, c);

    for (int i = 3; i < elementsof(names); i++) {
        int x = pos[i*4 + 0] + win->PosX +  4;
        int y = pos[i*4 + 1] + win->PosY - 20;
        if (i == 3) x -= 32; // progress label fixup
        Window->TextOut(win, x, y, "%s", names[i]);
    }


    progress->Visible = true; Event->Forward(progress, evt);
    less->Visible     = true; Event->Forward(less, evt);
    more->Visible     = true; Event->Forward(more, evt);
    slider->Visible   = true; Event->Forward(slider, evt);
    scroll->Visible   = true; Event->Forward(scroll, evt);
    number->Visible   = true; Event->Forward(number, evt);
    password->Visible = true; Event->Forward(password, evt);
    username->Visible = true; Event->Forward(username, evt);

    Canvas->Blit(canvas, x1, y1, x2, y2);
    if (canvas) canvas->Focus = focus;
    return true;
}

static bool ButtonOnAction(CWindow *win, CEvent *evt) {
    CWindow *child = evt->Source;
    if (!child) return false;

    if (progress) {
        switch (child->WindowID) {
        case 0x7002: // less
            progress->CurValue = MAX(progress->CurValue-progress->StepValue, progress->MinValue);
            Window->Redraw(progress);
            break;
        case 0x7003: // more
            progress->CurValue = MIN(progress->CurValue+progress->StepValue, progress->MaxValue);
            Window->Redraw(progress);
            break;
        }
    }
    return true;
}

static bool ButtonOnSize(CWindow *win, CEvent *evt) {
    CEvtArea *area = (CEvtArea*)evt;
    win->Wide = area->Wide;
    win->High = area->High;
    return true;
}

bool ButtonProc(CWindow *win, CEvent *evt) {
    switch (evt->Code) {
    case EVT_ONLOAD:   return ButtonOnLoad(win, evt);
    case EVT_ONDRAW:   return ButtonOnDraw(win, evt);
    case EVT_ONSIZE:   return ButtonOnSize(win, evt);
    case EVT_ONACTION: return ButtonOnAction(win, evt);
    }
    return true;
}