#include <Kernel.h>
#include <Shell.h>

bool INoteBox_ClearText(CWindow *notebox) {
    if (!notebox) return false;
    CNoteBox *data = (CNoteBox*)notebox->CustomData;
    if (!data) return false;
    char *text = data->Text;
    if (text) stosd(text, 0, 256/4);
    data->Caret = 0;
    return true;
}

char *INoteBox_GetText(CWindow *notebox) {
    if (!notebox) return 0;
    CNoteBox *data = (CNoteBox*)notebox->CustomData;
    if (!data) return 0;
    return data->Text;
}

bool INoteBox_SetText(CWindow *notebox, char *text, int size) {
    if (!notebox) return false;
    CNoteBox *data = (CNoteBox*)notebox->CustomData;
    if (!data) return false;
    if (size > 255) return false;
    movsb(data->Text, text, size);
    return true;
}

int INoteBox_GetSize(CWindow *notebox) {
    if (!notebox) return 0;
    CNoteBox *data = (CNoteBox*)notebox->CustomData;
    if (!data) return 0;
    return data->Size;
}

bool INoteBox_SetSize(CWindow *notebox, int size) {
    if (!notebox) return false;
    CNoteBox *data = (CNoteBox*)notebox->CustomData;
    if (!data) return false;
    if (size >= 256) return false;
    data->Size = size;
    return true;
}

char *INoteBox_GetFormat(CWindow *notebox) {
    if (!notebox) return 0;
    CNoteBox *data = (CNoteBox*)notebox->CustomData;
    if (!data) return 0;
    return data->Format;
}

bool INoteBox_SetFormat(CWindow *notebox, char *text) {
    if (!notebox) return false;
    CNoteBox *data = (CNoteBox*)notebox->CustomData;
    if (!data) return false;
    data->Format = text;
    return true;
}

bool INoteBox_GetReadOnly(CWindow *notebox) {
    if (!notebox) return 0;
    CNoteBox *data = (CNoteBox*)notebox->CustomData;
    if (!data) return 0;
    return data->ReadOnly;
}

bool INoteBox_SetReadOnly(CWindow *notebox, bool value) {
    if (!notebox) return false;
    CNoteBox *data = (CNoteBox*)notebox->CustomData;
    if (!data) return false;
    data->ReadOnly = value;
    return true;
}

bool INoteBox_GetNumeric(CWindow *notebox) {
    if (!notebox) return 0;
    CNoteBox *data = (CNoteBox*)notebox->CustomData;
    if (!data) return 0;
    return data->Numeric;
}

bool INoteBox_SetNumeric(CWindow *notebox, bool value) {
    if (!notebox) return false;
    CNoteBox *data = (CNoteBox*)notebox->CustomData;
    if (!data) return false;
    data->Numeric = value;
    return true;
}

bool INoteBox_GetPassword(CWindow *notebox) {
    if (!notebox) return 0;
    CNoteBox *data = (CNoteBox*)notebox->CustomData;
    if (!data) return 0;
    return data->Password;
}

bool INoteBox_SetPassword(CWindow *notebox, bool value) {
    if (!notebox) return false;
    CNoteBox *data = (CNoteBox*)notebox->CustomData;
    if (!data) return false;
    data->Password = value;
    return true;
}

static INoteBox inotebox = {
    INoteBox_ClearText,
    INoteBox_GetText,
    INoteBox_SetText,
    INoteBox_GetSize,
    INoteBox_SetSize,
    INoteBox_GetFormat,
    INoteBox_SetFormat,
    INoteBox_GetReadOnly,
    INoteBox_SetReadOnly,
    INoteBox_GetNumeric,
    INoteBox_SetNumeric,
    INoteBox_GetPassword,
    INoteBox_SetPassword
};

export INoteBox *NoteBox = &inotebox;