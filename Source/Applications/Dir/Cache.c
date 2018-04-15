#include <Kernel.h>
#include <Shell.h>
#include "Dir.h"

import IMemory   *Memory;
import IFileIO   *FileIO;
import IBitmap   *Bitmap;
import IListView *ListView;

static int count;
static CItem *items;
static CBitmap icon[16];
static char *meta[16*4] = {
     "DIR",  "0", "Media/Icons/dir.bmp",  "Apps/Dir.exe %s",
     "FILE", "0", "Media/Icons/file.bmp", 0,
     "BIN",  "0", "Media/Icons/bin.bmp",  0,
     "BMP",  "0", "Media/Icons/bmp.bmp",  "Apps/Bmp.exe %s",
     "CFG",  "0", "Media/Icons/cfg.bmp",  "Apps/Txt/Txt.exe %s",
     "CSV",  "0", "Media/Icons/csv.bmp",  "Apps/Txt/Txt.exe %s",
     "DAT",  "0", "Media/Icons/dat.bmp",  0,
     "DLL",  "0", "Media/Icons/dll.bmp",  0,
     "EXE",  "0", "Media/Icons/exe.bmp",  "%s",
     "JSON", "0", "Media/Icons/json.bmp", "Apps/Txt/Txt.exe %s",
     "SH",   "0", "Media/Icons/sh.bmp",   "Apps/Sh.exe  %s",
     "TXT",  "0", "Media/Icons/txt.bmp",  "Apps/Txt/Txt.exe %s",
     "VID",  "0", "Media/Icons/vid.bmp",  "Apps/Vid.exe %s",
     "WAV",  "0", "Media/Icons/wav.bmp",  "Apps/Wav.exe %s",
     "XML",  "0", "Media/Icons/xml.bmp",  "Apps/Txt/Txt.exe %s",
     "ZIP",  "0", "Media/Icons/zip.bmp",  "Apps/Zip.exe %s"
};

char *ICache_GetAssociations(CItem *item) {
    if (StriCmp(item->Type, "EXE") == 0) {
        return item->Full;
    }
    for (int i = 0; i < elementsof(meta)/4; i++) {
        char *type = meta[i*4 + 0];
        if (StriCmp(item->Type, type) == 0) {
            char *app = meta[i*4 + 3];
            return app;
        }
    }
    return 0;
}

bool ICache_CacheIcons(void) {
    for (int i = 0; i < elementsof(meta)/4; i++) {
        char *path = meta[i*4 + 2];
        if (!Bitmap->LoadSprite(&icon[i], path)) return false;
    }
    return true;
}

bool ICache_InsertRecord(char *type, char *size, CBitmap *icon, char *path, char *name) {
    CItem *item = &items[count++];
    stosd(item, 0, sizeof(CItem)/4);
    Format(item->Full, "%s%s", path, name);

    if (type) StrCpy(item->Type, type);
    if (size) StrCpy(item->Size, size);
    if (path) {
        StrCpy(item->Path, path);
        int len = MAX(0, StrLen(item->Path)-1);
        if (item->Path[len] == '/') item->Path[len] = 0;
    }
    if (name) {
        StrCpy(item->Name, name);
        int len = MAX(0, StrLen(item->Name)-1);
        if (item->Name[len] == '/') item->Name[len] = 0;
    }

    item->Bitmap   = icon;
    item->FileSize = item->Size;
    item->FileType = item->Type;
    item->FilePath = item->Path;
    item->FileName = item->Name;
    Logger("[%s][%s][%s](%s)(%s)\n", item->Path, item->Name, item->Full, item->FileType, item->FileSize);
    return true;
}

CItem *ICache_FindFolder(char *path) {
    for (int i = 0; i < count; i++) {
        CItem *item = &items[i];
        if (StrCmp(item->Full, path) == 0) return item;
    }
    return 0;
}

CItem *ICache_FindRecord(char *path, char *name) {
    char temp[64];
    stosd(temp, 0, sizeof(temp)/4);
    Format(temp, "%s%s", path, name);
    for (int i = 0; i < count; i++) {
        CItem *item = &items[i];
        if (StrCmp(item->Full, temp) == 0) return item;
    }
    return 0;
}

bool ICache_CacheFolders(void) {
    CDisk *disk = FileIO->Disk;
    char *base = (char*)disk;
    INode *nodes = (INode*)&base[disk->NodeList];

    for (int index = 0; index < 1024; index++) {
        CItem *item = &items[index];
        INode *node = &nodes[index];
        if (node->FileName[0] == 0) break;

        char path[64];
        char head[64];
        stosd(path, 0, sizeof(path)/4);
        StrCpy(path, node->FileName);

        for (char *body = path; *body; ) {
            char *tail = body;
            while (*tail && *tail != '/' && *tail != '.') tail++;

            char c = *tail;
            if (c == '/' || c == 0) {
                *tail = 0;
                stosb(head, 0, sizeof(head));
                movsb(head, path, (int)body-(int)path);
                if (!ICache_FindRecord(head, body)) {
                    CBitmap *image = &icon[0];
                    ICache_InsertRecord("DIR", "", image, head, body);
                }
                *tail = c;
            } else break;
            body = ++tail;
        }
    }
    return true;
}

bool ICache_CacheFiles(void) {
    CDisk *disk = FileIO->Disk;
    char *base = (char*)disk;
    INode *nodes = (INode*)&base[disk->NodeList];

    for (int index = 0; index < 1024; index++) {
        CItem *item = &items[index];
        INode *node = &nodes[index];
        if (node->FileName[0] == 0) break;

        char path[64];
        char type[64];
        char head[64];
        stosd(path, 0, sizeof(path)/4);
        StrCpy(path, node->FileName);

        for (char *body = path; *body; ) {
            char *tail = body;
            while (*tail && *tail != '/' && *tail != '.') tail++;

            char c = *tail;
            if (c == '.') {
                stosb(head, 0, sizeof(head));
                movsb(head, path, (int)body-(int)path);
                if (!ICache_FindRecord(head, body)) {
                    CBitmap *image = &icon[1];
                    StrCpy(type, &tail[1]);
                    StrUpper(type);
                    for (int i = 2; i < elementsof(meta)/4; i++) {
                        if (StrCmp(meta[i*4 + 0], type) == 0) {
                            image = &icon[i];
                            break;
                        }
                    }
                    char size[16];
                    Format(size, "%d", node->FileSize);
                    ICache_InsertRecord(&tail[1], size, image, head, body);
                }
                break;
            }
            body = ++tail;
        }
    }
    return true;
}

bool ICache_SetUp(void) {
    items = (CItem*)Memory->Alloc(128*KB);
    if (!items) return false;

    if (!ICache_CacheIcons()) return false;

    count = 0;
    stosd(items, 0, 128*KB/4);

    ICache_CacheFolders();
    ICache_CacheFiles();
    return true;
}

bool ICache_ListFolder(CWindow *listview, char *dir) {
    ListView->RemoveAll(listview);
    for (int i = 0; i < count; i++) {
        CItem *item = &items[i];
        if (StrCmp(item->Path, dir) == 0) {
            ListView->Insert(listview, (char**)&item->Bitmap);
        }
    }
    return true;
}

static ICache cache = {
    ICache_SetUp,
    ICache_GetAssociations,
    ICache_CacheIcons,
    ICache_CacheFolders,
    ICache_CacheFiles,
    ICache_InsertRecord,
    ICache_FindFolder,
    ICache_FindRecord,
    ICache_ListFolder
};

export ICache *Cache = &cache;