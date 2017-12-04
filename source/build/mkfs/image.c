#define _CRT_SECURE_NO_WARNINGS
#include <memory.h>
#include <string.h>
#include <inode.h>
#include "mkfs.h"

extern int imagebase;
char ram[64*MB];
char *image = ram;
volume_t *volume;
block_t  *blocks;
inode_t  *inodes;

void init_image(void) {
    volume = (volume_t*)&ram[imagebase];
    blocks = (block_t*)&ram[imagebase + sizeof(volume_t)];
    inodes = (inode_t*)&ram[imagebase + sizeof(volume_t) + sizeof(block_t)*MAX_BLOCKS];
    memset(image, 0, sizeof(image));

    volume->blocks    = (block_t*)(imagebase + sizeof(volume_t));
    volume->inodes    = (inode_t*)(imagebase + sizeof(volume_t) + sizeof(block_t)*MAX_BLOCKS);
    volume->root      = (fnode_t*)FIRST;
    volume->entry     = 0;
    volume->imagebase = imagebase;

    volume->max_blocks = MAX_BLOCKS;
    volume->max_nodes  = MAX_NODES;
    volume->node_count = 0;

    volume->disk_space = DISK_SPACE;
    volume->disk_used  = SYS_NODES;
    volume->disk_free  = DISK_SPACE-SYS_NODES;

    strcpy(volume->label,     "OP SYS");
    strcpy(volume->author,    "MICHAEL COLLINS (2017)");
    strcpy(volume->publisher, "LIMERICK INSTITUTE OF TECHNOLOGY");
    blocks[0].base = FIRST;
    blocks[0].size = FINAL - FIRST;

    fsstart(ram, imagebase);
}