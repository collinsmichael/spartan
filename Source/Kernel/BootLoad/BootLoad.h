#ifndef KERNEL_BOOTLOAD_H
#define KERNEL_BOOTLOAD_H
#include <oop.h>
#include <Kernel\BootLoad\Intel\Intel.h>
#include <Kernel\BootLoad\Intel\Fpu.h>
#include <Kernel\BootLoad\Intel\Pic.h>
#include <Kernel\BootLoad\Intel\Tss.h>
#include <Kernel\BootLoad\MultiBoot\MultiBoot.h>
#include <Kernel\BootLoad\PcBios\Cga.h>
#include <Kernel\BootLoad\PcBios\Cmos.h>
#include <Kernel\BootLoad\PcBios\Com.h>
#include <Kernel\BootLoad\PcBios\PcBios.h>
#include <Kernel\BootLoad\PcBios\Pit.h>
#include <Kernel\BootLoad\PcBios\Ps2.h>
#include <Kernel\BootLoad\PcBios\Rtc.h>
#include <Kernel\BootLoad\PcBios\PcBios.h>

typedef struct CModule {
    int    Name;
    bool (*Proc[3])(u8*,u32);
} CModule;

typedef struct CInstaller {
    int Name;
    bool (*Proc)(u8*,u32);
} CInstaller;

typedef struct IBootLoad {
    bool (*Grub)(void *info);
    bool (*Boot)(void *grub);
    bool (*Prepare)(CInstaller prepare[], int count);
    bool (*Enable)(CInstaller enable[], int count);
    bool (*Activate)(void);
} IBootLoad;

bool BootStrap(void *grub);

#ifndef IMPL_IBOOTLOAD
int Hook(int irq, u32 (*handler)(u32 err, u32 esp));
int Unhook(int irq);
#endif IMPL_IBOOTLOAD

#endif KERNEL_BOOTLOAD_H