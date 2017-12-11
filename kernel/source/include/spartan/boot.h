#ifndef BOOT_H
#define BOOT_H
#include <stdint.h>
#pragma pack(push, 1)
#define MB_SEARCH        8192
#define MB_HEADER_ALIGN  4
#define MB_HEADER_MAGIC  0x1BADB002
#define MB_BOOT_MAGIC    0x2BADB002
#define MB_MOD_ALIGN     0x00001000
#define MBI_ALIGN        0x00000004
#define MB_PAGE_ALIGN    0x00000001
#define MB_MEMORY_INFO   0x00000002
#define MB_VIDEO_MODE    0x00000004
#define MB_AOUT_KLUDGE   0x00010000
#define MBI_MEMORY       0x00000001
#define MBI_BOOTDEV      0x00000002
#define MBI_CMDLINE      0x00000004
#define MBI_MODS         0x00000008
#define MBI_AOUT_SYMS    0x00000010
#define MBI_ELF_SHDR     0X00000020
#define MBI_MEM_MAP      0x00000040
#define MBI_DRIVE_INFO   0x00000080
#define MBI_CONFIG_TABLE 0x00000100
#define MBI_BOOT_LOADER  0x00000200
#define MBI_APM_TABLE    0x00000400
#define MBI_VBE_INFO     0x00000800
#define MBI_FRAMEBUFFER  0x00001000

typedef unsigned char      u8;
typedef unsigned short     u16;
typedef unsigned int       u32;
typedef unsigned long long u64;

typedef struct MULTIBOOT {
    u32 Magic;
    u32 Flags;
    u32 CheckSum;
    u32 HeaderAddr;
    u32 LoadAddr;
    u32 LoadEndAddr;
    u32 BssEndAddr;
    u32 EntryAddr;
    u32 ModeType;
    u32 Width;
    u32 Height;
    u32 Depth;
} MULTIBOOT;

typedef struct AOUT_SYM {
    u32 TabSize;
    u32 StrSize;
    u32 Addr;
    u32 Reserved;
} AOUT_SYM;

typedef struct ELF_SYM {
    u32 Num;
    u32 Size;
    u32 Addr;
    u32 Shndx;
} ELF_SYM;

typedef struct BOOT_INFO {
    u32 Flags;
    u32 MemLower;
    u32 MemUpper;
    u32 BootDevice;
    u32 CmdLine;
    u32 ModsCount;
    u32 ModsAddr;
    union {
        AOUT_SYM AoutSym;
        ELF_SYM  ElfSec;
    } u;
    u32 MmapLength;
    u32 MmapAddr;
    u32 DrivesLength;
    u32 DrivesAddr;
    u32 ConfigTable;
    u32 BootLoaderName;
    u32 ApmTable;
    u32 VesaInfo;
    u32 ModeInfo;
    u16 VbeMode;
    u16 VbeSeg;
    u16 VbeOff;
    u16 VbeLen;
    u64 FramebufferAddr;
    u32 FramebufferPitch;
    u32 FramebufferResX;
    u32 FramebufferResY;
    u8  FramebufferBpp;
    #define MB_FRAMEBUFFER_TYPE_INDEXED      0
    #define MB_FRAMEBUFFER_TYPE_RGB          1
    #define MB_FRAMEBUFFER_TYPE_EGA_TEXT     2
    u8 FramebufferType;
    union {
        struct {
            u32 FramebufferPaletteAddr;
            u16 FramebufferPaletteNumColors;
        };
        struct {
            u8 FramebufferRedFieldPosition;
            u8 FramebufferRedMaskSize;
            u8 FramebufferGreenFieldPosition;
            u8 FramebufferGreenMaskSize;
            u8 FramebufferBlueFieldPosition;
            u8 FramebufferBlueMaskSize;
        };
    };
} BOOT_INFO;

typedef struct MB_COLOUR {
    u8 Red;
    u8 Green;
    u8 Blue;
} MB_COLOUR;

typedef struct MB_MMAP {
    u32 Size;
    u64 Addr;
    u64 Len;
    #define MB_MEM_AVAILABLE              1
    #define MB_MEM_RESERVED               2
    #define MB_MEM_ACPI_RECLAIMABLE       3
    #define MB_MEM_NVS                    4
    #define MB_MEM_BADRAM                 5
    u32 Type;
} MB_MMAP;

typedef struct MB_MOD {
    u32 ModStart;
    u32 ModEnd;
    u32 CmdLine;
    u32 Pad;
} MB_MOD;

typedef struct MB_APM_INFO {
    u16 Version;
    u16 Cseg;
    u32 Offset;
    u16 Cseg16;
    u16 Dseg;
    u16 Flags;
    u16 Cseg_Len;
    u16 Cseg16_Len;
    u16 DsegLen;
} MB_APM_INFO;

typedef struct MBHDR {
    uint32_t Magic;
    uint32_t Flags;
    uint32_t Check;
    uint32_t Header;
    uint32_t Text_base;
    uint32_t Text_end;
    uint32_t Bss_end;
    uint32_t Entry;
    uint32_t VesaType;
    uint32_t VesaResX;
    uint32_t VesaResY;
    uint32_t VesaBits;
    // spartan additions
    uint32_t Magic2;
    uint32_t Info;
    uint32_t MzHdr;
    uint32_t InitRd;
} MBHDR;

#define MBHDR_MAGIC  (0x1BADB002)
#define MBHDR_FLAGS  (0x00010007)
#define MBHDR_CHECK  (-(MBHDR_MAGIC+MBHDR_FLAGS))

#pragma pack(pop)
#endif // BOOT_H