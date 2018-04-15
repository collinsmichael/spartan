#include <Kernel.h>
#include <Shell.h>

import IFileIO *FileIO;
import IMemory *Memory;
import IVesa   *Vesa;

static bool IBitmap_Debug(CBitmap *bitmap) {
    Debug(" bitmap         = %X\n", bitmap          );
    Debug(" bitmap->ResX   = %X\n", bitmap->ResX    );
    Debug(" bitmap->ResY   = %X\n", bitmap->ResY    );
    Debug(" bitmap->Bits   = %X\n", bitmap->Bits    );
    Debug(" bitmap->Scan   = %X\n", bitmap->ScanLine);
    Debug(" bitmap->Image  = %X\n", bitmap->Image   );
    Debug(" bitmap->Mask   = %X\n", bitmap->Mask    );
    Debug(" bitmap->Merged = %X\n", bitmap->Merged  );
    return true;
}

static bool IBitmap_Destroy(CBitmap *bitmap) {
    if (!bitmap) return false;
    if (bitmap->Image )Memory->Free(bitmap->Image);
    if (bitmap->Mask  )Memory->Free(bitmap->Mask);
    if (bitmap->Merged)Memory->Free(bitmap->Merged);
    return true;
}

static bool IBitmap_LoadBitmap(CBitmap *bitmap, char *path) {
    if (!bitmap || !path) return false;

    CFile *file = FileIO->Open(path, "-rw-");
    if (!file) return false;
    BMP *bmp = (BMP*)FileIO->Base(file);
    FileIO->Close(file);

    int resx = ABS(bmp->Width);
    int resy = ABS(bmp->Height);
    int bits = ABS(bmp->BitCount);
    int scan = resx*bits/8;
    bitmap->Image = Memory->Alloc(resy*scan);
    if (!bitmap->Image) return false;

    char *source = (char*)bmp + bmp->Offset;
    char *target = bitmap->Image;
    for (int y = 0; y < resy; y++) {
        int v = (bmp->Height <= 0) ? y : resy - 1 - y;
        int *trg = (int*)&target[y*scan];
        int *src = (int*)&source[v*scan];
        movsd(trg, src, resx);
    }

    bitmap->ResX     = resx;
    bitmap->ResY     = resy;
    bitmap->Bits     = bits;
    bitmap->ScanLine = scan;
    return true;
}

static bool IBitmap_LoadSprite(CBitmap *bitmap, char *path) {
    if (!bitmap) return false;

    if (!IBitmap_LoadBitmap(bitmap, path)) return false;

    int resx = bitmap->ResX;
    int resy = bitmap->ResY;
    int scan = bitmap->ScanLine;
    bitmap->Mask = Memory->Alloc(resy*scan);
    if (!bitmap->Mask) {
        Memory->Free(bitmap->Image);
        return false;
    }

    bitmap->Merged = Memory->Alloc(resy*scan);
    if (!bitmap->Merged) {
        Memory->Free(bitmap->Mask);
        Memory->Free(bitmap->Image);
        return false;
    }

    for (int y = 0; y < resy; y++) {
        int *img = (int*)&bitmap->Image[y*scan];
        int *and = (int*)&bitmap->Mask[y*scan];
        for (int x = 0; x < resx; x++) {
            int pixel = img[x];
            and[x] = (pixel & 0xFF000000) ? 0x00FFFFFF : 0;
            img[x] = (and[x]) ? 0 : (pixel & 0x00FFFFFF);
        }
    }
    return true;
}

static bool IBitmap_CreateSprite(CBitmap *bitmap, int resx, int resy, int bits) {
    if (!bitmap) return false;

    int scan = resx*resy*bits;
    bitmap->Image = Memory->Alloc(resy*scan);
    if (!bitmap->Image) return false;

    bitmap->Mask = Memory->Alloc(resy*scan);
    if (!bitmap->Mask) {
        Memory->Free(bitmap->Image);
        return false;
    }

    bitmap->Merged = Memory->Alloc(resy*scan);
    if (!bitmap->Merged) {
        Memory->Free(bitmap->Mask);
        Memory->Free(bitmap->Image);
        return false;
    }

    bitmap->ResX     = resx;
    bitmap->ResY     = resy;
    bitmap->Bits     = bits;
    bitmap->ScanLine = scan;
    stosd(bitmap->Image,  0, resy*scan/4);
    stosd(bitmap->Mask,   0, resy*scan/4);
    stosd(bitmap->Merged, 0, resy*scan/4);
    return true;
}

static IBitmap bitmap = {
    IBitmap_Debug,
    IBitmap_Destroy,
    IBitmap_LoadBitmap,
    IBitmap_LoadSprite,
    IBitmap_CreateSprite
};

export IBitmap *Bitmap = &bitmap;