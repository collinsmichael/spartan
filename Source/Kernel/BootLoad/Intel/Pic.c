#include <Kernel.h>

NOTE("************************************************************************")
NOTE("* 8259A Programmable Interrupt Controller (https://goo.gl/5Wcv5q)      *")
NOTE("*                                                                      *")
NOTE("* The Programmable Interrupt Controller manages IRQs                   *")
NOTE("* It has a master and a slave. Both need to be remapped to avoid BIOS  *")
NOTE("* conflicts due to IBM reserving the same IRQs that Intel reserved     *")
NOTE("*                                                                      *")
NOTE("* Jargon: IRQ is the interrupt signal which is mapped to an ISR        *")
NOTE("* Jargon: ISR is a subroutine which is executed in response to an IRQ  *")
NOTE("************************************************************************")

static u8 pic1;
static u8 pic2;

bool RemapPic(void) {
    Logger(" Remapping  PIC  ... OK\n");
	pic1 = inb(PIC1_DATA);                     // save masks
	pic2 = inb(PIC2_DATA);                     // save masks
    outb(PIC1_CMD,  ICW1|ICW1_INIT|ICW1_ICW4); // ICW1_INIT+ICW1_ICW4
    outb(PIC2_CMD,  ICW1|ICW1_INIT|ICW1_ICW4); // ICW1_INIT+ICW1_ICW4
    outb(PIC1_DATA, ICW2|0x20);                // PIC1 vector
    outb(PIC2_DATA, ICW2|0x28);                // PIC2 vector
    outb(PIC1_DATA, ICW3|ICW3_IRQ);            // tell PIC1 its master chained to PIC2
    outb(PIC2_DATA, ICW3|ICW3_ID);             // tell PIC2 its identity is slave
    outb(PIC1_DATA, ICW4|ICW4_8086);           // generate 8086 interrupts
    outb(PIC2_DATA, ICW4|ICW4_8086);           // generate 8086 interrupts

    pic1 = 0;//~(PIC1_SPK|PIC1_KBD|PIC1_PIC2|PIC1_COM1|PIC1_COM2);
    pic2 = 0;//~(PIC2_RTC|PIC2_CGA|PIC2_AUX |PIC2_FPU |PIC2_HDD );
    outb(PIC1_DATA, pic1);                     // unmask all interrupts
    outb(PIC2_DATA, pic2);                     // unmask all interrupts
    return true;
}

bool DisablePic(void) {
    outb(PIC1_DATA, pic1 = 0xFF); // mask all interrupts
    outb(PIC2_DATA, pic2 = 0xFF); // mask all interrupts
    return true;
}

bool EnablePic(void) {
    outb(PIC1_DATA, pic1 = 0x00); // mask all interrupts
    outb(PIC2_DATA, pic2 = 0x00); // mask all interrupts
    return true;
}

bool DisableIrq(u8 irq) {
    if (irq >= 0x20 && irq <= 0x27) {
        outb(PIC1_DATA, pic1 |= (1 << (irq-0x20)));
    }
    if (irq >= 0x28 && irq <= 0x2F) {
        outb(PIC2_DATA, pic2 |= (1 << (irq-0x28)));
    }
    return true;
}

bool EnableIrq(u8 irq) {
    if (irq >= 0x20 && irq <= 0x27) {
        outb(PIC1_DATA, pic1 &= ~(1 << (irq-0x20)));
    }
    if (irq >= 0x28 && irq <= 0x2F) {
        outb(PIC2_DATA, pic2 &= ~(1 << (irq-0x28)));
    }
    return true;
}