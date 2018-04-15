#include <Kernel.h>
#include <Shell.h>

import IThread *Thread;
import IWidget *Widget;
import IEvent  *Event;

int main(void) {
    CWindow *console = Widget->Console(nullptr);
    if (!console) return -1;
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