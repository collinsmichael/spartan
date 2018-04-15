#include <Kernel.h>
#include <Shell.h>

import IFileIO *FileIO;
import IMemory *Memory;

static bool IFont_Destroy(CFont *font) {
    if (!font) return false;
    if (font->Image) Memory->Free(font->Image);
    return true;
}

static bool IFont_LoadFont(CFont *font, char *path) {
    if (!font || !path) return false;

    CFile *file = FileIO->Open(path, "-rw-");
    if (!file) return false;
    BMP *bmp = (BMP*)FileIO->Base(file);
    FileIO->Close(file);

    int resx = ABS(bmp->Width);
    int resy = ABS(bmp->Height);
    int bits = ABS(bmp->BitCount);
    int scan = resx*bits/8;

    font->Image = Memory->Alloc(resy*scan);
    if (!font->Image) return false;

    char *source = (char*)bmp + bmp->Offset;
    char *target = font->Image;
    movsd(target, source, resy*scan/4);

    font->FontW    = resx/16;
    font->FontH    = resy/16;
    font->ScanLine = scan;
    return true;
}

bool IFont_PutC(CFont *font, CCanvas *canvas, int x, int y, int fg, char c) {
    if (!font || !canvas) return false;
    int fx = (c >> 0) & 0x0F;
    int fy = (c >> 4) & 0x0F;

    int u1 = fx*font->FontW;
    int v1 = fy*font->FontH;
    int u2 = u1 + font->FontW;
    int v2 = v1 + font->FontH;

    char *source = &font->Image[v1*font->ScanLine];
    char *target = &canvas->Draw[y*canvas->ScanLine];
    for (int v = v1; v < v2; v++ ) {
        int *trg = (int*)target;
        trg = &trg[x];
        for (int u = u1; u < u2; u++) {
            char packed = source[u/8] & (0x80 >> (u & 7));
            u32 and = (packed) ? 00 : 0x00FFFFFF;
            u32 or  = (packed) ? fg : 0x00000000;
            *trg++ = *trg & and | or;
        }
        target += canvas->ScanLine;
        source += font->ScanLine;
    }
    return true;
}

bool IFont_PutCharClip(CArea *area, CFont *font, CCanvas *canvas, int x, int y, int fg, char c) {
    if (!area || !font || !canvas) return false;
    int fx = (c >> 0) & 0x0F;
    int fy = (c >> 4) & 0x0F;

    int u1 = fx*font->FontW;
    int v1 = fy*font->FontH;
    int u2 = u1 + font->FontW;
    int v2 = v1 + font->FontH;

    char *source = &font->Image[v1*font->ScanLine];
    char *target = &canvas->Draw[y*canvas->ScanLine];
    for (int v = v1; v < v2; v++ ) {
        if (y < area->Y1 || y >= area->Y2) continue;
        y++;
        int *trg = (int*)target;
        trg = &trg[x];
        int i = x;
        for (int u = u1; u < u2; u++) {
            if (i < area->X1 || i >= area->X2) continue;
            char packed = source[u/8] & (0x80 >> (u & 7));
            u32 and = (packed) ? 00 : 0x00FFFFFF;
            u32 or  = (packed) ? fg : 0x00000000;
            *trg++ = *trg & and | or;
            i++;
        }
        target += canvas->ScanLine;
        source += font->ScanLine;
    }
    return true;
}

static IFont font = {
    IFont_Destroy,
    IFont_LoadFont,
    IFont_PutC,
    IFont_PutCharClip
};

export IFont *Font = &font;