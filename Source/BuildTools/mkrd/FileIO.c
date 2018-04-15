#include "RamDisk.h"
#include "FileIO.h"

extern IRamDisk RamDisk;

void stosb(void*,int,int);
void stosd(void*,int,int);
void movsb(void*,void*,int);
void movsd(void*,void*,int);

CFile *Open(char *path, char *flag);
int Close(CFile *file);
int Read(CFile *file, char *buf, int len);
int Write(CFile *file, char *buf, int len);
int Size(CFile *file);
int Seek(CFile *file, int offset, int origin);
char *Base(CFile *file);

IFileIO FileIO = {
    0, Open, Close, Read, Write, Size, Seek, Base
};

int FileAlloc(CDisk *disk) {
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

int FileRelease(CDisk *disk, int sector) {
    if (!disk) return 0;
    if (sector < 64 || sector > disk->Capacity) return 0;

    char *block = (char*)disk;
    block = &block[disk->Geometry];
    int blocks = disk->Capacity;

    if (block[sector] != 'f') return 0;
    block[sector] = 'o';
    disk->FileCount--;
    return 1;
}

CFile *Open(char *path, char *flag) {
    CNode *item = RamDisk.Search(FileIO.Disk, path);
    if (!item) return 0; // Error("FileIO.Open [fail] File not found! %s\n", path);

    int sector = FileAlloc(FileIO.Disk);
    if (!sector) return 0; // Error("FileIO.Open [fail] Out of disk space! %s\n", path);

    char *base = (char*)FileIO.Disk;
    CFile *file = (CFile*)&base[sector*4*KB];
    if (!file) return 0; // Error("FileIO.Open [fail] Out of disk space! %s\n", path);

    stosd(file, 0, 4*KB/4);
    file->Head  = 0;
    file->Tail  = 0;
    file->Size  = 4*KB - sizeof(CFile);
    file->Node  = item;

    file->Open  = (int(*)(char*,char*))Open;
    file->Close = (int(*)(CFile*))Close;
    file->Read  = (int(*)(CFile*,char*,int))Read;
    file->Write = (int(*)(void*,char*,int))Write;
    return file;
}

int Close(CFile *file) {
    if (!file) return 0;
    int sector = ((int)file - (int)FileIO.Disk) / (4*KB);
    if (!FileRelease(FileIO.Disk, sector)) return 0;
    stosd(file, 'xxxx', 4*KB/4);
    return 1;
}

int Read(CFile *file, char *buf, int len) {
    if (!file) return 0;
    CNode *node = file->Node;
    if (!node) return 0;

    char *data = FileIO.Base(file);
    int size = FileIO.Size(file);
    for (int i = 0; i < len; i++) {
        if (file->Head >= size) return i;
        buf[i] = data[file->Head++];
    }
    return 0;
}

int Write(CFile *file, char *buf, int len) {
    if (!file) return 0;
    CNode *node = file->Node;
    if (!node) return 0;

    char *data = FileIO.Base(file);
    int size = FileIO.Size(file);
    for (int i = 0; i < len; i++) {
        if (file->Head >= size) return i;
        data[file->Head++] = buf[i];
    }
    return 0;
}

int Size(CFile *file) {
    if (!file) return 0;
    if (!file->Node) return 0;
    return file->Node->FileSize;
}

int Seek(CFile *file, int offset, int origin) {
    if (!file) return 0;
    CNode *node = file->Node;
    if (!node) return 0;
    int size = node->FileSize;

    switch (offset) {
    case SEEK_SET: file->Head = offset;      break;
    case SEEK_CUR: file->Head += offset;     break;
    case SEEK_END: file->Head = size-offset; break;
    }
    return 1;
}

char *Base(CFile *file) {
    if (!file) return 0;
    CNode *node = file->Node;
    if (!node) return 0;

    char *base = (char*)FileIO.Disk;
    return &base[(int)node->FileData];
}