/* ************************************************************************** */
/* author(s) : Michael Collins                                                */
/* file name : drivers/vesa/vesa.h                                            */
/* file date : 2017/12/03                                                     */
/* file info : vesa video mode definitions                                    */
/* ************************************************************************** */
#include <libk.h>
#include <boot.h>

_declspec(dllexport)
char *realloc(void) { return "force relocations"; }

int _stdcall main(int magic, uint32_t *info[], MBHDR *mbhdr) {
    for (;;);
    return 1;
}