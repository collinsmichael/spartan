#include <Kernel.h>
#include <Shell.h>
#include <Desktop.h>

import IDevice *Device;
import ICanvas *Canvas;
import IEvent  *Event;

static CCanvas *screen;
static char keybrd[256];

static bool CaretHook(CPipe *pipe) {
    u8 packet[8];
    if (pipe->Consumed < sizeof(packet)) return true;
    for (u32 i = 0; i < sizeof(packet); i++) {
        u8 byte = pipe->Pointer[(pipe->Head + i) % pipe->Capacity];
        packet[i] = byte;
    }
    pipe->Head = pipe->Tail;
    pipe->Consumed -= sizeof(packet);

    CWindow *frame = screen->Focus;
    if (frame) {
        u8 down = packet[1];
        CEvent evt;
        stosd(&evt, 0, sizeof(CEvent)/4);
        evt.Target = screen->Focus;
        evt.Code   = (down) ? EVT_ONKEYDOWN : EVT_ONKEYUP;
        movsd(evt.Data, packet, sizeof(packet)/4);
        Event->RaiseEvent(&evt);
    }
    return true;
}

bool CaretInstall(void) {
    screen = Canvas->GetScreen();
    stosd(keybrd, 0, sizeof(keybrd)/4);
    if (!Device->Hook('KDB ', CaretHook)) {
        Logger(" [FAIL] Failed to hook caret to keyboard!\n");
        return false;
    }
    return true;
}