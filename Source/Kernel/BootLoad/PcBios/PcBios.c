#define IMPL_PCBIOS
#define IMPL_IMONITOR
#include <Kernel.h>

extern IMonitor *Monitor;

// This is a c wrapper aroung the PcBios.asm binary which
// needs to exist in low mem (16-bit compatible).
// Only use this if you ***really*** know what you're doing
// Entering protected mode renders most of the BIOS inoperable.
void PcBios(int intno, BIOS *reg) {
    void *bios = Monitor->GetBase('BIOS');
    void (*x)(int,void*) = (void(*)(int,void*))bios;
    x(intno, reg);
}

bool InstallPcBios(u8 *base, u32 size) {
    if (!base || size < 320) return false;
    movsd(base, (char*)0x00100100, 320/4);
    return true;
}