#ifndef SHELL_GRAPHICS_FONT_H
#define SHELL_GRAPHICS_FONT_H
#include <oop.h>
#include <Shell\RunTime\Graphics\Area.h>
#pragma pack(push, 1)

typedef struct CFont {
    int   ResX;
    int   ResY;
    int   Bits;
    int   ScanLine;
    char *Image;
    char *Mask;
    char *Merged;
    int   FontW;
    int   FontH;
} CFont;

typedef struct IFont {
    bool (*Destroy    )(CFont *font);
    bool (*LoadFont   )(CFont *font, char *path);
    bool (*PutC       )(CFont *font, void *canvas, int x, int y, int fg, char c);
    bool (*PutCharClip)(CArea *area, CFont *font, void *canvas, int x, int y, int fg, char c);
} IFont;

#pragma pack(pop)
#endif SHELL_GRAPHICS_FONT_H