#include <Kernel.h>

NOTE("************************************************************************")
NOTE("* (https://goo.gl/kX7eRJ) Multiboot Specification                      *")
NOTE("* (https://goo.gl/DqBiAM) PC-BIOS Memory Map Service 0x15 0xE820       *")
NOTE("* (https://goo.gl/3J5Wzm) VESA BIOS Extensions (VBE) Specification     *")
NOTE("************************************************************************")

extern IMonitor *Monitor;
extern IPool    *Pool;

bool IMultiBoot_MemMap(MB_INFO *grub) {
    if (!(grub->Flags & MBI_MEM_MAP)) return false;

    CPool *Heap = (CPool*)MMAP_HEAPPOOL;
    Pool->CreateMeta(Heap, 1*MB, 4*KB, 0, 0, "HEAP");
    Heap->Node = Monitor->GetBase('HEAP');
    if (!Heap->Node) return false;

    // memory is non existent unless discovered
    Heap->Count = 0;
    stosd(Heap->Node, '----', 1*MB/4);
    for (u32 pos = 0; pos  < grub->MmapLength;) {
        // parse the E820 memory map entry
        u8 *memmap = (u8*)grub->MmapAddr;
        E820 *e820 = (E820*)&memmap[pos];
        pos += e820->Next + sizeof(u32);

        // boundary checks 4 GB - 1 is maximum 32-bit addressable byte
        u64 buf = MIN((u64)4*GB-1, e820->Base);
        u64 end = MIN((u64)4*GB-1, e820->Base + e820->Size);

        // convert address to heap entry (each entry maps 4 KB)
        u32 mem = (((u32)buf + Heap->Gran-1) & ~(Heap->Gran-1)) / Heap->Gran;
        u32 len = (((u32)(end - buf) + Heap->Gran-1) & ~(Heap->Gran-1)) / Heap->Gran;

        // mark pages as necessary
        static u8 lookup[] = { '-', 'o', 'X', 'X', '-', '-' };
        u8 flag = MIN(e820->Type, elementsof(lookup)-1);
        stosb(&Heap->Node[mem], lookup[flag], len);
        Heap->Count += len;
    }

    // reserve kernel and shell memory
    Heap->Next = (u32)MMAP_USER / Heap->Gran;
    stosd(Heap->Node, 'XXXX', Heap->Next/4);
    Monitor->Protect('IVT ');
    return true;
}

bool IMultiBoot_Vesa(MB_INFO *grub) {
    if (!(grub->Flags & MBI_VBE_INFO)) return false;

    MODE_INFO *mod = (MODE_INFO*)grub->ModeInfo;
    if (!mod) return false;
    
    VESA_INFO *vbe = (VESA_INFO*)grub->VesaInfo;
    if (!vbe) return false;

    char *base = Monitor->GetBase('VESA');
    if (!base) return false;
    stosd(base, 'xxxx', 4*KB/4);
    movsd(base, vbe, sizeof(VESA_INFO)/4);
    movsd(&base[sizeof(VESA_INFO)], mod, sizeof(MODE_INFO)/4);

    CVesa *vesa = (CVesa*)&base[sizeof(VESA_INFO)+sizeof(MODE_INFO)];
    vesa->Mode     = grub->VbeMode;
    vesa->ResX     = mod->ResX;
    vesa->ResY     = mod->ResY;
    vesa->Bits     = mod->Bits;
    vesa->ScanLine = mod->BytesPerScanline;
    vesa->Pixels   = mod->PhysBasePtr;
    Logger(" Vesa.ResX     = %X\n", vesa->ResX);
    Logger(" Vesa.ResY     = %X\n", vesa->ResY);
    Logger(" Vesa.Bits     = %X\n", vesa->Bits);
    Logger(" Vesa.ScanLine = %X\n", vesa->ScanLine);
    Logger(" Vesa.Pixels   = %X\n", vesa->Pixels);
    Monitor->Protect('VESA');
    Logger(" Vesa.ResX     = %X\n", vesa->ResX);
    Logger(" Vesa.ResY     = %X\n", vesa->ResY);
    Logger(" Vesa.Bits     = %X\n", vesa->Bits);
    Logger(" Vesa.ScanLine = %X\n", vesa->ScanLine);
    Logger(" Vesa.Pixels   = %X\n", vesa->Pixels);
    return true;
}

bool MultiBoot(void *info) {
    Monitor->EnableErrorRecovery();
    if (!IMultiBoot_MemMap(info)) return false;
    if (!IMultiBoot_Vesa(info)) return false;
  //Monitor->ClearMemory();
    return true;
}