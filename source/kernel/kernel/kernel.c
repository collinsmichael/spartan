#include <arch.h>
#include <boot.h>
#include <libk.h>
#include <memory.h>
#include <multiboot.h>
#include <vesa.h>

_declspec(dllexport)
char *realloc(void) { return "force relocations"; }

#include <pipe.h>
uint32_t t;
void dummy(void) {
    static PIPE fd = { 0,0,1024*768*32/8,(uint8_t*)0x01000000 };
    uint32_t *frame = (uint32_t*)fd.ring;
    MODE_INFO *mode = GetModeInfo();
    for (int y = 0; y < mode->ResY; y++)
    for (int x = 0; x < mode->ResX; x++) {
        uint8_t c = (x ^ y) + t;
        frame[y*mode->ResX + x] = c*0x01010101;
    }
    t++;
    flush(&fd);
}

int multiboot(int magic, multiboot_info *mboot, MBHDR *mbhdr);

int _stdcall main(int magic, multiboot_info *mboot, MBHDR *mbhdr) {
    //if (!arch_init()) for (;;);
    if (!multiboot(magic, mboot, mbhdr)) for (;;);
    for (;;) dummy();
    return 1;
}