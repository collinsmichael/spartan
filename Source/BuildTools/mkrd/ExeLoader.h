#ifndef KERNEL_APPLICATION_EXELOADER_H
#define KERNEL_APPLICATION_EXELOADER_H
#include "Exe.h"
#include "FileIO.h"
#pragma pack(push, 1)

typedef struct CExeLoader {
    CFile *ExeFile;
    CFile *StdIn;
    CFile *StdOut;
    CFile *StdErr;
    char  *ImageBase;
    int    ImageSize;
} CExeLoader;

typedef struct IExeLoader {
    int (*Reloc)(char *path, int base, int size);
    int (*Link )(char *path, int base, int size);
} IExeLoader;

#pragma pack(pop)
#endif KERNEL_APPLICATION_EXELOADER_H