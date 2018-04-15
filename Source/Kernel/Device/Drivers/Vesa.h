#ifndef KERNEL_DEVICE_DRIVERS_VESA_H
#define KERNEL_DEVICE_DRIVERS_VESA_H
#include <oop.h>

typedef struct CVesa {
    u16 Mode;
    u16 ResX;
    u16 ResY;
    u16 Bits;
    u16 ScanLine;
    u8 *Pixels;
} CVesa;

typedef struct IVesa {
    u16  (*ResX)(void);
    u16  (*ResY)(void);
    u16  (*Bits)(void);
    u16  (*ScanLine)(void);
    u8  *(*Pixels)(void);
    void (*Blit)(int params[]);
    void (*Sync)(void);
} IVesa;

#endif KERNEL_DEVICE_DRIVERS_VESA_H