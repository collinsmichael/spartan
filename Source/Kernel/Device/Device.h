#ifndef KERNEL_DEVICE_H
#define KERNEL_DEVICE_H

#include <oop.h>
#include <Kernel\BootLoad\BootLoad.h>
#include <Kernel\Device\Drivers\RamDisk.h>
#include <Kernel\Device\Drivers\Vesa.h>
#include <Kernel\Device\FileIO\FileIO.h>
#include <Kernel\Device\FileIO\Pipe.h>

typedef struct IDevice {
    int  (*Latch  )(int irq, u32 (*handler)(u32 err, u32 esp));
    int  (*Unlatch)(int irq);
    bool (*Hook   )(u32 device, void *EndPoint);
    bool (*Unhook )(u32 device);
} IDevice;

typedef struct IClock {
    void (*Flush   )(void);
    void (*CmosDate)(u8 *datetime);
    bool (*Now     )(u8 *now);
    u32  (*GetTicks)(void);
    u32  (*GetFreq )(void);
} IClock;

typedef struct IKeyboard {
    void  (*Flush  )(void);
    bool  (*KeyDown)(int scancode);
    int   (*GetLed )(void);
    bool  (*SetLed )(int led);
    char *(*GetKeyMap)(void);
    bool  (*SetKeyMap)(char *layout);
} IKeyboard;
#define KB_CAP_LOCK 1
#define KB_NUM_LOCK 2
#define KB_SCR_LOCK 4

typedef struct IMouse {
    bool (*Enable)(void);
    void (*Flush)(void);
    int  (*GetSpeed)(void);
    bool (*SetSpeed)(int speed);
} IMouse;

#endif KERNEL_DEVICE_H