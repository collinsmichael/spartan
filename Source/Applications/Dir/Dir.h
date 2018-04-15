#ifndef APPLICATIONS_DIR_H
#define APPLICATIONS_DIR_H
#include <Shell\RunTime\Graphics\Bitmap.h>
#include <Shell\Window\Window.h>

typedef struct CItem {
    CBitmap *Bitmap;
    char *FileType;
    char *FileSize;
    char *FileName;
    char *FilePath;
    char  Full[48];
    char  Path[48];
    char  Name[32];
    char  Type[16];
    char  Size[12];
} CItem;

typedef struct ICache {
    bool   (*SetUp)(void);
    char  *(*GetAssociations)(CItem *item);
    bool   (*CacheIcons)(void);
    bool   (*CacheFolders)(void);
    bool   (*CacheFiles)(void);
    bool   (*InsertRecord)(char *type, char *size, CBitmap *icon, char *path, char *name);
    CItem *(*FindFolder)(char *path);
    CItem *(*FindRecord)(char *path, char *name);
    bool   (*ListFolder)(CWindow *listview, char *dir);
} ICache;

#endif APPLICATIONS_DIR_H