#ifndef PIT_H
#define PIT_H

/* ************************************************************************** */
/* Programmable Interval Timer Port Mappings                                  */
/* ************************************************************************** */
#define PIT_COMMAND 0x43 // Mode/Command register (write only, a read is ignored)
#define PIT_TIMER0  0x40 // Channel 0 data port (read/write)
#define PIT_TIMER1  0x41 // Channel 1 data port (read/write)
#define PIT_TIMER2  0x42 // Channel 2 data port (read/write)
#define NMI_CTRL    0x61

/* ************************************************************************** */
/* Command Control Word                                                       */
/* ************************************************************************** */
#define PIT_CHANNEL 0xC0
#define PIT_LATCH   0x30
#define PIT_OP      0x0E
#define PIT_BCD     0x01

#define PIT_CH0     0x00 // 0 0 = Channel 0
#define PIT_CH1     0x40 // 0 1 = Channel 1
#define PIT_CH2     0x80 // 1 0 = Channel 2
#define PIT_READ2   0xC0 // 1 1 = Read-back command (8254 only)

#define PIT_COUNT   0x00 // 0 0 = Latch count value command
#define PIT_LOBYTE  0x10 // 0 1 = Access mode: lobyte only
#define PIT_HIBYTE  0x20 // 1 0 = Access mode: hibyte only
#define PIT_BOTH    0x30 // 1 1 = Access mode: lobyte/hibyte


#define PIT_IOTC    0x00 // 0 0 0 = Mode 0 (interrupt on terminal count)
#define PIT_HWOS    0x02 // 0 0 1 = Mode 1 (hardware re-triggerable one-shot)
#define PIT_RATE    0x04 // 0 1 0 = Mode 2 (rate generator)
#define PIT_SQWV    0x06 // 0 1 1 = Mode 3 (square wave generator)
#define PIT_SWTS    0x08 // 1 0 0 = Mode 4 (software triggered strobe)
#define PIT_HWTS    0x0A // 1 0 1 = Mode 5 (hardware triggered strobe)
#define PIT_RATE2   0x0C // 1 1 0 = Mode 2 (rate generator, same as 010b)
#define PIT_SQWV2   0x0E // 1 1 1 = Mode 3 (square wave generator, same as 011b)

#endif PIT_H