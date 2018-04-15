#include <Kernel.h>
#include <Library.h>

extern char *RAM;

int DiskFormat(CDisk *volume, int size, char *VolumeID) {
    if (!volume) return 0;
    char *disk = (char*)volume;

    stosd(volume, 0, 256/4);
    if (StrLen(VolumeID) > sizeof(volume->VolumeID)) {
        return 0; // Error("RamDisk.Format [fail] VolumeID is too long! %s > 32 characters\n", path);
    }

    volume->DiskBase  = (int)volume;
    volume->DiskSize  = size;
    volume->Capacity  = 4*KB-256;
    volume->Geometry  = 256;
    volume->NodeList  = 4*KB;
    volume->FileData  = 64*4*KB;
    volume->NodeCount = 0;
    volume->FileCount = 0;
    volume->NextNode  = volume->FileData/(4*KB);
    volume->NextFile  = volume->Capacity/(4*KB);

    stosb(disk, ' ', sizeof(volume->VolumeID));
    movsb(disk, VolumeID, StrLen(VolumeID));
    stosd(&disk[256], 'oooo', (4*KB-256)/4);
    stosd(&disk[256], 'nnnn', 64/4);
    disk[256] = 'v';

    stosd(&disk[volume->NodeList], 0, (volume->FileData - volume->NodeList)/4);
    stosd(&disk[volume->FileData], 'xxxx', (volume->DiskSize - volume->FileData)/4);
    return 1;
}

int DiskAlloc(CDisk *disk, int size) {
    if (!disk) return 0;

    size = (size + (4*KB-1)) & ~(4*KB-1);
    int n = size/(4*KB);
    char *block = (char*)disk;
    block = &block[disk->Geometry];
    int blocks = disk->Capacity;

    for (int i = disk->NextNode; i + n != disk->NextNode; i++) {
        i = i % blocks;
        if (block[i] == 'f') {
            i = 0;

        } else for (int j = 0; j <= n; j++) {
            if (block[i+j] != 'o') break;
            if (j == n) {
                stosb(&block[i], 'x', n);
                disk->NextNode = (i + n) % blocks;
                return i;
            }
        }
    }
    return 0;
}

int DiskRelease(CDisk *disk, int sector, int size) {
    if (!disk) return 0;

    size = (size + (4*KB-1)) & ~(4*KB-1);
    int n = size/(4*KB);
    char *block = (char*)disk;
    block = &block[disk->Geometry];

    for (int i = sector; i < sector+n; i++) {
        if (block[i] == 'x') block[i] = 'o';
    }
    return 1;
}

INode *DiskSearch(CDisk *disk, char *path) {
    char *base = (char*)disk;
    INode *node = (INode*)&base[disk->NodeList];
    int n = BinarySearch((char*)node, disk->NodeCount, sizeof(INode), 16, path);
    if (n < 0) return 0;
    return &node[n];
}

INode *DiskInsert(CDisk *disk, char *path, int size, char *flag) {
    char *base = (char*)disk;
    INode *item = DiskSearch(disk, path);
    if (item) return item; // FileNames must be unique, so just return the existing one
    if (size < 0) return 0; // Error("RamDisk.Create [fail] Negative file size! %s\n", path);
    if ((flag[0] != 's' && flag[0] != '-') || (flag[1] != 'r' && flag[1] != '-') ||
        (flag[2] != 'w' && flag[2] != '-') || (flag[3] != 'x' && flag[3] != '-')) {
        return 0; // Error("RamDisk.Create [fail] Invalid file permissions! %s (%s)\n", path, flag);
    }

    INode *node = (INode*)&base[disk->NodeList];
    item = &node[disk->NodeCount];

    int len = StrLen(path);
    if (len > 48-1) return 0; // Error("RamDisk.Create [fail] File name is too long! %s > 47 characters\n", path);

    int lba = DiskAlloc(disk, size);
    if (!lba) return 0; // Error("RamDisk.Create [fail] Out of disk space! %s\n", path);

    stosb(item, 0, sizeof(INode));
    movsb(&item->FileName, path, len);
    movsb(&item->FileFlag, flag, 4);
    item->FileSize = size;
    item->FileData = lba*4*KB;
    item->CheckSum = 0;

    char *data = &base[disk->FileData + lba*4*KB];
    int dspace = (size+(4*KB-1)) / (4*KB);
    stosd(data, 'xxxx', dspace/4);

    disk->NodeCount++;
    BubbleSort((char*)node, disk->NodeCount, sizeof(INode), 16);
    return item;
}

int DiskRemove(CDisk *disk, char *path) {
    char *base = (char*)disk;
    INode *item = DiskSearch(disk, path);
    if (!item) return 0; // Error("RamDisk.Delete [fail] File not found! %s\n", path);

    int s = (int)item->FileData;
    int f = (int)item->FileSize + s;

    disk->NodeCount--;
    INode *node = (INode*)&base[disk->NodeList];
    char *src = (char*)item;
    char *trg = (char*)&node[disk->NodeCount];
    Swap(trg, src, sizeof(INode));
    BubbleSort((char*)node, disk->NodeCount, sizeof(INode), 16);

    char *block = &base[disk->Geometry];
    stosb(&block[s/(4*KB)], 'o', (f-s)/(4*KB));
    stosd(trg, 0, sizeof(INode)/4);
    return 1;
}

struct IRamDisk ramdisk = {
    DiskFormat,
    DiskInsert,
    DiskRemove,
    DiskSearch
};

export IRamDisk *RamDisk = &ramdisk;