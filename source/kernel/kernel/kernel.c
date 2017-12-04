#include <arch.h>
#include <boot.h>
#include <libk.h>

_declspec(dllexport)
char *realloc(void) { return "force relocations"; }

_declspec(dllexport)
int _stdcall kernel(int magic, uint32_t *info[], MBHDR *mbhdr) {
    for (;;);
}

int _stdcall main(int magic, uint32_t *info[], MBHDR *mbhdr) {
    for (;;);
    return 1;
}