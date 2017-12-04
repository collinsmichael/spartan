#ifndef INODE_H
#define INODE_H
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

/*
#define BLOCKS    256
#define SYS_NODES (sizeof(volume_t) + BLOCKS*sizeof(inode_t) + BLOCKS*sizeof(block_t))
#define FIRST ( 5*MB + SYS_NODES)
#define FINAL (FIRST + 15*MB)
#define DISK_SPACE (FINAL-FIRST)
#define MAX_NODES 256
*/

#define MAX_NODES  256
#define MAX_BLOCKS 224
#define SYS_NODES  (sizeof(volume_t) + sizeof(inode_t)*MAX_NODES + sizeof(block_t)*MAX_BLOCKS)
#define FIRST      (imagebase + SYS_NODES)
#define FINAL      (FIRST + 15*MB)
#define DISK_SPACE (FINAL-FIRST)


typedef struct block_t {
    int base;
    int size;
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

int dalloc(int size);
void dfree(int addr, int size);

int mknode(int size, int flags, int uid, int gid);
int delnode(int inode);
int renode(int inode, int size);
int defrag(void);


int mkflags(char *access);
int user(void);
int group(void);
int chmod(char *path, char *access);


int mkbin(char *path);
int mkdir(char *path);
int rmbin(char *path);
int rmdir(char *path);

int ffind(char *path, char *buf, int len);
int fnext(int inode, char *buf, int len);

typedef struct XFILE {
    int   inode;
    int   head;
    int   tail;
    char *ring;
} XFILE;

#ifdef EOF
#undef EOF
#endif
#define EOF (-1)

#ifdef SEEK_SET
#undef SEEK_SET
#endif
#define SEEK_SET (0)

#ifdef SEEK_CUR
#undef SEEK_CUR
#endif
#define SEEK_CUR (1)

#ifdef SEEK_END
#undef SEEK_END
#endif
#define SEEK_END (2)

XFILE *xopen(char *path, char *access);
int xclose(XFILE *file);
int xsize(XFILE *file);
int xseek(XFILE *file, int offset, int method);
int xgetc(XFILE *file);
int xputc(int ch, XFILE *file);

#pragma pack(pop)
#endif // INODE_H