#define IMPL_IVESA
#include <Kernel.h>

NOTE("************************************************************************")
NOTE("* VESA BIOS Extensions (VBE) Specification (https://goo.gl/3J5Wzm)     *")
NOTE("************************************************************************")

static CVesa *vesa;

void IVesa_Sync(void) {
    while ((inb(CGA_STATUS) & CGA_VSYNC) == 0);
}

void SysCall_Blit(int params[]) {
    if (!params) return;
    int x1 = params[0];
    int y1 = params[1];
    int x2 = params[2];
    int y2 = params[3];
    int u1 = params[4];
    int v1 = params[5];
    int u2 = params[6];
    int v2 = params[7];
    int sn = params[8];
    int tn = vesa->ScanLine;

    x1 = MIN(MAX(0, x1), vesa->ResX);
    y1 = MIN(MAX(0, y1), vesa->ResY);
    x2 = MIN(MAX(0, x2), vesa->ResX);
    y2 = MIN(MAX(0, y2), vesa->ResY);
    u1 = MIN(MAX(0, u1), vesa->ResX);
    v1 = MIN(MAX(0, v1), vesa->ResY);
    u2 = MIN(MAX(0, u2), vesa->ResX);
    v2 = MIN(MAX(0, v2), vesa->ResY);

    char *source = (char*)params[9];
    char *target = vesa->Pixels;
    target += y1*tn + x1*4;
    source += v1*sn + u1*4;
    int count = x2 - x1;

    for (int y = y1; y < y2; y++) {
        movsd(target, source, count);
        source += sn;
        target += tn;
    }
}

void IVesa_Blit(int params[]) {
    SysCall(SYS_BLIT,(int)params,0,0,0,0,0);
}

u16 IVesa_ResX(void) {
    return vesa->ResX;
}

u16 IVesa_ResY(void) {
    return vesa->ResY;
}

u16 IVesa_Bits(void) {
    return vesa->Bits;
}

u16 IVesa_ScanLine(void) {
    return vesa->ScanLine;
}

u8 *IVesa_Pixels(void) {
    return vesa->Pixels;
}

bool EnableVesa(u8 *base, u32 size) {
    if (!base) return false;
    vesa = (CVesa*)&base[sizeof(VESA_INFO)+sizeof(MODE_INFO)];
    return true;
}

IVesa ivesa = {
    IVesa_ResX,
    IVesa_ResY,
    IVesa_Bits,
    IVesa_ScanLine,
    IVesa_Pixels,
    IVesa_Blit,
    IVesa_Sync
};

export IVesa *Vesa = &ivesa;