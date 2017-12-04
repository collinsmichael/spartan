#include <arch.h>
#include <boot.h>
#include <inode.h>
#include <libk.h>
#include <multiboot.h>

_declspec(dllexport)
char *realloc(void) { return "force relocations"; }

typedef int (_stdcall *kmain)(int magic, multiboot_info *info, MBHDR *mbhdr);

int _stdcall main(int magic, multiboot_info *info, MBHDR *mbhdr) {
    kmain kernel = (kmain)((volume_t*)mbhdr->initrd)->entry;
    if (kernel) kernel(magic, info, mbhdr);
    for (;;);
    return 1;
}