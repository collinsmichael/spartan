#define IMPL_IPIPE
#include <Kernel.h>

NOTE("(https://goo.gl/gqFv27) 8254 Programmable Interval Timer DataSheet")
NOTE("(https://goo.gl/Exojgh) Intel Port Mappings DataSheet")

extern IDevice *Device;
extern IPipe    *Pipe;

static CPipe *pipe;
static u32 hz;
static u16 counter;
static bool play = true;

u32 ISpeaker_SetHerz(u32 hertz) {
    hz = hertz;
    if (!hz) hz = 1;
    counter = 1193180 / hz;

    // 10|11|011|0
    outb(PIT_COMMAND, PIT_CH2|PIT_BOTH|PIT_SQWV);
    u8 lo = counter % 256;
    u8 hi = counter / 256;
    outb(PIT_TIMER2, lo);
    outb(PIT_TIMER2, hi);
    return true;
}

u32 Speaker(u32 err, u32 esp) {
    if (play) {
        char pcm = pipe->Pointer[pipe->Head % pipe->Capacity];
        pipe->Head++;
        ISpeaker_SetHerz(pcm);
    }
    return esp;
}

u32 ISpeaker_Play(char *pcm) {
    play = true;
    u8 status = inb(NMI_CTRL);
    outb(NMI_CTRL, status | 0x03); // Latch Speaker to PIT Timer 2
    return true;
}

u32 ISpeaker_Stop(void) {
    play = false;
    u8 status = inb(NMI_CTRL);
    outb(NMI_CTRL, status & 0xFC); // Unlatch Speaker to PIT Timer 2
    return true;
}

bool InstallSpeaker(u8 *base, u32 size) {
    pipe = (CPipe*)base;
    Pipe->Create(pipe, 1, 1); // 1 = Pid Shell
    play = false;
    hz = 0;
    return true;
}

bool EnableSpeaker(u8 *base, u32 size) {
    Device->Latch(IRQ_PIT, Speaker);
    return true;
}