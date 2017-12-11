#include <arch.h>
#include <boot.h>
#include <cga.h>
#include <vesa.h>

extern int m;
extern int mx;
extern int my;
extern int key;
static int ox;
static int oy;

void vbe(MODE_INFO *mode) {
    uint8_t *frame = (uint8_t*)mode->PhysBasePtr;

    int s = 8;
    int nx = mx;
    int ny = my;
    if (nx < 0) nx = 0; else if (nx >= mode->ResX) nx = mode->ResX-1;
    if (ny < 0) ny = 0; else if (ny >= mode->ResY) ny = mode->ResY-1;
    while ((inb(CGA_STATUS) & CGA_VSYNC) == 0);
    for (int y = oy-s; y < oy+s; y++) {
        if (y < 0 || y >= mode->ResY) continue;
        uint8_t *screen = (uint8_t*)&frame[y*mode->BytesPerScanline];
        for (int x = ox-s; x < ox+s; x++) {
            if (x < 0 || x >= mode->ResX) continue;
            *(uint32_t*)&screen[x*mode->Bits/8] = 0;
        }
    }
    ox = nx;
    oy = ny;
    uint32_t colour = key*0x01010101;
    colour += m*0x01010101;
    for (int y = ny-s; y < ny+s; y++) {
        if (y < 0 || y >= mode->ResY) continue;
        uint8_t *screen = (uint8_t*)&frame[y*mode->BytesPerScanline];
        for (int x = nx-s; x < nx+s; x++) {
            if (x < 0 || x >= mode->ResX) continue;
            *(uint32_t*)&screen[x*mode->Bits/8] = colour;
        }
    }
}

int test(int magic, BOOT_INFO *mboot, MBHDR *mbhdr) {
    MODE_INFO *mode = (MODE_INFO*)mboot->ModeInfo;
    ox = mx = mode->ResX/2;
    oy = my = mode->ResY/2;

    for (;;) vbe(mode);
    return 1;
}