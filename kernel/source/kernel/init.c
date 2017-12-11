#include <arch.h>
#include <drivers.h>

int init(void) {
    cli();
    gdt_reset();
    idt_reset();
    tss_reset();
    pic_remap();
    ps2_reset();
    sti();

    return 0;
}