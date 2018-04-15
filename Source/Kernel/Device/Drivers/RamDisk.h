#ifndef KERNEL_DEVICE_DRIVERS_RAMDISK_H
#define KERNEL_DEVICE_DRIVERS_RAMDISK_H
#pragma pack(push, 1)

#define KB (1024)
#define MB (1024*KB)
#define GB (1024*MB)
#define elementsof(a)  (sizeof(a) / sizeof(a[0]))

typedef struct CDisk {
    char VolumeID[32];
    int  DiskBase;
    int  DiskSize;
    int  Capacity;
    int  Geometry;
    int  NodeList;
    int  FileData;
    int  NodeCount;
    int  FileCount;

    int  Entry;
    int  NextNode;
    int  NextFile;
} CDisk;

typedef struct INode {
    char  FileFlag[4];
    int   FileData;
    int   FileSize;
    int   CheckSum;
    char  FileName[48];
} INode;

typedef struct IRamDisk {
    int    (*Format)(CDisk *disk, int size, char *VolumeID);
    INode *(*Insert)(CDisk *disk, char *path, int size, char *flag);
    int    (*Remove)(CDisk *disk, char *path);
    INode *(*Search)(CDisk *disk, char *path);
} IRamDisk;

#pragma pack(pop)
#endif KERNEL_DEVICE_DRIVERS_RAMDISK_H