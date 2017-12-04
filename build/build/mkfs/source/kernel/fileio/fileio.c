#define _CRT_SECURE_NO_WARNINGS
#include <malloc.h>
#include <memory.h>
#include <stdio.h>
#include <string.h>
#include "inode.h"

extern char *image;
extern volume_t *volume;
extern block_t  *blocks;
extern inode_t  *inodes;

XFILE *xopen(char *path, char *access) {
    char buf[256];

    if (!path || !access) return 0;
    XFILE *file = (XFILE*)malloc(sizeof(XFILE));
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

int xclose(XFILE *file) {
    if (!file) return 0;
    free(file);
    return 1;
}

int xsize(XFILE *file) {
    if (!file) return 0;
    inode_t *node = &inodes[file->inode];
    return node->size;
}

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

int xgetc(XFILE *file) {
    if (!file) return 0;
    inode_t *node = &inodes[file->inode];
    char *data = &image[node->base];
    if (file->head >= node->size) return EOF;
    char c = data[file->head++];
    return c;
}

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