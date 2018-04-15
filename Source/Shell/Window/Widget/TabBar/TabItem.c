#include <Kernel.h>
#include <Shell.h>

extern IBitmap *Bitmap;
extern IWindow *Window;
extern IWidget *Widget;

CTabItem *ITabBar_Retrieve(CWindow *tb, int i) {
    if (!tb) return 0;
    CTabBar *data = (CTabBar*)tb->CustomData;
    if (!data) return 0;
    if (i < 0 || i >= data->Count) return false;

    CTabItem *item = &data->Items[i];
    return item;
}

static CTabItem *ITabBar_Selected(CWindow *tb) {
    if (!tb) return 0;
    CTabBar *data = tb->CustomData;
    if (!data) return 0;
    return ITabBar_Retrieve(tb, data->Selected);
}

static CTabItem *ITabBar_Insert(CWindow *tb, int count, char *text[], char *icon[]) {
    if (!tb || !text) return 0;
    CTabBar *data = (CTabBar*)tb->CustomData;
    if (!data) return 0;

    data->Count = MAX(0, data->Count);
    if (data->Count + count >= 256) return 0;

    char    *texts = data->Texts;
    CBitmap *icons = data->Icons;
    CTabItem *active = nullptr;
    for (int i = 0; i < count; i++) {
        int x = data->Count;
        CTabItem *item = &data->Items[x];

        item->TabId  = x;
        item->Icon   = nullptr;
        item->Text   = nullptr;
        item->Button = nullptr;

        if (text) {
            item->Text = &texts[x*32];
            movsb(item->Text, text[i], 31);
            item->Text[31] = 0;
        }

        if (icon) {
            item->Icon = &icons[x];
            if (!Bitmap->LoadSprite(item->Icon, icon[i])) {
                return 0;
            }
        }

        CWindow *button = Widget->Button(tb);
        if (!button) return 0;
        item->Button = button;
        button->WindowID = 0x100 + x;
        button->Wide = 128;
        button->High = 32;
        button->PosX = tb->PosX + x*button->Wide;
        button->PosY = tb->PosY;
        button->Text = item->Text;
        button->Bitmap = item->Icon;

        data->Count++;
        active = item;
    }
    if (data->Selected < 0) {
        data->Selected = data->Count;
    }
    return active;
}

static bool ITabBar_Remove(CWindow *tb, int i) {
    if (!tb) return false;

    CTabBar *data = (CTabBar*)tb->CustomData;
    if (!data) return 0;

    if (i >= data->Count) return false;
    if (i == data->Selected) data->Selected = -1;

    CTabItem *tab = &data->Items[i];
    if (tab->Button) Window->Destroy(tab->Button);
    if (tab->Icon  ) Bitmap->Destroy(tab->Icon);

    data->Count = MAX(data->Count-1, 0);

    CTabItem *sItem = &data->Items[data->Count];
    CTabItem *tItem = &data->Items[i];
    movsd(tItem, sItem, sizeof(CTabItem)/4);
    stosd(sItem, 0, sizeof(CTabItem)/4);

    char *sText = &data->Texts[data->Count];
    char *tText = &data->Texts[i];
    movsd(tText, sText, 32/4);
    stosd(sText, 0, 32/4);

    CBitmap *sIcon = &data->Icons[data->Count];
    CBitmap *tIcon = &data->Icons[i];
    movsd(tText, sText, sizeof(CBitmap)/4);
    stosd(sText, 0, sizeof(CBitmap)/4);
    return true;
}

static bool ITabBar_RemoveAll(CWindow *tb) {
    if (!tb) return false;
    CTabBar *data = (CTabBar*)tb->CustomData;
    if (!data) return 0;

    CTabItem *items = data->Items;
    for (int i = 0; i < data->Count; i++) {
        CTabItem *item = &items[i];
        if (item->Button) Window->Destroy(item->Button);
        if (item->Icon  ) Bitmap->Destroy(item->Icon);
    }
    data->Count = 0;
    data->Selected = -1;
    return true;
}

static ITabBar tabbar = {
    ITabBar_Remove,
    ITabBar_RemoveAll,
    ITabBar_Insert,
    ITabBar_Retrieve,
    ITabBar_Selected
};

export ITabBar *TabBar = &tabbar;