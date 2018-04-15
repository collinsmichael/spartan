#define IMPL_ISYSCALL
#include <oop.h>
#include <Kernel.h>

extern IDevice *Device;

u32  DoYield(u32 err, u32 esp);
u32  DoKill(u32 err, u32 esp);
u32  DoFork(u32 err, u32 esp, char *path);
void SysCall_Blit(int params[]);

int int386(int i, REG32 *reg);

int SysCall(int eax, int ebx, int ecx, int edx, int ebp, int esi, int edi) {
    REG32 reg;
    reg.eax = eax;
    reg.ebx = ebx;
    reg.ecx = ecx;
    reg.edx = edx;
    reg.ebp = ebp;
    reg.esi = esi;
    reg.edi = edi;
    return int386(0x80, &reg);
}

u32 DoSysCall(u32 err, u32 esp) {
    REG32 *x;
    _asm mov [x], eax

    switch (x->eax) {
    case SYS_YIELD: esp = DoYield(err, esp);               break;
    case SYS_KILL:  esp = DoKill(err, esp);                break;
    case SYS_FORK:  esp = DoFork(err, esp, (char*)x->ebx); break;
    case SYS_BLIT:        SysCall_Blit((int*)x->ebx);      break;
    }
    return esp;
}

bool EnableSysCalls(void) {
    Device->Latch(0x80, DoSysCall);
    return true;
}