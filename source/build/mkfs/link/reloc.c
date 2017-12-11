#define _CRT_SECURE_NO_WARNINGS
#include <memory.h>
#include <stdio.h>
#include <string.h>
#include <inode.h>
#include <exe.h>

extern char *boot;
extern char *image;
extern volume_t *volume;
extern block_t  *blocks;
extern inode_t  *inodes;

int reloc(char *path, int base, int size) {
    //printf("[reloc] %s\n", path);

    char *binary = &image[base];
    mzhdr_t *mzhdr = (mzhdr_t*)binary;
    pehdr_t *pehdr = (pehdr_t*)&binary[mzhdr->e_lfanew];
    ophdr_t *ophdr = (ophdr_t*)&binary[mzhdr->e_lfanew + sizeof(pehdr_t)];

    pedir_t *dir = (pedir_t*)&binary[mzhdr->e_lfanew + sizeof(pehdr_t) + sizeof(ophdr_t)];
    pereloc_t *reloc = (pereloc_t*)&binary[dir->reloc_address];
    int length = dir->reloc_size;
    if (length <= 0) return 0 & printf("error no relocation data (need to relocate %s)\n", path);

    int target = base;
    int source = ophdr->imagebase;
    if (target == source) return 1;
    int delta = target - source;

    while (length > 0) {
        char *start = (char*)reloc;
        int   count = (reloc->blocksize-sizeof(pereloc_t))/sizeof(uint16_t);
        int   page = reloc->address;
        uint16_t *list = (uint16_t*)&start[sizeof(pereloc_t)];
        while (count) {
            int *fixup;
            count--;
            uint16_t type = (list[0] & 0xF000) >> 12;
            uint16_t offset = list[0] & 0x0FFF;
            uint32_t rva = page + offset;
            if (type == 3) {
                fixup = (int*)&binary[rva];
                *fixup += delta;
            }
            list++;
        }
        length -= reloc->blocksize;
        reloc = (pereloc_t*)&start[reloc->blocksize];
    }
    //if (strstr(path, "kernel.dll")) volume->kernel = target;
    ophdr->imagebase = target;
    if (strcmp(path, boot) == 0) {
        volume->entry = ophdr->imagebase + ophdr->entry;
    }
    return 1;
}

int _relocate(char *root) {
    char fname[256];
    char path[512];
    memset(fname, 0, sizeof(fname));
    int parent = ffind(root, fname, sizeof(fname));
    if (parent < 0) return 0;
    strcpy(fname, "..");
    for (;;) {
        int child = fnext(parent, fname, sizeof(fname));
        if (child <= 0) return 0;

        memset(path, 0, sizeof(path));
        sprintf(path, "%s/%s", root, fname);

        inode_t *inode = &inodes[child];
        if (inode->flags & DIR_MASK) {
            _relocate(path);
        } else if (strstr(fname, ".exe") || strstr(fname, ".dll")) {
            reloc(path, inode->base, inode->size);
        }
    }
    return 1;
}

int relocate(void) {
    char root[256];
    memset(root, 0, sizeof(root));
    int parent = ffind("", root, sizeof(root));
    if (parent < 0) return 0;
    memset(root, 0, sizeof(root));
    strcpy(root, "..");
    for (;;) {
        int child = fnext(parent, root, sizeof(root));
        if (child <= 0) break;
        inode_t *inode = &inodes[child];
        if (inode->flags & DIR_MASK) _relocate(root);
    }
    return 1;
}