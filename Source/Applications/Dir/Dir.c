#include <Kernel.h>
#include <Shell.h>
#include "Dir.h"

import IThread   *Thread;
import IProcess  *Process;
import IBitmap   *Bitmap;
import IEvent    *Event;
import IWindow   *Window;
import IWidget   *Widget;
import IListView *ListView;
extern ICache    *Cache;

static CBitmap button;
static char dir[256];
static char *headers[4] = { "", "Type", "Size", "Name" };
static int colsize[4];
static CWindow *nav;
static CWindow *back;
static CWindow *listview;

bool ChangeDir(CWindow *frame, CWindow *listview) {
    Logger("ChangeDir(%s)\n", dir);
    CListView *list = listview->CustomData;
    if (!list) return false;

    if (!Cache->ListFolder(listview, dir)) return false;

    CEvent draw;
    stosd(&draw, 0, sizeof(CEvent)/4);
    draw.Source = frame;
    draw.Target = frame->Parent;
    draw.Code = EVT_ONDRAW;
    Event->RaiseEvent(&draw);
    return true;
}

bool FolderOnLoad(CWindow *win, CEvent *evt) {
    CWindow *parent = win->Parent;
    if (parent) {
        parent->Title = dir;
    }

    if (!Bitmap->LoadSprite(&button, "Media/SysIcons/back.bmp")) {
        return false;
    }

    nav = Widget->Panel(win);
    if (!nav) return false;
    nav->PosX = win->PosX;
    nav->PosY = win->PosY;
    nav->Wide = win->Wide;
    nav->High = button.ResY;

    back = Widget->Button(win);
    if (!back) return false;
    back->WindowID = 0x0777;
    back->Bitmap = &button;
    back->PosX = win->PosX;
    back->PosY = win->PosY;
    back->Wide = button.ResX;
    back->High = button.ResY;

    listview = Widget->ListView(win);
    if (!listview) return false;
    listview->PosX = win->PosX;
    listview->PosY = win->PosY + button.ResY;
    listview->Wide = win->Wide;
    listview->High = win->High - button.ResY;

    CListView *list = listview->CustomData;
    if (!list) return false;

    list->NumCols  = elementsof(headers);
    list->Headers  = headers;
    list->ColSize  = colsize;
    list->RowSize  = 48;
    list->HeadSize = 24;

    return ChangeDir(win, listview);
}

bool FolderOnSize(CWindow *win, CEvent *evt) {
    CEvtArea *area = (CEvtArea*)evt;
    win->PosX = area->PosX;
    win->PosY = area->PosY;
    win->Wide = area->Wide;
    win->High = area->High;

    if (nav) {
        nav->PosX = win->PosX;
        nav->PosY = win->PosY;
        nav->Wide = win->Wide;
        nav->High = button.ResY;
    }

    if (listview) {
        listview->PosX = win->PosX;
        listview->PosY = win->PosY + button.ResY;
        listview->Wide = win->Wide;
        listview->High = win->High - button.ResY;

        colsize[0] = 48;
        colsize[1] = 64;
        colsize[2] = 96;
        colsize[3] = listview->Wide - (48+96+64+32);
    }
    return true;
}

bool OpenListItem(CWindow *win, CWindow *listview, CItem *item) {
    if (!item) return false;

    if (StriCmp(item->FileType, "DIR") == 0) {
        stosd(dir, 0, sizeof(dir)/4);
        StrCpy(dir, item->Full);
        ChangeDir(win, listview);
    } else {
        char *filter = Cache->GetAssociations(item);
        if (filter) {
            static char cmdline[256];
            Format(cmdline, filter, item->Full);
            Debug(" Process->Create(%s)\n", cmdline);
            CPcb *pcb = Process->Create(cmdline);
        }
    }
    return true;
}

bool GoBack(CWindow *frame) {
    CItem *item = (CItem*)Cache->FindFolder(dir);
    if (item) {
        stosd(dir, 0, sizeof(dir)/4);
        StrCpy(dir, item->Path);
        ChangeDir(frame, listview);
    }
    return true;
}

bool FolderOnAction(CWindow *win, CEvent *evt) {
    CWindow *child = evt->Source;
    if (child->WindowID == -1) {
        GoBack(win);
    } if (child->WindowID == 0x0777) {
        GoBack(win);
    } else {
        CItem *item = (CItem*)ListView->Selected(listview);
        OpenListItem(win, listview, item);
    }
    return true;
}

bool FolderOnKeyUp(CWindow *win, CEvent *evt) {
    CEvtKey *key = (CEvtKey*)evt;
    if (key->Key == KEY_SPACE || key->Key == KEY_ENTER) {
        CItem *item = (CItem*)Cache->FindFolder(dir);
        OpenListItem(win, listview, item);
    } else if (key->Key == KEY_ESC || key->Key == KEY_BACK) {
        GoBack(win);
    }
    return true;
}

bool FolderOnFocus(CWindow *win, CEvent *evt) {
    CCanvas *canvas   = win->Canvas;
    if (canvas && listview) {
        canvas->Focus = listview;
        if (listview->CallBack) {
            listview->CallBack(listview, evt);
        }
    }
    return false;
}

bool FolderOnDraw(CWindow *win, CEvent *evt) {
    CCanvas *canvas = win->Canvas;
    if (canvas) canvas->Focus = listview;
    return true;
}

bool FolderProc(CWindow *win, CEvent *evt) {
    switch (evt->Code) {
    case EVT_ONLOAD:    return FolderOnLoad(win, evt);
    case EVT_ONDRAW:    return FolderOnDraw(win, evt);
    case EVT_ONSIZE:    return FolderOnSize(win, evt);
    case EVT_ONACTION:  return FolderOnAction(win, evt);
    case EVT_ONKEYUP:   return FolderOnKeyUp(win, evt);
    case EVT_ONFOCUS:   return FolderOnFocus(win, evt);
    }
    return true;
}

int main(void) {
    stosd(dir, 0, sizeof(dir)/4);
    if (!Cache->SetUp()) return -1;

    CWindow *frame = Window->Create(nullptr, FolderProc);
    if (!frame) return -1;

    for (CEvent evt;;) {
        if (Event->GetEvent(&evt)) {
            Event->HandleEvent(&evt);
        }
    }
    return 0;
}

void start(void) {
    Thread->Exit(main());
}