#include <arch.h>
#include <boot.h>
#include <inode.h>
#include <libk.h>

_declspec(dllexport)
char *realloc(void) { return "force relocations"; }

typedef int (_stdcall *kmain)(int magic, uint32_t *info[], MBHDR *mbhdr);

int _stdcall main(int magic, uint32_t *info[], MBHDR *mbhdr) {
    kmain kernel = (kmain)((volume_t*)mbhdr->initrd)->entry;
    if (kernel) kernel(magic, info, mbhdr);
    for (;;);
    return 1;
}