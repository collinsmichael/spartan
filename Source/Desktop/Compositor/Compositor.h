#ifndef DESKTOP_COMPOSITOR_H
#define DESKTOP_COMPOSITOR_H
#include <oop.h>

typedef struct ICompositor {
    bool (*Install)(void);
    int  (*DrawScanLine)(CCanvas *canvas, int y);
} ICompositor;

#endif DESKTOP_COMPOSITOR_H