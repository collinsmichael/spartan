#ifndef KERNEL_DEVICE_FILEIO_H
#define KERNEL_DEVICE_FILEIO_H
#include <Kernel\Device\Drivers\RamDisk.h>

#pragma pack(push, 1)

typedef struct CFile {
    int    Head;
    int    Tail;
    int    Size;
    INode *Node;
    int  (*Open )(char *path, char *flag);
    int  (*Close)(void *file);
    int  (*Read )(void *file, char *buf, int len);
    int  (*Write)(void *file, char *buf, int len);
} CFile;

typedef struct IFileIO {
    CDisk   *Disk;
    INode *(*FindFirst)(char *path);
    INode *(*FindNext)(INode *prev, char *path);
    CFile *(*Open )(char *path, char *flag);
    int    (*Close)(CFile *file);
    int    (*Read )(CFile *file, char *buf, int len);
    int    (*Write)(CFile *file, char *buf, int len);
    int    (*Size )(CFile *file);
    int    (*Seek )(CFile *file, int offset, int origin);
    char  *(*Base )(CFile *file);
} IFileIO;

#ifndef STDDEF_H
#define STDDEF_H
    #define SEEK_SET 0
    #define SEEK_CUR 1
    #define SEEK_END 2
#endif

#pragma pack(pop)
#endif KERNEL_DEVICE_FILEIO_H