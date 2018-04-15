#ifndef RTC_H
#define RTC_H

#define CMOS_INDEX 0x70
#define CMOS_DATA  0x71
#define CMOS_MASK  0x80
#define CMOS_RTC_A 0x0A
#define CMOS_RTC_B 0x0B
#define CMOS_RTC_C 0x0C
#define CMOS_INT   0x40

#define RTC_32768HZ 0x01
#define RTC_16384HZ 0x02
#define RTC_8192HZ  0x03
#define RTC_4096HZ  0x04
#define RTC_2048HZ  0x05
#define RTC_1024HZ  0x06
#define RTC_512HZ   0x07
#define RTC_256HZ   0x08
#define RTC_128HZ   0x09
#define RTC_64HZ    0x0A
#define RTC_32HZ    0x0B
#define RTC_16HZ    0x0C
#define RTC_8HZ     0x0D
#define RTC_4HZ     0x0E
#define RTC_1HZ     0x0F

#endif RTC_H