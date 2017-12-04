#define _CRT_SECURE_NO_WARNINGS
#include <inode.h>
#include <libk.h>
#include <malloc.h>
#include <memory.h>
#include <stdio.h>
#include <string.h>

extern char     *image;
extern volume_t *volume;
extern block_t  *blocks;
extern inode_t  *inodes;

int renode(int inode, int size);
int ffind(char *path, char *buf, int len);
int mkbin(char *path);

_declspec(dllexport)
XFILE *xopen(char *path, char *access) {
    char buf[256];

    if (!path || !access) return 0;
    XFILE *file = (XFILE*)kalloc(sizeof(XFILE));
    if (!file) return 0;
    file->head = 0;
    file->tail = 0;

    file->inode = (strchr(access, 'w') || strchr(access, 'a'))
        ? mkbin(path)
        : ffind(path, buf, sizeof(buf));
    if (!file->inode) return 0;
    inode_t *node = &inodes[file->inode];
    file->ring = &image[node->base];
    return file;
}

_declspec(dllexport)
int xclose(XFILE *file) {
    if (!file) return 0;
    kfree(file, sizeof(XFILE));
    return 1;
}

_declspec(dllexport)
int xsize(XFILE *file) {
    if (!file) return 0;
    inode_t *node = &inodes[file->inode];
    return node->size;
}

_declspec(dllexport)
int xseek(XFILE *file, int offset, int method) {
    if (!file) return 0;
    inode_t *node = &inodes[file->inode];
    if (method == SEEK_END) file->head = node->size;
    else if (method == SEEK_SET) file->head = 0;
    else if (method != SEEK_CUR) return 0;

    file->head += offset;
    if (file->head > node->size) file->head = node->size;
    else if (file->head < 0) file->head = 0;
    return file->head;
}

_declspec(dllexport)
int xgetc(XFILE *file) {
    if (!file) return 0;
    inode_t *node = &inodes[file->inode];
    char *data = &image[node->base];
    if (file->head >= node->size) return EOF;
    char c = data[file->head++];
    return c;
}

_declspec(dllexport)
int xputc(int ch, XFILE *file) {
    if (!file) return 0;
    inode_t *node = &inodes[file->inode];
    char *data = &image[node->base];
    data[file->head++] = ch;
    if (++node->size >= node->capacity) {
        int capacity = node->capacity + PAGE_SIZE;
        if (!renode(file->inode, capacity)) return -1;
    }
    return ch;
}