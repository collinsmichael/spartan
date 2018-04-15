#include <Kernel.h>
#include <Shell.h>

import IThread   *Thread;
import IMemory   *Memory;
import IProcess  *Process;
import IGraphics *Graphics;
import IBitmap   *Bitmap;
import ICanvas   *Canvas;
import IEvent    *Event;
import IWindow   *Window;
import IWidget   *Widget;
import ITabBar   *TabBar;

bool ButtonProc(CWindow *win, CEvent *evt);
bool TextProc(CWindow *win, CEvent *evt);
bool ListProc(CWindow *win, CEvent *evt);

CWindow *tabbar;
CWindow *buttontab;
CWindow *texttab;
CWindow *listtab;

static CBitmap icon;
static char *texts[] = {
    "Test", "List"
};
static char *icons[] = {
    "Apps/Gui/Cog.bmp",
    "Apps/Gui/List.bmp"
};

bool OnLoad(CWindow *win, CEvent *evt) {
    CWindow *parent = win->Parent;
    if (parent) {
        parent->Title = "Gui Demo";
    }

    tabbar = Widget->TabBar(win);
    if (!tabbar) return false;

    if (!TabBar->Insert(tabbar, elementsof(texts), texts, icons)) {
        return false;
    }

    int x = win->PosX;
    int y = win->PosY + 32;
    int w = win->Wide;
    int h = win->High - 32;

    buttontab = Window->Create(win, ButtonProc);
    listtab = Window->Create(win, ListProc);
    return true;
}

bool OnDraw(CWindow *win, CEvent *evt) {
    if (!win->Visible) return true;
    return true;
}

bool ShowHide(CWindow *child, bool show) {
    for (CWindow *list = child->Head; list; list = list->Next) {
        child->Visible = show;
    }
    return true;
}

bool OnAction(CWindow *win, CEvent *evt) {
    CTabItem *tab = TabBar->Selected(tabbar);
    if (tab) {
        Debug(" TabID = %d\n", tab->TabId);
        CWindow *child = nullptr;
        switch (tab->TabId) {
        case 0: child = buttontab; break;
        case 1: child = listtab;   break;
        }

        if (child) {
            ShowHide(buttontab, false);
            ShowHide(listtab, false);
            ShowHide(child, true); 
            evt->Code = EVT_ONDRAW;
            child->CallBack(child, evt);
        }
    }
    return true;
}

bool OnSize(CWindow *win, CEvent *evt) {
    CEvtArea *area = (CEvtArea*)evt;
    win->Wide = area->Wide;
    win->High = area->High;

    int x = win->PosX;
    int y = win->PosY + 32;
    int w = win->Wide;
    int h = win->High - 32;
    if (tabbar) Window->Resize(tabbar, x, y-32, w, 32);
    if (buttontab) Window->Resize(buttontab, x, y, w, h);
    //if (texttab) Window->Resize(texttab, x, y, w, h);
    if (listtab) Window->Resize(listtab, x, y, w, h);
    return true;
}

bool GuiProc(CWindow *win, CEvent *evt) {
    switch (evt->Code) {
    case EVT_ONLOAD:   return OnLoad(win, evt);
    case EVT_ONDRAW:   return OnDraw(win, evt);
    case EVT_ONSIZE:   return OnSize(win, evt);
    case EVT_ONACTION: return OnAction(win, evt);
    }
    return true;
}

int main(void) {
    CWindow *frame = Window->Create(nullptr, GuiProc);
    if (!frame) return -1;
    for (CEvent evt;;) {
        if (Event->GetEvent(&evt)) {
            Event->HandleEvent(&evt);
        }
    }
    return 0;
}