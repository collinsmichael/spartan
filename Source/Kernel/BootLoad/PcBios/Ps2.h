#ifndef PS2_H
#define PS2_H
#pragma pack(push, 1)

typedef struct CMouse {
    int PosX;
    int PosY;
    int Wheel;
    int Left;
    int Middle;
    int Right;
    int DeltaX;
    int DeltaY;
} CMouse;

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

#define PS2_SETLED   0xED
#define PS2_SCRLOCK  0x01
#define PS2_NUMLOCK  0x02
#define PS2_CAPLOCK  0x04

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

// MOUSE PACKET FORMATS
//
// Data Packet Format
// For reference, the standard PS/2-compatible mouse data packet format is as shown in the following table.
// 
// Data Packet Format for PS/2-compatible Mouse
// 
// Byte    D7     D6     D5     D4     D3    D2  D1  D0  Comment
// 1       Yover  Xover  Ysign  Xsign  Tag   M   R   L   X/Y overflows and signs, buttons
// 2       X7     X6     X5     X4     X3    X2  X1  X0  X data byte
// 3       Y7     Y6     Y5     Y4     Y3    Y2  Y1  Y0  Y data byte
// 
// Windows mouse drivers do not check the overflow bits. In case of overflow, the mouse should simply send the maximal signed displacement value.
// Also for reference, the data packet format for an IntelliMouse-compatible 3-button wheel mouse is as shown in the following table.
// 
// Data Packet Format for 3-button Wheel Mouse
// Byte    D7    D6    D5      D4     D3   D2   D1   D0   Comment
// 1       0     0     Ysign   Xsign  1    M    R    L    X/Y signs and R/L/M buttons
// 2       X7    X6    X5      X4     X3   X2   X1   X0   X data byte
// 3       Y7    Y6    Y5      Y4     Y3   Y2   Y1   Y0   Y data byte
// 4       Z7    Z6    Z5      Z4     Z3   Z2   Z1   Z0   Z/wheel data byte
// 
// The new data packet format for a 5-button wheel mouse should be as shown in the following table.
// Data Packet Format for 5-button Wheel Mouse
// Byte    D7    D6    D5      D4     D3   D2   D1   D0   Comment
// 1       0     0     Ysign   Xsign  1    M    R    L    X/Y signs and L/R/M buttons
// 2       X7    X6    X5      X4     X3   X2   X1   X0   X data byte
// 3       Y7    Y6    Y5      Y4     Y3   Y2   Y1   Y0   Y data byte
// 4       0     0     B5      B4     Z3   Z2   Z1   Z0   Z/wheel data and buttons 4 and 5
// Notice that the Z/wheel data for a 5-button wheel mouse has been reduced to four bits instead of the 8 bits used in the IntelliMouse-compatible 3-button wheel mode. This reduction is made possible by the fact that the wheel typically cannot generate values beyond the range +7/-8 during any given interrupt period. Windows mouse drivers will sign extend the four Z/wheel data bits when the mouse is in the 5-button wheel mode, and the full Z/wheel data byte when the mouse operates in the 3-button wheel mode.

#pragma pack(pop)
#endif PS2_H