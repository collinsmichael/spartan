#include <Kernel.h>

NOTE("************************************************************************")
NOTE("* Intel Architectures Reference Manual (https://goo.gl/46aNJU)         *")
NOTE("************************************************************************")

extern IMonitor *Monitor;
extern IDevice  *Device;

char master[64] = // Source/Kernel/BootLoad/Intel/Isr.asm
"\xFA\x90\x60\x1E\x0F\x20\xDA\x52\x6A\x10\x1F\xBA\x00\x20\x00\x00"
"\x0F\x22\xDA\x54\x6A\x78\xFF\x15\x78\x78\x78\x78\x94\xB0\x20\xE6"
"\x20\x58\x0F\x22\xD8\x1F\x61\xCF\x78\x78\x78\x78\x78\x78\x78\x78"
"\x78\x78\x78\x78\x78\x78\x78\x78\x78\x78\x78\x78\x78\x78\x78\x78";

char slave[64] = // Source/Kernel/BootLoad/Intel/Isr.asm
"\xFA\x90\x60\x1E\x0F\x20\xDA\x52\x6A\x10\x1F\xBA\x00\x20\x00\x00"
"\x0F\x22\xDA\x54\x6A\x78\xFF\x15\x78\x78\x78\x78\x94\xB0\x20\xE6"
"\xA0\xE6\x20\x58\x0F\x22\xD8\x1F\x61\xCF\x78\x78\x78\x78\x78\x78"
"\x78\x78\x78\x78\x78\x78\x78\x78\x78\x78\x78\x78\x78\x78\x78\x78";

char except[64] = // Source/Kernel/BootLoad/Intel/Isr.asm
"\xFA\x90\x60\x1E\x0F\x20\xDA\x52\x6A\x10\x1F\xBA\x00\x20\x00\x00"
"\x0F\x22\xDA\x54\xFF\x74\x24\x2C\xFF\x15\x78\x78\x78\x78\x94\xB0"
"\x20\xE6\x20\x58\x0F\x22\xD8\x1F\x61\x8F\x04\x24\xCF\x78\x78\x78"
"\x78\x78\x78\x78\x78\x78\x78\x78\x78\x78\x78\x78\x78\x78\x78\x78";

u8 fixup[] =
"mmmmmmmmxmxxxxxmmxmmmmmmmmmmmmmmmmmmmmmmssssssssmmmmmmmmmmmmmmmm";

u32 DefaultIsrStub(u32 err, u32 esp) {
    u32 *ret = (u32*)&err;
    u32 eip = ret[-1];
    u32 irq = (eip-0x8000) / 64;
    Logger("ISR 0x%X (eip=%X esp=%X err=%X)\n", irq, eip, esp, err);
    return esp;
}

bool InstallIrq(u8 *base, u32 size) {
    u32 *irq = (u32*)base;
    for (int i = 0; i < 256; i++) {
        irq[i] = (u32)DefaultIsrStub;
    }
    return true;
}

bool InstallIsr(u8 *base, u32 size) {
    u32 *irq = (u32*)Monitor->GetBase('IRQ ');
    for (int i = 0; i < 256; i++) {
        char *isr = &base[i*64];
        stosb(isr, i, 64);

        u8 d = fixup[MIN(i, elementsof(fixup)-2)];
        switch (d) {
        case 'm': movsd(isr, master, 0x40/4); break;
        case 's': movsd(isr, slave,  0x40/4); break;
        case 'x': movsd(isr, except, 0x40/4); break;
        }

        u32 *call = (u32*)&isr[(d == 'x') ? 0x1A : 0x18];
        *call = (u32)&irq[i];
    }
    return true;
}

bool InstallIdt(u8 *base, u32 size) {
    u64 *idt = (u64*)base;
    u8 *isr = Monitor->GetBase('ISR ');
    for (int i = 0; i < 256; i++) {
        idt[i] = 0x8E0000080000 + (u32)&isr[i*64];
        if (i >= 128) idt[i] |= (u64)3 << 45;
    }

    u32 *idtr = (u32*)&base[256*8];
    idtr[0] = (256*8 - 1) << 16;
    idtr[1] = (u32)base;

    u32 *ivtr = (u32*)&base[256*8 + 16];
    ivtr[0] = 0x3FF << 16;
    ivtr[1] = 0x00000000;
    return true;
}

bool EnableIdt(u8 *base, u32 size) {
    u32 *idtr = (u32*)&base[256*8+2];
    lidt(idtr);
    return true;
}