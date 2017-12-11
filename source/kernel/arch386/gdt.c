#include <stdint.h>


//00020000-0002FFFF (64KB)
//00040000-00067FFF (96KB)
//00080000-0009EFFF

/*
#define GDT ((uint64_t*)(0x00000000))

// sets up physical segments (global descriptor table)
int gdt_init(void) {
    GDT[0] = 0x0000000000000000; // null
    GDT[1] = 0x00CF92000000FFFF; // data flat
    GDT[2] = 0x00009A000000FFFF; // code real
    GDT[3] = 0x000092000000FFFF; // data real
    GDT[4] = 0x00CF9A000000FFFF; // code ring0
    GDT[5] = 0x00CF92000000FFFF; // data ring0
    GDT[6] = 0x00DFFA000000FFFF; // code ring3
    GDT[7] = 0x00DFF2000000FFFF; // data ring3
    GDT[8] = 0x000089000700FFFF; // task ring0
    GDT[9] = 0x0000E9000780FFFF; // task ring3
    lgdt(GDT, 10*sizeof(uint64_t)-1);
    return 1;
}*/