#include <arch.h>

#define GDT ((uint64_t*)(GDT_BASE+0x0000))

static uint64_t gdt[] = {
    0x0000000000000000, // 0x00 null
    0x00CF9A000000FFFF, // 0x10 ring0 code
    0x00CF92000000FFFF, // 0x18 ring0 data
    0x00DFFA000000FFFF, // 0x20 ring3 code
    0x00DFF2000000FFFF, // 0x28 ring3 data
    0x0000E9000000FFFF  // 0x38 ring3 task
    +((int)TSS_BASE*65536)
};

void gdt_reset(void) {
    memcpy(GDT, gdt, sizeof(gdt));
    lgdt(GDT, sizeof(gdt)-1);
}