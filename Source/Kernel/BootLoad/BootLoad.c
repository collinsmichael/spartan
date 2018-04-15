#define IMPL_IBOOTLOAD
#define IMPL_IMONITOR
#include <Kernel.h>

NOTE("************************************************************************")
NOTE("* Multiboot Specification (https://goo.gl/kX7eRJ)                      *")
NOTE("************************************************************************")

extern IMonitor   *Monitor;
extern IException *Exception;

bool EnableSysCalls(void);
bool EnablePs2(void);
bool EnableFpu(void);
bool RemapPic(void);

bool IBootLoad_Grub(void *info) {
    if (!info) return false;
    return MultiBoot(info);
}

bool IBootLoad_Prepare(CInstaller *prepare, int count) {
    for (int i = 0; i < count; i++) {
        int owner = prepare[i].Name;
        u8 *base = Monitor->GetBase(owner);
        u32 size = Monitor->GetSize(owner);
        bool status = prepare[i].Proc(base, size);
#ifndef OPTIMIZED
        u64 totext = (u64)owner;
        if (status) Logger(" Installing %s ... OK\n", &totext);
        else Logger(" Installing %s ... Failed\n", &totext);
#endif
        if (!status) return false;
        Monitor->Protect(owner);
    }

    for (int i = 0; i < count; i++) {
        bool status = Monitor->Verify(prepare[i].Name);
        if (!status) return false;
    }
    return true;
}

bool IBootLoad_Enable(CInstaller *enable, int count) {
    for (int i = 0; i < count; i++) {
        int owner = enable[i].Name;
        u8 *base = Monitor->GetBase(owner);
        u32 size = Monitor->GetSize(owner);
        bool status = enable[i].Proc(base, size);

#ifndef OPTIMIZED
        u64 totext = (u64)owner;
        if (status) Logger(" Enabling   %s ... OK\n", &totext);
        else Logger(" Enabling   %s ... Failed\n", &totext);
#endif
        if (!status) return false;
        Monitor->Protect(owner);
    }
    return true;
}

bool IBootLoad_Activate(void) {
    if (!EnableSysCalls()) return false;
    if (!Exception->Install()) return false;
    if (!EnablePs2()) return false;
    if (!RemapPic()) return false;
    if (!EnableFpu()) return false;
    return true;
}

bool IBootLoad_Boot(void *grub) {
    return true;
}

IBootLoad bootload = {
    IBootLoad_Grub,
    IBootLoad_Boot,
    IBootLoad_Prepare,
    IBootLoad_Enable,
    IBootLoad_Activate
};

export IBootLoad *BootLoad = &bootload;