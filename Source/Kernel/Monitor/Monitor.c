#define IMPL_IMONITOR
#include <Kernel.h>

#define NAME 0x00
#define BASE 0x01
#define SIZE 0x02
#define COPY 0x03
#define CRC  0x04

static u32 map[][5] = {
    { 'IVT ', 0x00000000,  4*KB },
    { 'BIOS', 0x00001000,  4*KB },

    { 'PAGE', 0x00002000,  8*KB }, { 'GDT ', 0x00004000,  4*KB },
    { 'TSS ', 0x00005000,  4*KB }, { 'IDT ', 0x00006000,  4*KB },
    { 'IRQ ', 0x00007000,  4*KB }, { 'ISR ', 0x00008000, 16*KB },


    { 'TASK', 0x0000F000,  4*KB }, { 'RDYQ', 0x00010000, 16*KB },
    { 'BLKQ', 0x00014000, 16*KB }, { 'KILL', 0x00018000, 16*KB },
    { 'CTX ', 0x0001C000, 16*KB }, { 'PCB ', 0x00020000, 16*KB },
    { 'PEB ', 0x00024000, 16*KB },
    { 'TBL3', 0x00028000, 16*KB },

    { 'COM1', 0x00030000,  8*KB },
    { 'COM2', 0x00032000,  8*KB },
    { 'KDB ', 0x00034000,  4*KB },
    { 'MOUS', 0x00035000,  4*KB },
    { 'RTC ', 0x00036000,  4*KB },
    { 'VESA', 0x00037000,  4*KB },
    { 'SND ', 0x00038000, 32*KB },

    { 'TBL0', 0x00800000,  4*MB },
    { 'HEAP', 0x00C00000,  1*MB },

    { 'DISK', 0x00100000, 13*MB },
    { 'STAK', 0x0008E000,  4*KB },
    { 'TOPS', 0x0008F000,  4*KB }
};

int IMonitor_GetId(int owner) {
    for (int id = 0; id < elementsof(map); id++) {
        if (owner == map[id][NAME]) return id;
    }
    return 0;
}

u32 IMonitor_CalculateCrc(u8 *base, u32 size) {
    char h[4] = "\x00\x00\x00\x00";
    char m[4] = "\x2C\x3B\x5A\x69";
    u32 *crc = (int*)h;
    for (char *t = base; size > 0; size -= 4, t += 4) {
        // 2x2 matrix multiplication spreads the bits
        h[0] ^= t[0]*m[0] + t[1]*m[2];
        h[1] ^= t[0]*m[1] + t[1]*m[3];
        h[2] ^= t[2]*m[0] + t[3]*m[2];
        h[3] ^= t[2]*m[1] + t[3]*m[3];
        // rotate left further spreads the bits
        crc[0] += rol(crc[0], 13);
    }
    return crc[0];
}

bool IMonitor_Recover(int owner) {
    int id = IMonitor_GetId(owner);
    if (id < 0 || id >= elementsof(map)) return false;

    u8 *base = (u8*)map[id][BASE];
    u8 *copy = (u8*)map[id][COPY];
    u32 size = (u32)map[id][SIZE];

    u32 crc1 = IMonitor_CalculateCrc(base, size);
    if (crc1 == map[id][CRC]) {
        movsd(copy, base, size/4);
        return true;
    }

    u32 crc2 = IMonitor_CalculateCrc(copy, size);
    if (crc2 == map[id][CRC]) {
        movsd(base, copy, size/4);
        return true;
    }
    return false;
}

bool IMonitor_Verify(int owner) {
    int id = IMonitor_GetId(owner);
    if (id < 0 || id >= elementsof(map)) return false;

    u8 *base = (u8*)map[id][BASE];
    u32 size = map[id][SIZE];

    u32 crc = IMonitor_CalculateCrc(base, size);
    bool status = (crc == map[id][CRC]);

    #ifndef OPTIMIZED
    u64 totext = (u64)owner;
    char *text = (status) ? "OK" : "Failed Cycle Redundency Check";
    Logger(" Verifying  %s ... %s (CRC = %X)\n", &totext, text, crc);
    #endif
    return status;
}

