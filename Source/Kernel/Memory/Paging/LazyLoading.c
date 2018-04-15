#include <Kernel.h>

void LazyLoading(CPcb *pcb, CPeb *peb, int *table) {
    // clear the page dir (implements lazy loading)
    stosd(peb->Pages, 0, 4*KB/4);
    set_cr3(peb->Pages);

    // anticipate that the entry point will soon be needed
    int entry = pcb->AppMain;
    int dir = (((unsigned int)entry >> 22) % KB)/sizeof(void*);
    int tbl = (((unsigned int)entry >> 12) % KB)/sizeof(void*);
    peb->Pages[dir] = (int)table;
    stosd(table, 0, 4*KB);
}