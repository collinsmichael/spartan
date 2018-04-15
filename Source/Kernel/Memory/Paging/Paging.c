#define IMPL_IPAGING
#define IMPL_IMONITOR
#include <Kernel.h>

NOTE("************************************************************************")
NOTE("* Intel Architectures Reference Manual (https://goo.gl/46aNJU)         *")
NOTE("*                                                                      *")
NOTE("* Paging is a mechanism which accomodates memory protection & logical  *")
NOTE("* addressing. Each running process gets its own private address space. *")
NOTE("* The Page Directory maps logical addresses to physical addresses.     *")
NOTE("* Pages, Page Tables, and Page Directories must be aligned to a 4 KB,  *")
NOTE("* and are often 4KB in size.                                           *")
NOTE("*                                                                      *")
NOTE("* The Page Directory is a 4 KB array of pointers to Page Tables.       *")
NOTE("* Each Page Tables is a 4 KB array of pointers to 4 KB Pages.          *")
NOTE("* Positions in the Page Directory & Table define logical addresses     *")
NOTE("* Pointer held in the Page Tables define physical addresses.           *")
NOTE("* Low order bits are not part of the pointers and define privileges.   *")
NOTE("************************************************************************")

extern IMonitor *Monitor;
#define RING0_RWX   (0x0003)
#define RING3_RWX   (0x0007)

bool InstallPaging(u8 *base, u32 size) {
    NOTE("ring0 page directory has full visibility of the entire address space")
    NOTE("ring3 page directory has no visibility of most of the address space")
    NOTE("ring3 can see a locked down version of the kernels address space")

    u32 *tbl0 = (u32*)Monitor->GetBase('TBL0');
    u32 *tbl3 = (u32*)Monitor->GetBase('TBL3');
    stosd(tbl3, 0, 16*KB/4);
    for (int i = 0; i < 1024*1024; i++) {
        u32 page = i*4*KB;
        tbl0[i] = page | RING3_RWX;
        if (page < 16*MB) tbl3[i] = page | RING3_RWX;
    }

    u32 *dir0 = (u32*)base;
    u32 *dir3 = (u32*)&base[4*KB];
    for (int i = 0; i < 1024; i++) {
        u32 page = (u32)&tbl0[i*4*KB/4];
        dir0[i] = page | RING3_RWX;
    }

    stosd(dir3, 0, 4*KB/4);
    for (int i = 0; i < 4; i++) {
        u32 page = (u32)&tbl3[i*4*KB/4];
        dir3[i] = page | RING3_RWX;
    }
    return true;
}

bool EnablePaging(u8 *base, u32 size) {
    set_cr3(base);
    set_cr0(get_cr0() | CR0_PG | CR0_PE);
    return true;
}

bool IPaging_AppPages(CPcb *pcb) {
    if (!pcb) return false;
    CPeb *peb = pcb->Peb;
    if (!peb) return false;

    u32 *dir = peb->Pages;
    u32 *tbl0 = (u32*)Monitor->GetBase('TBL0');
    u32 *dir0 = (u32*)Monitor->GetBase('PAGE');
    stosd(dir, 0, 4*KB/4);
    movsd(dir, dir0, 8);

    u32 s = pcb->Binary / (4*MB);
    u32 f = (pcb->Binary + pcb->Extent) / (4*MB);
    for (u32 i = s; i < f; i++) {
        u32 page = dir0[i] & ~RING3_RWX;
        dir[i] = page | RING3_RWX;
    }
    return true;
}

IPaging paging = {
    IPaging_AppPages
};

export IPaging *Paging = &paging;