bool IMonitor_Protect(int owner) {
    int id = IMonitor_GetId(owner);
    if (id < 0 || id >= elementsof(map)) return false;
    u8 *base = (u8*)map[id][BASE];
    u8 *copy = (u8*)map[id][COPY];
    u32 size = (u32)map[id][SIZE];

    u32 crc = IMonitor_CalculateCrc(base, size);
    map[id][CRC] = crc;
    movsd(copy, base, size/4);

    #ifndef OPTIMIZED
    u64 totext = (u64)owner;
    Logger(" Updating   %s ... OK (CRC = %X)\n", &totext, crc);
    #endif
    return true;
}

char *IMonitor_GetBase(int owner) {
    int id = IMonitor_GetId(owner);
    return (id < 0 || id >= elementsof(map)) ? 0 : (char*)map[id][BASE];
}

char *IMonitor_GetCopy(int owner) {
    int id = IMonitor_GetId(owner);
    return (id < 0 || id >= elementsof(map)) ? 0 : (char*)map[id][COPY];
}

u32 IMonitor_GetSize(int owner) {
    int id = IMonitor_GetId(owner);
    return (id < 0 || id >= elementsof(map)) ? 0 : map[id][SIZE];
}

u32 IMonitor_GetCrc(int owner) {
    char *base = IMonitor_GetBase(owner);
    int size = IMonitor_GetSize(owner);
    return IMonitor_CalculateCrc(base, size);
}

bool IMonitor_EnableErrorRecovery(void) {
    for (int m = 0; m < elementsof(map); m++) {
        map[m][COPY] = map[m][BASE] + 256*KB;
    }
    return true;
}

bool IMonitor_ClearMemory(void) {
    char *base = IMonitor_GetBase('VESA');
    char *copy = IMonitor_GetCopy('VESA');
    int size = IMonitor_GetSize('VESA');
    movsd(MMAP_CLEARFINISH, base, size/4);
    stosd(MMAP_CLEARSTART, 'xxxx', (MMAP_CLEARFINISH-MMAP_CLEARSTART)/4);
    movsd(base, MMAP_CLEARFINISH, size/4);
    movsd(copy, MMAP_CLEARFINISH, size/4);
    stosd(MMAP_CLEARSTART, 'xxxx', size/4);
    return true;
}

bool IMonitor_ProtectAll(void) {
    Logger(" [info] Creating Restore Point\n");
    for (int id = 0; id < elementsof(map); id++) {
        if (map[id][NAME] == 'DISK') break;

        int   size = map[id][SIZE];
        char *base = (char*)map[id][BASE];
        char *save = (char*)(base + 16*MB);
        movsd(save, base, size/4);
      //map[id][CRC] = IMonitor_CalculateCrc(base, size);
    }
    Logger(" [info] Restore Point Created\n");
    return true;
}

bool IMonitor_RecoverAll(void) {
    Logger(" [info] Restoring Restore Point\n");
    for (int id = 0; id < elementsof(map); id++) {
        if (map[id][NAME] == 'DISK') break;

        int   size = map[id][SIZE];
        char *base = (char*)map[id][BASE];
        char *save = (char*)(base + 16*MB);
        movsd(base, save, size/4);
    }
    Logger(" [info] Restore Point Restored\n");
    return true;
}

IMonitor monitor = {
    IMonitor_EnableErrorRecovery,
    IMonitor_ClearMemory,
    IMonitor_CalculateCrc,
    IMonitor_Recover,
    IMonitor_Verify,
    IMonitor_Protect,
    IMonitor_GetBase,
    IMonitor_GetSize,
    IMonitor_GetCrc,
    IMonitor_ProtectAll,
    IMonitor_RecoverAll
};

export IMonitor *Monitor = &monitor;