#include <arch.h>

#define IDT       ((uint64_t*)(IDT_BASE+0x0000))
#define IRQ       ((void   **)(IDT_BASE+0x0800))
#define ISR       ((char    *)(IDT_BASE+0x0C00))
#define DUMMY     ((char    *)(&ISR[256*32]))
#define FIXUP     ((void   **)(&ISR[i*32+0x04]))
#define IDT_MAGIC(i) (0x00008E0000000000 + GDT_RING0_CODE*65536 + (int)(&ISR[i*32]))

static char blank[128] = 
"\x60\x54\xFF\x15\x00\x00\x00\x00\x94\xB0\x20\xE6\x20\x61\xCF\x00"
"\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00"
"\x60\x54\xFF\x15\x00\x00\x00\x00\x94\xB0\x20\xE6\x20\x61\x8F\x04"
"\x24\xCF\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00"
"\x60\x54\xFF\x15\x00\x00\x00\x00\x94\xB0\x20\xE6\xA0\xE6\x20\x61"
"\xCF\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00"
"\x8B\x44\x24\x04\xC3\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00"
"\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00";

void idt_install(int irq, void *(*handler)(void*esp)) {
    IRQ[irq] = handler;
}

void idt_remove(int irq) {
    IRQ[irq] = DUMMY;
}

void idt_reset(void) {
    for (int i = 0; i < 256; i++) IDT[i] = IDT_MAGIC(i);
    for (int i = 0; i < 256; i++) IRQ[i] = DUMMY;
    for (int i = 0x00; i <= 0xFF; i++) memcpy(&ISR[i*32], &blank[0x00], 32);
    for (int i = 0x0A; i <= 0x0E; i++) memcpy(&ISR[i*32], &blank[0x20], 32);
    for (int i = 0x28; i <= 0x2F; i++) memcpy(&ISR[i*32], &blank[0x40], 32);
    memcpy(&ISR[0x08*32], &blank[0x20], 32);
    memcpy(&ISR[0x11*32], &blank[0x20], 32);
    memcpy(&ISR[256*32], &blank[0x60], 32);
    for (int i = 0; i < 256; i++) FIXUP[0] = &IRQ[i];
    lidt(IDT, 2047);
}