#ifndef KERNEL_MEMORY_MONITOR_H
#define KERNEL_MEMORY_MONITOR_H
#include <oop.h>
#pragma pack(push, 1)

typedef struct CMemMap {
    int name;
    int base;
    int size;
    int copy;
    int crc;
} CMemMap;

typedef struct IMonitor {
    bool  (*EnableErrorRecovery)(void);
    bool  (*ClearMemory)(void);
    u32   (*CalculateCrc)(u8 *base, u32 size);
    bool  (*Recover)(int owner);
    bool  (*Verify )(int owner);
    bool  (*Protect)(int owner);
    char *(*GetBase)(int owner);
    u32   (*GetSize)(int owner);
    u32   (*GetCrc )(int owner);

    bool  (*ProtectAll)(void);
    bool  (*RecoverAll)(void);
} IMonitor;

bool Format(char *text, char *format, ... );

#pragma pack(pop)
#endif KERNEL_MEMORY_MONITOR_H