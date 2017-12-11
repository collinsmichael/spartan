#include <ps2.h>
#include <arch.h>

static uint8_t kbuf[256];
static uint8_t kpos;
uint8_t key;

static uint8_t mbuf[256];
static uint8_t mpos;
int mx;
int my;
int m;

static
void *keyboard_isr(void *esp) {
    key = kbuf[kpos++] = inb(PS2_DATA);
    return esp;
}

static
void *mouse_isr(void *esp) {
    mbuf[mpos] = inb(PS2_DATA);
    if ((mpos = ++mpos % 3) == 0) {
        m = mbuf[0];
        if (m & 0x08) {
            int8_t x = mbuf[1];
            int8_t y = mbuf[2];
            if (m & 0x40) x |= 0x00000100;
            if (m & 0x80) y |= 0x00000100;
            if (m & 0x10) x |= 0xFFFFFE00;
            if (m & 0x20) y |= 0xFFFFFE00;
            mx += x;
            my -= y;
        } else mpos++;
    }
    return esp;
}

static
void ps2_wait(uint8_t type) {
    uint32_t t = 100000;
    uint8_t mask = (type) ? 2 : 1;
    uint8_t test = (type) ? 0 : 1;
    for (t = 100000; t; t--) {
        if ((inb(PS2_STATUS) & mask) == test) {
            break;
        }
    }
    return;
}

static
void ps2_send(uint8_t value) {
    ps2_wait(1);
    outb(PS2_STATUS, 0xD4);
    ps2_wait(1);
    outb(0x60, value);
}

static
uint8_t ps2_read(void) {
    ps2_wait(0);
    return inb(PS2_DATA);
}

void ps2_reset(void) {
    mpos = 0;
    idt_install(MS_IRQ, mouse_isr);
    kpos = 0;
    idt_install(KB_IRQ, keyboard_isr);

    ps2_read();
    ps2_wait(1);
    outb(PS2_STATUS, 0xA8);
    ps2_wait(1);
    outb(PS2_STATUS, 0x20);
    ps2_wait(0);
    uint8_t status = inb(PS2_DATA);
    ps2_wait(1);
    outb(PS2_STATUS, 0x60);
    ps2_wait(1);
    outb(PS2_DATA, (status|3) & ~0x10);
    ps2_send(0xF6);
    ps2_read();
    ps2_send(0xF4);
    ps2_read();
}