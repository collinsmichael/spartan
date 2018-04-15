#include <Kernel.h>

NOTE("************************************************************************")
NOTE("* Intel Architectures Reference Manual (https://goo.gl/46aNJU)         *")
NOTE("*                                                                      *")
NOTE("* the Global Descriptor Table defines a memory map                     *")
NOTE("* entries can be:                                                      *")
NOTE("*   o read / write / execute                                           *")
NOTE("*   o ring 0 / ring 1 / ring 2 / ring 3 (kernel mode or user mode)     *")
NOTE("*   o 16-Bit Real Mode / 32-Bit Protected Mode / 64-Bit Long Mode      *")
NOTE("*   o task state segments (stores context during context switch)       *")
NOTE("*                                                                      *")
NOTE("* Jargon: Ring 0 tasks are the most privileged (kernel mode)           *")
NOTE("* Jargon: Ring 3 tasks are the least privileged (user mode)            *")
NOTE("* Jargon: Real Mode is a mode of execution that uses 16-bit addressing *")
NOTE("* Jargon: Protected Mode uses 32-bit addressing                        *")
NOTE("* Jargon: Long Mode uses 64-bit addressing                             *")
NOTE("************************************************************************")

u64 gdt[] = {
    0x0000000000000000, // 0x00 null
    0x00CF9A000000FFFF, // 0x08 ring0 code
    0x00CF92000000FFFF, // 0x10 ring0 data
    0x00DFFA000000FFFF, // 0x18 ring3 code
    0x00DFF2000000FFFF, // 0x20 ring3 data
    0x000F98000000FFFF, // 0x28 rm16 code
    0x000F92000000FFFF, // 0x30 rm16 data
    0x0000E9000000FFFF  // 0x38 ring3 task
};

bool InstallGdt(u8 *base, u32 size) {
    u32 tss = (u32)base + size;
    gdt[7] |= tss*65536;
    movsd(base, gdt, sizeof(gdt)/4);

    u32 *gdtr = (u32*)&base[sizeof(gdt)];
    gdtr[0] = (sizeof(gdt)-1) << 16;
    gdtr[1] = (u32)base;
    return true;
}

bool EnableGdt(u8 *base, u32 size) {
    lgdt(&base[sizeof(gdt)+2]);
    return true;
}