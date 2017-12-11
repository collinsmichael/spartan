#include <arch.h>

void tss_reset(void) {
    memset(TSS_BASE,0,128);
    TSS_BASE->cr3    = (uint32_t)PGTBL;
    TSS_BASE->ss0    = GDT_RING0_DATA;
    TSS_BASE->esp0   = 0;
    TSS_BASE->eflags = 0x00003200;
    ltr(TSS_RING3_TASK);
} 