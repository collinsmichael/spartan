#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <string.h>
#include <windows.h>

#define STDDEF_H
#include "ExeLoader.h"
#include "RamDisk.h"
#include "FileIO.h"

extern IExeLoader ExeLoader;
extern IRamDisk RamDisk;
extern IFileIO  FileIO;
static char raw[16*MB+4*KB];
char *RAM = raw;
char *boot = 0;
int imagebase;

void banner(void) {
    printf(" ============================================================== \n");
    printf(" mkrd - version 1.0 - Michael Collins (2017)                    \n");
    printf(" ============================================================== \n");
}

int usage(int argc, char *argv[]) {
    if (argc == 5) return 1;
    //printf(" usage: mkrd base path/to/root disk.img boot.exe EI EJ P\n");
    printf(" usage: mkrd base path/to/root disk.img boot.exe\n");
    return 0;
}

void ListFiles(char *root, char *path) {
    WIN32_FIND_DATAA find;
    memset(&find, 0, sizeof(find));

    char wildcard[256];
    memset(wildcard, 0, sizeof(wildcard));
    sprintf(wildcard, "%s%s*", root, path);

    void *hFind = FindFirstFileA(wildcard, &find);
    if (hFind == INVALID_HANDLE_VALUE) return;
    do {
        if (strcmp(find.cFileName, ".") == 0) continue;
        if (strcmp(find.cFileName, "..") == 0) continue;

        char qualified[256];
        memset(qualified, 0, sizeof(qualified));
        sprintf(qualified, "%s%s", path, find.cFileName);

        char flag[8];
        strcpy(flag, "-rw-");
        if (find.dwFileAttributes & FILE_ATTRIBUTE_HIDDEN) flag[0] = 's';
        if (strstr(find.cFileName, ".exe")) flag[3] = 'x';
        if (strstr(find.cFileName, ".dll")) flag[3] = 'x';

        if (find.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
            strcat(qualified, "/");
            ListFiles(root, qualified);
        } else {
            int size = find.nFileSizeLow;
          //printf(" mkrd [pass] Creating %s (%7d) %s\n", flag, size, qualified);
            CNode *node = RamDisk.Insert(FileIO.Disk, qualified, size, flag);
        }
    } while (FindNextFileA(hFind, &find));
    FindClose(hFind);
}

int WriteFiles(void) {
    char *image = &RAM[imagebase];
    CDisk *disk = (CDisk*)image;
    CNode *node = (CNode*)&image[disk->NodeList];
    for (int i = 0; i < disk->NodeCount; i++) {
        char *data = &RAM[(int)node[i].FileData];
        int size = node[i].FileSize;

        FILE *file = fopen(node[i].FileName, "rb");
        if (!file) {
            printf(" mkrd [fail] Cannot open file! %s\n", node[i].FileName);
            return 0;
        }
        if (fread(data, 1, size, file) != size) {
            printf(" mkrd [fail] Cannot read file! %s\n", node[i].FileName);
            return 0;
        }
    }
    return 1;
}

int RelocFiles(void) {
    char *image = &RAM[imagebase];
    CDisk *disk = (CDisk*)image;
    CNode *node = (CNode*)&image[disk->NodeList];
    for (int i = 0; i < disk->NodeCount; i++) {
        if (node[i].FileFlag[3] != 'x') continue;

        char *path = node[i].FileName;
        int base = node[i].FileData;
        int size = node[i].FileSize;
        ExeLoader.Reloc(path, base, size);
      //printf(" mkrd [pass] Relocating %s\n", node[i].FileName);
    }
    return 1;
}

int LinkFiles(void) {
    char *image = &RAM[imagebase];
    CDisk *disk = (CDisk*)image;
    CNode *node = (CNode*)&image[disk->NodeList];
    for (int i = 0; i < disk->NodeCount; i++) {
        if (node[i].FileFlag[3] != 'x') continue;

        char *path = node[i].FileName;
        int base = node[i].FileData;
        int size = node[i].FileSize;
        ExeLoader.Link(path, base, size);
      //printf(" mkrd [pass] Linking %s\n", node[i].FileName);
    }
    return 1;
}

int main(int argc, char *argv[]) {
    if (!usage(argc, argv)) return 0;
    banner();

    imagebase = strtol(argv[1],0,0);
    char *root = argv[2];
    char *out  = argv[3];
    boot = argv[4];

    int imagesize = 8*MB-imagebase;
    char *image = &RAM[imagebase];
    CDisk *disk = (CDisk*)image;
    RamDisk.Format(disk, imagesize, "SPARTAN RAMDISK");
    disk->DiskBase = imagebase;
    FileIO.Disk = disk;

    char dir[1024];
    GetCurrentDirectoryA(sizeof(dir), dir);

    SetCurrentDirectoryA(root);
    ListFiles(root, "");

    // compact the file system
    CNode *node = (CNode*)&image[disk->NodeList];
    int base = disk->FileData + imagebase;
    for (int i = 0; i < disk->NodeCount; i++) {
        node[i].FileData = base;
        int size = node[i].FileSize;
        size = (size + (4*KB-1)) & ~(4*KB-1);
        base += size;
    }
    if (!WriteFiles()) return -1;
    if (!RelocFiles()) return -1;
    if (!LinkFiles()) return -1;
    
    SetCurrentDirectoryA(dir);
    FILE *fd = fopen(out, "wb");
    if (fd) {
        extern int counter;
        int usedspace = disk->FileData + counter;
        fwrite(&RAM[imagebase], 1, usedspace, fd);
        fflush(fd);
        fclose(fd);

        printf(" mkrd: VolumeID  = %s\n", disk->VolumeID);
        printf(" mkrd: DiskBase  = %08X (%d)\n", disk->DiskBase , disk->DiskBase );
        printf(" mkrd: DiskSpace = %08X (%d)\n", imagesize      , imagesize      );
        printf(" mkrd: DiskSize  = %08X (%d)\n", usedspace      , usedspace      );
        printf(" mkrd: Capacity  = %08X (%d)\n", disk->Capacity , disk->Capacity );
        printf(" mkrd: Geometry  = %08X (%d)\n", disk->Geometry , disk->Geometry );
        printf(" mkrd: NodeList  = %08X (%d)\n", disk->NodeList , disk->NodeList );
        printf(" mkrd: FileData  = %08X (%d)\n", disk->FileData , disk->FileData );
        printf(" mkrd: NodeCount = %08X (%d)\n", disk->NodeCount, disk->NodeCount);
        printf(" mkrd: FileCount = %08X (%d)\n", disk->FileCount, disk->FileCount);
        printf(" mkrd: Entry     = %08X (%d)\n", disk->Entry    , disk->Entry    );
        printf(" mkrd: NextNode  = %08X (%d)\n", disk->NextNode , disk->NextNode );
        printf(" mkrd: NextFile  = %08X (%d)\n", disk->NextFile , disk->NextFile );
        printf(" mkrd: BootFile  = %s\n", boot);
        printf(" mkrd: success!\n");
    }
    return 0;
}