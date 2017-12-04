#define _CRT_SECURE_NO_WARNINGS
#include <memory.h>
//#include <stdio.h>
#include <string.h>
#include "inode.h"

extern char *image;
extern volume_t *volume;
extern block_t  *blocks;
extern inode_t  *inodes;

_declspec(dllexport)
int fnext(int inode, char *buf, int len) {
    inode_t *root = &inodes[inode];
    int delta = 0;
    fnode_t *record;
    for (;;) {
        record = (fnode_t*)&image[root->base + delta];
        if (delta >= root->size) return -1;
        delta += record->length;
        if (strcmp(record->name, buf) == 0) break;
    }
    if (delta >= root->size) return -1;
    record = (fnode_t*)&image[root->base + delta];
    strncpy(buf, record->name, len);
    return record->inode;
}