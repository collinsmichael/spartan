#define _CRT_SECURE_NO_WARNINGS
#include <memory.h>
#include <string.h>
#include <inode.h>

extern char *image;
extern volume_t *volume;
extern block_t  *blocks;
extern inode_t  *inodes;

static
int _ffind(int inode, char *path, char *buf, int len) {
    char match[256];
    int length;
    for (length = 0; path[length]; length++) {
        if (path[length] == '/') break;
        match[length] = path[length];
    }
    match[length] = 0;

    inode_t *root = &inodes[inode];
    fnode_t *first = 0;
    int delta = 0;
    for (;;) {
        fnode_t *record = (fnode_t*)&image[root->base + delta];
        if (delta >= root->size) return 0;
        delta += record->length;
        if (strcmp(record->name, "..") == 0) {
            first = (fnode_t*)&image[root->base + delta];
            if (length == 0) {
                strncpy(buf, first->name, len);
                return inode;
            }
        }
        if (strcmp(record->name, match) == 0) {
            inode_t *node = &inodes[record->inode];
            if (node->flags & DIR_MASK) {
                return _ffind(record->inode, &path[length+1], buf, len);
            }
            return record->inode;
        }
    }
}

_declspec(dllexport)
int ffind(char *path, char *buf, int len) {
    return _ffind(0, path, buf, len);
}