#include <boot.h>
#include <fs.h>
#include <multiboot.h>
#include <inode.h>
#include <libk.h>
#include <vesa.h>

int read_memmap(multiboot_info *mboot) {
    if ((mboot->flags & MULTIBOOT_INFO_MEM_MAP) == 0) return 0;
    struct block_t *mm = (struct block_t*)(16*MB);
    char *e820 = (char*)mboot->mmap_addr;
    int size = mboot->mmap_length;
    for (int m = 0; size > 0; ) {
        multiboot_mmap_entry *map = (multiboot_mmap_entry*)e820;
        e820 += map->size;
        size -= map->size;
        if (map->type == 1) {
            mm[m].base = (uint32_t)map->addr;
            mm[m].size = (uint32_t)map->len;
        }
    }
    mmstart(mm);
    return 1;
}

int read_video(multiboot_info *mboot) {
    if ((mboot->flags & MULTIBOOT_INFO_VBE_INFO) == 0) return 0;
    VESA_INFO *vesa = (VESA_INFO*)mboot->vbe_control_info;
    MODE_INFO *mode = (MODE_INFO*)mboot->vbe_mode_info;
    vesa_start(vesa, mode);
    return 1;
}

int multiboot(int magic, multiboot_info *mboot, MBHDR *mbhdr) {
    if (!mboot || !mbhdr || magic != 0x2BADB002) return 0;
    if (!read_memmap(mboot)) return 0;
    if (!read_video(mboot)) return 0;
    if (!fsstart(0, mbhdr->initrd)) return 0;
    return 1;
}