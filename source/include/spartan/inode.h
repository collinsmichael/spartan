#ifndef INODE_H
#define INODE_H
#include <stddef.h>
#include <stdint.h>
#pragma pack(push, 1)

#define elementsof(a) (sizeof(a) / sizeof(a[0]))
#define align(a,b) do { a = (((int)a)+((int)b)-1) & ~(((int)b)-1); } while (0)

#define KB (1024)
#define MB (1024*KB)
#define GB (1024*MB)
#define PAGE_SIZE 256
#define DIRECTORY "vlrwxlr--l---" //("vlcdrw-l--rw-l--r--")
#define BINARY    "-lrwxlr-xl--x" //("-l-drw-l--rw-l--r--")
#define DIR_MASK  0x1000

#define MAX_NODES  256
#define MAX_BLOCKS 224
#define SYS_NODES  (sizeof(volume_t) + sizeof(inode_t)*MAX_NODES + sizeof(block_t)*MAX_BLOCKS)
#define FIRST      (imagebase + SYS_NODES)
#define FINAL      (FIRST + 15*MB)
#define DISK_SPACE (FINAL-FIRST)


typedef struct block_t {
    uint32_t base;
    uint32_t size;
} block_t;

typedef struct inode_t {
    int base;
    int size;
    int capacity;
    uint16_t flags;
    uint8_t  uid;
    uint8_t  gid;
} inode_t;

typedef struct fnode_t {
    uint8_t inode;    // inode number
    uint8_t length;   // length of this fnode record (always a multiple of 16)
    char name[14]; // actual length is length-8
} fnode_t;

typedef struct volume_t {
    block_t *blocks;
    inode_t *inodes;
    fnode_t *root;
    int entry;

    int disk_space;
    int disk_used;
    int disk_free;
    int max_blocks;

    int max_nodes;
    int node_count;
    int linker;
    int imagebase;
    char padding[16];

    char label[64];
    char author[64];
    char publisher[64];
} volume_t;

#define EOF (-1)
#define SEEK_SET (0)
#define SEEK_CUR (1)
#define SEEK_END (2)

#pragma pack(pop)
#endif // INODE_H