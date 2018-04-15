#ifndef SHELL_GRAPHICS_BITMAP_H
#define SHELL_GRAPHICS_BITMAP_H
#include <oop.h>
#pragma pack(push, 1)

typedef struct CBitmap {
    int   ResX;
    int   ResY;
    int   Bits;
    int   ScanLine;
    char *Image;
    char *Mask;
    char *Merged;
} CBitmap;

typedef struct IBitmap {
    bool (*Debug     )(CBitmap *bitmap);
    bool (*Destroy   )(CBitmap *bitmap);
    bool (*LoadBitmap)(CBitmap *bitmap, char *path);
    bool (*LoadSprite)(CBitmap *bitmap, char *path);
    bool (*CreateSprite)(CBitmap *bitmap, int resx, int resy, int bits);
} IBitmap;

#pragma pack(pop)
#endif SHELL_GRAPHICS_BITMAP_H