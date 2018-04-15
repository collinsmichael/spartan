#include <Kernel.h>
#include <Shell.h>
#include <Desktop.h>

import IThread   *Thread;
import IProcess  *Process;
import IPool     *Pool;
import IClock    *Clock;
import IKeyboard *Keyboard;
import IMouse    *Mouse;
import IWindow   *Window;
import INetwork  *Network;

extern IIdleThread *IdleThread;

int main(void) {
    Clock->Flush();
    Keyboard->Flush();
    Mouse->Flush();
    Window->Install();
    Network->Install();

    for (;;) {
        IdleThread->Run();
    }
    return 0;
}

int _stdcall start(int a, int b, int c) {
    Thread->Yield();
    if (Thread->GetPid() != 0) Thread->Exit(0);
    for (;;) main();
    return 0;
}