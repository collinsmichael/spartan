#ifndef KERNEL_MEMORY_MEMMAP_H
#define KERNEL_MEMORY_MEMMAP_H
#include <oop.h>

// *********************************************************************************
// RING0 MEMORY MAP                            START      END      SIZE     COMMENTS
// *********************************************************************************
#define MMAP_CLEARSTART   ((u8*)0x000006C0) // 00000400 - 00000500 00000100 (BIOS DATA AREA)
#define MMAP_CLEARFINISH  ((u8*)0x00080000) // 00000400 - 00000500 00000100 (BIOS DATA AREA)
#define MMAP_KSTART       ((u8*)0x00001000) // 00000500 - 00000800 00000700 (Start of Kernel Data)
#define MMAP_KSTACK       ((u8*)0x0008E000) // 0008E000 - 0008F000 00001000 (Some Systems extend the BIOS into this area)
#define MMAP_TOPSTACK     ((u8*)0x0008FFF0) // 0008F000 - 00090000 00001000 (Some Systems extend the BIOS into this area)
#define MMAP_KFINISH      ((u8*)0x0008F000) // 00090000 - 0009FC00 0000FC00 (End of Kernel Data)
#define MMAP_USER         ((u8*)0x02000000) // 01000000 - ???????? ???????? (Ring3 User Mode Memory)

#define RING0_CODE   (0x08+0)
#define RING0_DATA   (0x10+0)
#define RING3_CODE   (0x18+3)
#define RING3_DATA   (0x20+3)
#define RM16_CODE    (0x28+3)
#define RM16_DATA    (0x30+3)
#define RING3_TASK   (0x38+3)

#define MMAP_HEAPPOOL ((CPool*)0x0600)
#define MMAP_PIPEPOOL ((CPool*)0x0640)
#define MMAP_TASKPOOL ((CPool*)0x0680)

#endif KERNEL_MEMORY_MEMMAP_H