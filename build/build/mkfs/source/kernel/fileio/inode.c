#define _CRT_SECURE_NO_WARNINGS
#include <memory.h>
#include <stdio.h>
#include "inode.h"

extern char *image;
extern volume_t *volume;
extern block_t  *blocks;
extern inode_t  *inodes;

/* ************************************************************************** */
/* fill inode record with given details                                       */
/* ************************************************************************** */
static
int donode(int inode, int base, int size, int flags, int uid, int gid) {
    inodes[inode].base     = base;
    inodes[inode].size     = size;
    inodes[inode].capacity = size;
    inodes[inode].flags    = flags;
    inodes[inode].uid      = uid;
    inodes[inode].gid      = gid;
    //inodes[inode].pid      = 0;
    //inodes[inode].locked   = 0;
    align(inodes[inode].capacity, PAGE_SIZE);
    return inode;
}

/* ************************************************************************** */
/* create a new inode for a block of a given size                             */
/* ************************************************************************** */
int mknode(int len, int flag, int uid, int gid) {
    int base;
    int inode;
    for (inode = 0; inodes[inode].capacity > 0; inode++) {
        if (inode >= volume->max_nodes ) return 0;
    }
    base = dalloc(len);
    if (base == 0) return 0;
    memset(&inodes[inode], 0, sizeof(inode_t));
    donode(inode, base, len, flag, uid, gid);
    memset(&image[base], 0, inodes[inode].capacity);
    volume->node_count++;
    return inode;
}

/* ************************************************************************** */
/* delete an existing inode record for a given inode                          */
/* ************************************************************************** */
int delnode(int inode) {
    dfree(inodes[inode].base, inodes[inode].size);
    memset(&inodes[inode], 0, sizeof(inode_t));
    volume->node_count--;
    return 0;
}

/* ************************************************************************** */
/* resize the block assigned to a given inode                                 */
/* ************************************************************************** */
int renode(int inode, int size) {
    // try to grow the current disk block
    int delta = size - inodes[inode].capacity;
    int start = inodes[inode].base + inodes[inode].capacity;
    align(delta, PAGE_SIZE);
	struct block_t *block;
    for (block = blocks; block->base < start && block->size != 0; block++);
    if (block->base == start && block->size >= delta) {
        inodes[inode].capacity = size;
        block->base += delta;
        block->size -= delta;
        while (block->size == 0) {
            block->base = (block+1)->base;
            block->size = (block+1)->size;
            block++;
        }
        volume->disk_free -= delta;
        volume->disk_used += delta;
        return inode;
    }

    int base = dalloc(size);
    if (base == 0) return 0;

    memset(&image[base], 0, size);
    memcpy(&image[base], &image[inodes[inode].base], inodes[inode].size);
    dfree(inodes[inode].base, inodes[inode].size);
    memset(&image[inodes[inode].base], 0, inodes[inode].size);

    inodes[inode].base = base;
    inodes[inode].capacity = size;
    align(inodes[inode].capacity, PAGE_SIZE);
    return inode;
}

/* ************************************************************************** */
/* defragment the disk                                                        */
/* ************************************************************************** */
int stats(void) {
	struct block_t *block;
    printf(" base     size\n");
	for (block = blocks; block->size != 0; block++) {
        printf(" %08X %08X\n", block->base, block->size);
    }
    printf(" inode base     size     capacity\n");
    for (int i = 0; i < volume->node_count; i++) {
        printf(" %02X    %08X %08X %08X\n",
            i, inodes[i].base, inodes[i].size, inodes[i].capacity);
    }
    return 1;
}