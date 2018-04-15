#include <Kernel.h>

void Ps2Wait(u8 type) {
    u8 mask = (type) ? 2 : 1;
    u8 test = (type) ? 0 : 1;
    for (u32 t = 100000; t; t--) {
        if ((inb(PS2_STATUS) & mask) == test) {
            break;
        }
    }
    return;
}

void Ps2Send(u8 value) {
    Ps2Wait(1);
    outb(PS2_STATUS, 0xD4);
    Ps2Wait(1);
    outb(PS2_DATA, value);
}

u8 Ps2Read(void) {
    Ps2Wait(0);
    return inb(PS2_DATA);
}

bool Ps2Ack(void){ 
    for (int i = 0; i < 100000; i++) {
        if (inb(PS2_DATA) == 0xFA) return true;
    }
    return false;
}

void Ps2Data(char val) { 
    Ps2Wait(0);
    outb(PS2_DATA, val);
}

void Ps2Command(char cmd, char val) { 
    Ps2Wait(0);
    outb(PS2_DATA, cmd);
    Ps2Ack();

    outb(PS2_DATA, val);
    Ps2Wait(0);
    Ps2Ack();
}

bool EnablePs2(void) {
    Ps2Read();
    Ps2Wait(1);
    outb(PS2_STATUS, 0xA8);
    Ps2Wait(1);
    outb(PS2_STATUS, 0x20);
    Ps2Wait(0);
    u8 status = inb(PS2_DATA);
    Ps2Wait(1);
    outb(PS2_STATUS, 0x60);
    Ps2Wait(1);
    outb(PS2_DATA, (status|3) & ~0x10);
    Ps2Send(0xF6);
    Ps2Read();
    Ps2Send(0xF4);
    Ps2Read();
    return true;
}