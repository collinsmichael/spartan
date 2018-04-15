#include <Kernel.h>
#include <Shell.h>

extern IMonitor  *Monitor;
extern IBootLoad *BootLoad;

bool InstallPcBios(u8 *base, u32 size);
bool InstallGdt(u8 *base, u32 size);
bool InstallTss(u8 *base, u32 size);
bool InstallIrq(u8 *base, u32 size);
bool InstallIsr(u8 *base, u32 size);
bool InstallIdt(u8 *base, u32 size);
bool InstallPaging(u8 *base, u32 size);
bool InstallClock(u8 *base, u32 size);
bool InstallKeyboard(u8 *base, u32 size);
bool InstallMouse(u8 *base, u32 size);
bool InstallSpeaker(u8 *base, u32 size);
bool InstallCom1(u8 *base, u32 size);
bool InstallCom2(u8 *base, u32 size);
bool InstallScheduler(u8 *base, u32 size);

bool EnableGdt(u8 *base, u32 size);
bool EnableTss(u8 *base, u32 size);
bool EnableIdt(u8 *base, u32 size);
bool EnablePaging(u8 *base, u32 size);
bool EnableClock(u8 *base, u32 size);
bool EnableKeyboard(u8 *base, u32 size);
bool EnableMouse(u8 *base, u32 size);
bool EnableSpeaker(u8 *base, u32 size);
bool EnableSerial(u8 *base, u32 size);
bool EnableRamDisk(u8 *base, u32 size);
bool EnableVesa(u8 *base, u32 size);

bool StubInstaller(u8 *base, u32 size) {
    return true;
}

CInstaller prepare[] = {
    { 'IVT ', StubInstaller    }, { 'BIOS', InstallPcBios    },
    { 'GDT ', InstallGdt       }, { 'TSS ', InstallTss       },
    { 'IRQ ', InstallIrq       }, { 'ISR ', InstallIsr       },
    { 'IDT ', InstallIdt       },

    { 'PAGE', InstallPaging    }, { 'CTX ', InstallScheduler },
    { 'RDYQ', StubInstaller    }, { 'BLKQ', StubInstaller    },
    { 'RTC ', InstallClock     }, { 'KDB ', InstallKeyboard  },
    { 'COM1', InstallCom1      }, { 'COM2', InstallCom2      },
    { 'MOUS', InstallMouse     }, { 'SND ', InstallSpeaker   }
};

CInstaller enable[] = {
    { 'IVT ', StubInstaller    }, { 'BIOS', StubInstaller    },
    { 'GDT ', EnableGdt        }, { 'TSS ', EnableTss        },
    { 'IRQ ', StubInstaller    }, { 'ISR ', StubInstaller    },
    { 'IDT ', EnableIdt        },

    { 'PAGE', EnablePaging     },
    { 'RDYQ', StubInstaller    }, { 'BLKQ', StubInstaller    },
    { 'VESA', EnableVesa       }, { 'SND ', EnableSpeaker    },
    { 'COM1', StubInstaller    }, { 'COM2', EnableSerial     },
    { 'RTC ', EnableClock      }, { 'KDB ', EnableKeyboard   },
    { 'MOUS', EnableMouse      }
};

bool BootStrap(void *grub) {
    if (!BootLoad->Grub(grub)) return false;
    if (!EnableRamDisk(Monitor->GetBase('DISK'), Monitor->GetSize('DISK'))) return false;
    if (!BootLoad->Prepare(prepare, elementsof(prepare))) return false;
    if (!BootLoad->Enable(enable, elementsof(enable))) return false;

    if (!BootLoad->Activate()) return false;
    return true;
}