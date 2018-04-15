#include <Kernel.h>
#include <Shell.h>

char **IListView_Retrieve(CWindow *lv, int i) {
    if (!lv) return 0;
    CListView *data = lv->CustomData;
    if (!data) return 0;
    if (i < 0 || i >= data->NumRows) return false;
    return data->Items[i];
}

static char **IListView_Selected(CWindow *lv) {
    if (!lv) return 0;
    CListView *data = lv->CustomData;
    if (!data) return 0;
    return IListView_Retrieve(lv, data->Selected);
}

static bool IListView_Insert(CWindow *lv, char **li) {
    if (!lv || !li) return false;
    CListView *data = lv->CustomData;
    if (!data) return false;
    if (data->NumRows >= data->MaxRows) return false;

    data->NumRows = MAX(0, data->NumRows);
    data->Items[data->NumRows] = li;
    data->NumRows++;
    return true;
}

static bool IListView_Remove(CWindow *lv, int i) {
    if (!lv) return false;

    CListView *data = lv->CustomData;
    if (!data) return false;
    if (i >= data->NumRows) return false;
    if (i == data->Selected) data->Selected = -1;
    if (i == data->Hover) data->Hover = -1;
    data->NumRows = MAX(data->NumRows-1, 0);
    data->Items[i] = data->Items[data->NumRows];
    data->Items[data->NumRows] = nullptr;
    return true;
}

static bool IListView_RemoveAll(CWindow *lv) {
    if (!lv) return false;

    CListView *data = lv->CustomData;
    if (!data) return false;

    if (data->NumRows > 0) {
        stosd(data->Items, nullptr, data->NumRows);
    }
    data->Scroll = 0;
    data->Selected = -1;
    data->Hover = -1;
    data->NumRows = 0;
    return true;
}

static IListView listview = {
    IListView_Retrieve,
    IListView_Selected,
    IListView_Insert,
    IListView_Remove,
    IListView_RemoveAll
};

export IListView *ListView = &listview;