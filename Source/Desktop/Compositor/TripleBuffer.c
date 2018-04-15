/*
#include <Kernel.h>
#include <Shell.h>
#include <Desktop.h>

CWindow *desk = nullptr;
CBuffer *head = nullptr;
CBuffer *tail = nullptr;

bool FetchDrawBuffers(int y) {
    head = nullptr;
    tail = nullptr;

    for (CCanvas *list = desk; list; list = list->Prev) {
        if (list->Sequence == list->Disp) continue;
        list->Disp = list->Sequence;

        ((head) ? head->Prev : tail) = list;
        head = list;
    }
    return 1;
}

int y = 0;
for (int t = 0;; t++) {
    if (t == 21) t = 0; // synchronize to vertical retrace
    if (t != 0) {
        if (y < ResY) {
            DisplayScanLine(y);
            y++;
        } else {
            FetchDrawBuffers();
            y = 0;
        }
    }
}
*/