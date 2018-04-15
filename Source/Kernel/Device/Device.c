#include <Kernel.h>

extern IMonitor *Monitor;

u32 DefaultIsrStub(u32 err, u32 esp);

int IDevice_Latch(int irq, u32 (*handler)(u32 err, u32 esp)) {
    //Logger("IDevice_Hook(%X, %X)\n", irq, handler);
    u32 *list = (u32*)Monitor->GetBase('IRQ ');
    if (list) {
        list[irq] = (u32)handler;
        Monitor->Protect('IRQ ');
    }
    return irq;
}

int IDevice_Unlatch(int irq) {
    u32 *list = (u32*)Monitor->GetBase('IRQ ');
    if (list) {
        list[irq] = (u32)DefaultIsrStub;
        Monitor->Protect('IRQ ');
    }
    return irq;
}

bool IDevice_Hook(u32 device, void *EndPoint) {
    CPipe *pipe = (CPipe*)Monitor->GetBase(device);
    if (!pipe) return false;
    pipe->Reader = (u32)EndPoint;
    return true;
}

bool IDevice_Unhook(u32 device) {
    CPipe *pipe = (CPipe*)Monitor->GetBase(device);
    if (!pipe) return false;
    pipe->Reader = nullptr;
    return true;
}

bool InstallDevices(void) {
    NOTE("Hook(0x20, Speaker);")
    NOTE("Hook(0x22, PicMaster);")
    NOTE("Hook(0x23, SerialCom2);")
    NOTE("Hook(0x24, SerialCom1);")
    NOTE("Hook(0x25, Paralell2);")
    NOTE("Hook(0x26, FloppyDisk);")
    NOTE("Hook(0x27, Paralell1);")
    NOTE("Hook(0x28, RealTimeClock);")
    NOTE("Hook(0x29, Acpi);")
    NOTE("Hook(0x2A, Nic1);")
    NOTE("Hook(0x2B, Nic2);")
    NOTE("Hook(0x2C, Mouse);")
    NOTE("Hook(0x2D, Fpu);")
    NOTE("Hook(0x2E, HardDiskMaster);")
    NOTE("Hook(0x2F, HardDiskSlave);")
    return true;
}

static IDevice device = {
    IDevice_Latch,
    IDevice_Unlatch,
    IDevice_Hook,
    IDevice_Unhook
};

export IDevice *Device = &device;