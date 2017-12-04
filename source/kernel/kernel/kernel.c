#include <arch.h>
#include <boot.h>
#include <libk.h>
#include <memory.h>
#include <multiboot.h>
#include <vesa.h>

_declspec(dllexport)
char *realloc(void) { return "force relocations"; }

int _stdcall main(int magic, multiboot_info *mboot, MBHDR *mbhdr) {
    if (mboot->vbe_mode_info) {
        modeinfo_t *mode = (modeinfo_t*)mboot->vbe_mode_info;
        void    *frame = (void*)mode->physbaseptr;
        uint32_t resx  = mode->resx;
        uint32_t resy  = mode->resy;
        uint32_t bits  = mode->bits;
        memset(frame, 0xFFFFFFFF, resx*resy*bits/8);
    }
    return 1;
}