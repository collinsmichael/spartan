#include <Library.h>
#include <Kernel.h>
#include <Shell.h>

import IThread  *Thread;
import IProcess *Process;
extern IEvent   *Event;

// true entry point for all applications
void DeathLoop(void) {
    CPeb *peb = Process->GetPeb();
    CCanvas *canvas = (peb) ? peb->Canvas : nullptr;
    CWindow *frame = (canvas) ? canvas->BtmMost : nullptr;
    CWindow *close = (frame) ? frame->Tail : nullptr;

    for (;;) {
        CEvent evt;
        Thread->Yield();
        if (Event->GetEvent(&evt)) {
            CWindow *target = evt.Target;
            if (target == frame || target == close) {
                Event->HandleEvent(&evt);
            }
        }
    }
}