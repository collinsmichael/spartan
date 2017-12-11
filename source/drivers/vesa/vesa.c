/* ************************************************************************** */
/* author(s) : Michael Collins                                                */
/* file name : drivers/vesa/vesa.h                                            */
/* file date : 2017/12/03                                                     */
/* file info : vesa video mode definitions                                    */
/* ************************************************************************** */
#include <boot.h>
#include <multiboot.h>
#include <libk.h>
#include <limits.h>
#include <pipe.h>
#include <vesa-def.h>

static VESA_INFO *vi;
static MODE_INFO *mi;

_declspec(dllexport)
MODE_INFO *GetModeInfo(void) {
    return mi;
}

_declspec(dllexport)
VESA_INFO *GetVesaInfo(void) {
    return vi;
}

_declspec(dllexport)
void vesa_start(VESA_INFO *vesa, MODE_INFO *mode) {
    vi = vesa;
    mi = mode;
}

int _stdcall main(int magic, multiboot_info *mboot, MBHDR *mbhdr) {
    if (mboot) vi = (VESA_INFO*)mboot->vbe_control_info;
    if (mboot) mi = (MODE_INFO*)mboot->vbe_mode_info;
    return 1;
}