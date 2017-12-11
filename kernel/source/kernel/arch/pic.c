#include <arch.h>
#include <pic.h>

void pic_remap(void) {
    cli();
    outb(PIC1,      0x11); // ICW1_INIT+ICW1_ICW4
    outb(PIC2,      0x11); // ICW1_INIT+ICW1_ICW4
    outb(PIC1_DATA, 0x20); // PIC1 vector
    outb(PIC2_DATA, 0x28); // PIC2 vector
    outb(PIC1_DATA, 0x04); // tell PIC1 its master chained to PIC2
    outb(PIC2_DATA, 0x02); // tell PIC2 its identity is slave
    outb(PIC1_DATA, 0x01); // ICW4_8086 (generate 8086 interrupts)
    outb(PIC2_DATA, 0x01); // ICW4_8086 (generate 8086 interrupts)
    outb(PIC1_DATA, 0x00); // unmask all interrupts
    outb(PIC2_DATA, 0x00); // unmask all interrupts
}