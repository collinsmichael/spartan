#define PIC1      0x20
#define PIC2      0xA0
#define PIC1_DATA 0x21
#define PIC2_DATA 0xA1
#define EOI       0x20

/*
void pic_remap(void) { // remap PIC IRQs
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
    sti();
}
*/
/*
// cannot chain IRQ handlers
_declspec(dllexport)
int idt_insert(int irq, void (*isr)(void)) {
    if (irq < 0 || irq > 255) return 0;      // invalid IRQ number
    if (!isr) return 0;                      // invalid handler
    if (ISR[irq].jump > 1024*1024) return 0; // handler already installed
    ISR[irq].jump = isr;
    return 1;
}

// cannot remove IRQ handlers unless you know its physical address
_declspec(dllexport)
int idt_remove(int irq, void (*isr)(void)) {
    if (irq < 0 || irq > 255) return 0; // invalid IRQ number
    if (isr != ISR[irq].jump) return 0; // handler not installed
    ISR[irq].jump = &ISR[irq].dummy;
    return 1;
}

#define ISR0 "\xFE\x05\x00\x00\x00\x00\xB0\x20\xE6\x20\xCF\x00\x00\x00\x00\x00"\
             "\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00"
#define ISR1 "\xFE\x05\x00\x00\x00\x00\xB0\x20\xE6\xA0\xE6\x20\xCF\x00\x00\x00"\
             "\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00"
#define ISR2 "\xFE\x05\x00\x00\x00\x00\xE6\x60\xB0\x20\xE6\x20\xCF\x00\x00\x00"\
             "\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00"

int idt_init(void) {
#if (0)
    for (int i = 0; i < 256; i++) {
        char *src = (i >= 0x28 && i < 0x30) ? ISR1 : ISR0;
        uint32_t trg = (uint32_t)&ISR[i];
        memcpy(&ISR[i], src, 32);
        ISR[i].call += (uint32_t)trg;
        ISR[i].jump += (uint32_t)trg;
    }
#else
    for (int i = 0; i < 256; i++) {
        char *src = (i >= 0x28 && i < 0x30)
                ? ISR1 : (i == 0x21) ? ISR2 : ISR0;
        uint32_t trg = (uint32_t)&ISR[i];
        memcpy(&ISR[i], src, 32);
        char *isr = &ISR[i];
        int *pixel = &isr[2];
        *pixel = 0x000B814A + (i/16)*160 + (i%16)*4;
    }
#endif

    // install the above handlers in the IDT
    for (int i = 0; i < 256; i++) {
        uint32_t trg = (uint32_t)&ISR[i];
        IDT[i] = 0x00008E0000100000 + trg;
    }

    // fixup IRQs which generate error codes
#if (0)
    IDT[0x08]++; // DOUBLE FAULT
    IDT[0x0A]++; // INVALID TSS
    IDT[0x0B]++; // SEGMENT NOT PRESENT
    IDT[0x0C]++; // STACK SEGMENT FAULT
    IDT[0x0D]++; // GENERAL PROTECTION
    IDT[0x0E]++; // PAGE FAULT
    IDT[0x11]++; // ALIGNMENT CHECK
#endif

    lidt(IDTR);
    sti();
    return 1;
}
*/