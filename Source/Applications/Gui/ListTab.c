#include <Kernel.h>
#include <Shell.h>

import IMemory   *Memory;
import IBitmap   *Bitmap;
import IWindow   *Window;
import IWidget   *Widget;
import ITabBar   *TabBar;
import IListView *ListView;

extern CWindow *tabbar;
static CWindow *listview;

static CBitmap icon;
static char *headers[3] = { "", "Number", "Text" };
static int colsize[3];
static char *texts[] = {
    "Buttons",
    "TextBox",
    "ListView"
};
static char *icons[] = {
    "Apps/Gui/Cog.bmp",
    "Apps/Gui/Text.bmp",
    "Apps/Gui/List.bmp"
};

static int listcount;
static char *listdata;

static bool InsertListRecord(int number, char *text) {
    if (listcount >= 1024) return false;

    char *data = &listdata[64*listcount];
    char **item = (char**)data;
    listcount++;

    stosd(data, 0, 64/4);
    item[0] = (char*)&icon;
    item[1] = &data[16];
    item[2] = &data[32];

    Format(&data[16], "%d", number);
    Format(&data[32], "%s", text);

    ListView->Insert(listview, item);
    return true;
}

static bool ListOnLoad(CWindow *win, CEvent *evt) {
    if (!win || !tabbar) return false;
    CWindow *parent = win->Parent;
    win->PosX = parent->PosX;
    win->PosY = parent->PosY;
    win->Wide = parent->Wide;
    win->High = parent->High;

    listdata = Memory->Alloc(64*KB);
    if (!listdata) return false;

    char *path = "Media/Icons/bmp.bmp";
    if (!Bitmap->LoadSprite(&icon, path)) return false;

    listview = Widget->ListView(win);
    if (!listview) return false;
    listview->PosX = win->PosX;
    listview->PosY = win->PosY + 32;
    listview->Wide = win->Wide;
    listview->High = win->High - 32;
    listview->Visible = false;

    colsize[0] = 48;
    colsize[1] = 128;
    colsize[2] = listview->Wide - (48+128);

    CListView *list = listview->CustomData;
    if (!list) return false;

    list->NumCols  = elementsof(headers);
    list->Headers  = headers;
    list->ColSize  = colsize;
    list->RowSize  = 48;
    list->HeadSize = 24;

    static char *states[] = {
        "Alabama",        "Alaska",       "Arizona",      "Arkansas",
        "California",     "Colorado",     "Connecticut",  "Delaware",
        "Florida",        "Georgia",      "Hawaii",       "Idaho",
        "Illinois",       "Indiana",      "Iowa",         "Kansas",
        "Kentucky",       "Louisiana",    "Maine",        "Maryland",
        "Massachusetts",  "Michigan",     "Minnesota",    "Mississippi",
        "Missouri",       "Montana",      "Nebraska",     "Nevada",
        "New Hampshire",  "New Jersey",   "New Mexico",   "New York",
        "North Carolina", "North Dakota", "Ohio",         "Oklahoma",
        "Oregon",         "Pennsylvania", "Rhode Island", "South Carolina",
        "South Dakota",   "Tennessee",    "Texas",        "Utah",
        "Vermont",        "Virginia",     "Washington",   "West Virginia",
        "Wisconsin",      "Wyoming",
    };

    for (int i = 0; i < elementsof(states); i++) {
        InsertListRecord(i+1, states[i]);
    }
    return true;
}

static bool ListOnDraw(CWindow *win, CEvent *evt) {
    if (!win->Visible) return true;
    CCanvas *canvas = win->Canvas;
    if (canvas) canvas->Focus = listview;
    return true;
}

static bool ListOnAction(CWindow *win, CEvent *evt) {
    return true;
}

static bool ListOnSize(CWindow *win, CEvent *evt) {
    CEvtArea *area = (CEvtArea*)evt;
    win->Wide = area->Wide;
    win->High = area->High;

    int x = win->PosX;
    int y = win->PosY;
    int w = win->Wide;
    int h = win->High;
    if (listview) {
        colsize[2] = w - (48+128);
        Window->Resize(listview, x, y, w, h);
        listview->Visible = win->Visible;
    }
    return true;
}

bool ListProc(CWindow *win, CEvent *evt) {
    switch (evt->Code) {
    case EVT_ONLOAD:   return ListOnLoad(win, evt);
    case EVT_ONDRAW:   return ListOnDraw(win, evt);
    case EVT_ONSIZE:   return ListOnSize(win, evt);
    case EVT_ONACTION: return ListOnAction(win, evt);
    }
    return true;
}