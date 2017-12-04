#define _CRT_SECURE_NO_WARNINGS
#include <memory.h>
#include <stdio.h>
#include <string.h>
#include "inode.h"

extern char *image;
extern volume_t *volume;
extern block_t  *blocks;
extern inode_t  *inodes;

typedef struct fnew_t {
    uint8_t inode;  // inode number
    uint8_t length; // length of this fnode record (always a multiple of 16)
    char name[254];
} fnew_t;

static
int _prepare(fnew_t *insert, char *path) {
    int length;
    memset(insert, 0, sizeof(fnew_t));
    for (length = 0; length < sizeof(insert->name); length++) {
        if (path[length] == 0) break;
        if (path[length] == '/') break;
        insert->name[length] = path[length];
    }
    insert->length = length + (sizeof(insert->length)+sizeof(insert->inode)+1); // strlen -> length of record
    align(insert->length, 16);   // paragraph aligned
    return length;
}

static
void _preamble(int inode, fnode_t *directory, fnode_t *record) {
    static fnode_t parent;
    static fnode_t child;
    parent.inode = directory->inode;
    child.inode = inode;
    parent.length = sizeof(fnode_t);
    child.length = sizeof(fnode_t);
    child.name[0] = parent.name[1] = parent.name[0] = '.';

    char *temp = &image[inodes[inode].base];
    fnode_t *create = (fnode_t*)temp;
    memcpy(temp, &child, sizeof(fnode_t));
    temp += sizeof(fnode_t);
    memcpy(temp, &parent, sizeof(fnode_t));
}

static
int _make(int inode, char *path, char *access) {
    // prepare record for insertion
    fnew_t insert;
    int length = _prepare(&insert, path);
    inode_t *root = &inodes[inode];
    fnode_t *directory = (fnode_t*)&image[root->base];
    for (int delta = 0;;) {
        fnode_t *record = (fnode_t*)&image[root->base + delta];
        if (delta >= root->size) {
            // insert a new inode and fnode
            int growth = directory->length + insert.length;

            char *mode = DIRECTORY;
            if (strlen(path) == length) {
                mode = access;
            }
            insert.inode = mknode(growth, mkflags(mode), user(), group());
            if (!insert.inode) return 0;
            inode_t *node = &inodes[insert.inode];
            if (node->flags & DIR_MASK) {
                _preamble(insert.inode, directory, record);
            } else node->size = 0;

            // expand if necessary
            if (delta + insert.length > root->capacity) {
                if (!renode(inode, root->capacity+PAGE_SIZE)) return 0;
                record = (fnode_t*)&image[root->base + delta];
                directory = (fnode_t*)&image[root->base];
            }

            // append new record to parent directory
            memcpy(record, &insert, insert.length);
            root->size += insert.length;
        }
        delta += record->length;
        if (strcmp(record->name, insert.name) == 0) {
            if (path[length] == '/' && path[length+1] != 0) {
                return _make(record->inode, &path[length+1], access);
            }
            return record->inode;
        }
    }
}

int mkdir(char *path) {
    return _make(0, path, DIRECTORY);
}

int mkbin(char *path) {
    return _make(0, path, BINARY);
}