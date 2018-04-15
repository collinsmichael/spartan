#ifndef KERNEL_BOOTLOAD_PCBIOS_CMOS_H
#define KERNEL_BOOTLOAD_PCBIOS_CMOS_H

#define CMOS_INDEX 0x70
#define CMOS_DATA  0x71
#define CMOS_MASK  0x80
#define CMOS_RTC_A 0x0A
#define CMOS_RTC_B 0x0B
#define CMOS_RTC_C 0x0C
#define CMOS_INT   0x40

#define CMOS_SECOND   0x00
#define CMOS_MINUTE   0x01
#define CMOS_HOUR     0x02
#define CMOS_DAY      0x03
#define CMOS_MONTH    0x04
#define CMOS_YEAR     0x05
#define CMOS_CENTURY  0x06
#define CMOS_STATUSA  0x07
#define CMOS_STATUSB  0x08
#define CMOS_NO_NMI   0x80

#endif KERNEL_BOOTLOAD_PCBIOS_CMOS_H