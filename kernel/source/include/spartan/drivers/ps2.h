/* ************************************************************************** */
/* author(s) : Michael Collins                                                */
/* file name : include/spartan/pc/cga.h                                       */
/* file date : 2017/12/03                                                     */
/* file info : Personal System 2 Keyboard and Mouse Interface                 */
/* ************************************************************************** */
#ifndef PS2_H
#define PS2_H
#include <stdint.h>
#pragma pack(push, 1)

#define KB_IRQ 0x21
#define MS_IRQ 0x2C


#define PS2_STATUS 0x0064 // 8bit rw
#define PS2_DATA   0x0060 // 8bit rw

#define PS2_DATA_FULL 0x01 // data port is full (read port 0x60 to flush)
#define PS2_CMD_FULL  0x02 // command port is full (do not write to port 0x64)
#define PS2_SYS_FLAG  0x04 // 
#define PS2_CMD_DATA  0x08 // Writing to port 0x64 sets this bit to 1
#define PS2_INHIBIT   0x10 // Password inhibited input
#define PS2_AUX_FULL  0x20 // Mouse data is full (read port 0x60 to flush)
#define PS2_TIMEOUT   0x40 // General timeout
#define PS2_PARITY    0x80 // Parity error

// KEYBOARD INITIALIZATION
//   A8 ENABLE AUXILIARY
//   AE ENABLE KEYBOARD
//   20 READ CONTROLLER RAM
//   60 WRITE CONTROLLER RAM
//        7 6 5 4 3 2 1 0 
//        ¦ ¦ ¦ ¦ ¦ ¦ ¦ ¦
//        ¦ ¦ ¦ ¦ ¦ ¦ ¦ +-? 0: Enable Keyboard
//        ¦ ¦ ¦ ¦ ¦ ¦ +---? 1: Enable Auxialiary Device
//        ¦ ¦ ¦ ¦ ¦ +-----? 2: System Flag
//        ¦ ¦ ¦ ¦ +-------? 3: Reserved
//        ¦ ¦ ¦ +---------? 4: Disable Keyboard
//        ¦ ¦ +-----------? 5: Disable Auxiliary Device
//        ¦ +-------------? 6: Keyboard Translate
//        +---------------? 7: Reserved
//   D4 WRITE TO AUXILIARY DEVICE
//   F4 WRITE TO AUXILIARY DEVICE

void ps2_reset(void);

#pragma pack(pop)
#endif // PS2_H