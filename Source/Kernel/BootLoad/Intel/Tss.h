#ifndef KERNEL_BOOTLOAD_INTEL_TSS_H
#define KERNEL_BOOTLOAD_INTEL_TSS_H

// structure of a TSS
typedef struct CTss {
    int BackLink;
    int esp0;
    int ss0;
    int esp1;
    int ss1;
    int esp2;
    int ss2;
    int cr3;
    int eip;
    int eflags;
    int eax;
    int ecx;
    int edx;
    int ebx;
    int esp;
    int ebp;
    int esi;
    int edi;
    int es;
    int cs;
    int ss;
    int ds;
    int fs;
    int gs;
    int ldt;
    int pio;
} CTss;

#endif KERNEL_BOOTLOAD_INTEL_TSS_H