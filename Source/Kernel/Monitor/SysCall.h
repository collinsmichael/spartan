#ifndef KERNEL_POSIX_SYSCALL_H
#define KERNEL_POSIX_SYSCALL_H
#include <oop.h>
#pragma pack(push, 1)

typedef struct REG32 {
    u32 eax;
    u32 ecx;
    u32 edx;
    u32 ebx;
    u32 esp;
    u32 ebp;
    u32 esi;
    u32 edi;
} REG32;

#ifndef IMPL_ISYSCALL
int SysCall(int eax, int ebx, int ecx, int edx, int ebp, int esi, int edi);
bool EnableSysCalls(void);
#endif

#define SYS_YIELD 0x8000
#define SYS_KILL  0x8001
#define SYS_FORK  0x8002
#define SYS_BLIT  0x8003

#pragma pack(pop)
#endif KERNEL_POSIX_SYSCALL_H