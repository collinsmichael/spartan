#define IMPL_IFILEIO
#include <Kernel.h>

extern IRamDisk *RamDisk;

bool EnableRamDisk(u8 *base, u32 size);

INode *IFileIO_FindFirst(char *path);
INode *IFileIO_FindNext(INode *prev, char *path);

int IFileIO_Alloc(CDisk *disk);
int IFileIO_Release(CDisk *disk, int sector);
int IFileIO_Close(CFile *file);
int IFileIO_Read(CFile *file, char *buf, int len);
int IFileIO_Write(CFile *file, char *buf, int len);
int IFileIO_Size(CFile *file);
int IFileIO_Seek(CFile *file, int offset, int origin);
char *IFileIO_Base(CFile *file);
CFile *IFileIO_Open(char *path, char *flag);

IFileIO fileio = {
    0, IFileIO_FindFirst, IFileIO_FindNext,
    IFileIO_Open, IFileIO_Close,
    IFileIO_Read, IFileIO_Write,
    IFileIO_Size, IFileIO_Seek, IFileIO_Base
};

bool EnableRamDisk(u8 *base, u32 size) {
    if (!base) return 0;
    fileio.Disk = (CDisk*)&base[4*KB];
    fileio.Disk->DiskBase = 0;
    return 1;
}

int IFileIO_Alloc(CDisk *disk) {
    if (!disk) return 0;

    char *block = (char*)disk;
    block = &block[disk->Geometry];
    int blocks = disk->Capacity;

    for (int i = disk->NextFile-1; i != disk->NextFile; i--) {
        if (i < 64 || block[i] == 'x') {
            i = blocks;

        } else if (block[i] == 'o') {
            block[i] = 'f';
            disk->NextFile = i;
            disk->FileCount++;
            return i;
        }
    }
    return 0;
}

int IFileIO_Release(CDisk *disk, int sector) {
    if (!disk) return 0;
    if (sector < 64 || sector > disk->Capacity) return 0;

    char *block = (char*)disk;
    block = &block[disk->Geometry];
    int blocks = disk->Capacity;

    if (block[sector] != 'f') return 0;
    block[sector] = 'o';
    disk->FileCount--;
    while (block[disk->NextFile] == 'o') {
        if (disk->NextFile < 64 || disk->NextFile == disk->Capacity) break;
        disk->NextFile++;
    }
    return 1;
}

int IFileIO_Close(CFile *file) {
    if (!file) return 0;
    int sector = ((int)file - (int)fileio.Disk) / (4*KB);
    if (!IFileIO_Release(fileio.Disk, sector)) return 0;
    stosd(file, 'xxxx', 4*KB/4);
    return 1;
}

int IFileIO_Read(CFile *file, char *buf, int len) {
    if (!file) return 0;
    INode *node = file->Node;
    if (!node) return 0;

    char *data = fileio.Base(file);
    int size = fileio.Size(file);
    for (int i = 0; i < len; i++) {
        if (file->Head >= size) return i;
        buf[i] = data[file->Head++];
    }
    return 0;
}

int IFileIO_Write(CFile *file, char *buf, int len) {
    if (!file) return 0;
    INode *node = file->Node;
    if (!node) return 0;

    char *data = fileio.Base(file);
    int size = fileio.Size(file);
    for (int i = 0; i < len; i++) {
        if (file->Head >= size) return i;
        data[file->Head++] = buf[i];
    }
    return 0;
}

int IFileIO_Size(CFile *file) {
    if (!file) return 0;
    if (!file->Node) return 0;
    return file->Node->FileSize;
}

int IFileIO_Seek(CFile *file, int offset, int origin) {
    if (!file) return 0;
    INode *node = file->Node;
    if (!node) return 0;
    int size = node->FileSize;

    switch (offset) {
    case SEEK_SET: file->Head = offset;      break;
    case SEEK_CUR: file->Head += offset;     break;
    case SEEK_END: file->Head = size-offset; break;
    }
    return 1;
}

char *IFileIO_Base(CFile *file) {
    if (!file) return 0;
    INode *node = file->Node;
    if (!node) return 0;

    char *base = (char*)fileio.Disk->DiskBase;
    return &base[(int)node->FileData];
}

CFile *IFileIO_Open(char *path, char *flag) {
    INode *item = RamDisk->Search(fileio.Disk, path);
    if (!item) return 0; // Error("FileIO.Open [fail] File not found! %s\n", path);

    int sector = IFileIO_Alloc(fileio.Disk);
    if (!sector) return 0; // Error("FileIO.Open [fail] Out of disk space! %s\n", path);

    char *base = (char*)fileio.Disk;
    CFile *file = (CFile*)&base[sector*4*KB];
    if (!file) return 0; // Error("FileIO.Open [fail] Out of disk space! %s\n", path);

    stosd(file, 0, 4*KB/4);
    file->Head  = 0;
    file->Tail  = 0;
    file->Size  = 4*KB - sizeof(CFile);
    file->Node  = item;

    file->Open  = (int(*)(char*,char*))IFileIO_Open;
    file->Close = (int(*)(CFile*))IFileIO_Close;
    file->Read  = (int(*)(CFile*,char*,int))IFileIO_Read;
    file->Write = (int(*)(void*,char*,int))IFileIO_Write;
    return file;
}

INode *IFileIO_FindFirst(char *path) {
    CDisk *disk = fileio.Disk;
    char *base = (char*)disk;
    INode *node = (INode*)&base[disk->NodeList];

    if (!path) return node;
    if (!path[0]) return node;
    int n = BinarySearchPartial((char*)node, disk->NodeCount, sizeof(INode), 16, path);
    if (n < 0) return 0;
    return &node[n];
}

INode *IFileIO_FindNext(INode *prev, char *path) {
    if (!prev || !path) return 0;

    CDisk *disk = fileio.Disk;
    char *base = (char*)disk;
    INode *node = (INode*)&base[disk->NodeList];

    int len = StrLen(path);
    int end = len;
    for (int i = len; prev->FileName[i]; i++) {
        if (prev->FileName[i] == '/') {
            end = i;
            break;
        }
    }

    if (end != len) {
        for (INode *skip = &prev[1]; ; skip++) {
            if (skip->FileName[0] == 0) return 0;
            if (MemCmp(skip->FileName, prev->FileName, end) != 0) break;
            prev = skip;
        }
    }

    INode *next = &prev[1];
    if (next->FileName[0] == 0) return 0;
    if (!path || !path[0]) return next;
    if (MemCmp(next->FileName, path, len) == 0) return next;
    return 0;
}

export IFileIO *FileIO = &fileio;