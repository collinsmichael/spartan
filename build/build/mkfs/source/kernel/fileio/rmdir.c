#define _CRT_SECURE_NO_WARNINGS
#include <memory.h>
#include <stdio.h>
#include <string.h>
#include "inode.h"

extern char *image;
extern volume_t *volume;
extern block_t  *blocks;
extern inode_t  *inodes;

static
int _parentof(char *path, char *name, int size) {
    char buf[256];
    strncpy(buf, path, sizeof(buf)-1);
    for (char *temp = buf;;) {
        char *scan = strchr(temp+1, '/');
        if (scan) temp = scan;
        else {
            strncpy(name, ++temp, size);
            *temp = 0;
            break;
        }
    }
    if (!buf[0]) return ~0;
    return ffind(buf, buf, sizeof(buf));
}

int rmbin(char *path) {
    char fname[256];
    int child = ffind(path, fname, sizeof(fname));
    if (!child) return 0;
    int parent = _parentof(path, fname, sizeof(fname));
    if (parent == ~0) return 0;

    inode_t *root = &inodes[parent];
    for (int delta = 0;;) {
        fnode_t *record = (fnode_t*)&image[root->base + delta];
        if (delta >= root->size) return 0;
        delta += record->length;
        if (strcmp(record->name, fname) == 0) {
            /* lock(parent); */
            /* lock(child); */
            char *source = &image[root->base + delta];
            char *target = (char*)record;
            int count = root->size - delta;
            int length = record->length;
            memcpy(target, source, count);
            memset(&source[count-length], 0, length);
            root->size -= length;
            // shrink block if necessary
            if (root->size < root->capacity - 2*PAGE_SIZE) {
                renode(parent, root->capacity - PAGE_SIZE);
            }
            delnode(child);
            /* unlock(child); */
            /* unlock(parent); */
            return 1;
        }
    }
    return 0;
}

int rmdir(char *path) {
    return rmbin(path);
}