#include <arch.h>
#include <boot.h>
#include <libk.h>

_declspec(dllexport)
int arch_init(void) {
    //if (!gdt_init()) return 0;
    //if (!idt_init()) return 0;
    //if (!tss_init()) return 0;
    return 1;
}

int _stdcall main(int magic, uint32_t *info[], MBHDR *mbhdr) {
    for (;;);
    return 1;
}