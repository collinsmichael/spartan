#ifndef COM_H
#define COM_H
#pragma pack(push, 1)

#define COM1 0x3F8
#define COM2 0x2F8
#define COM3 0x3E8
#define COM4 0x2E8

#define UART_TX   0x00 // W=Transmitter Holding Buffer R=Receiver Buffer
#define UART_DLL  0x00 // Divisor Latch Low
#define UART_DLH  0x01 // Divisor Latch High
#define UART_IE   0x01 // Interrupt Enable
#define UART_IIR  0x02 // Interrupt Identification Register
#define UART_FCR  0x02 // FIFO Control Register
#define UART_LCR  0x03 // Line Control Register
#define UART_MCR  0x04 // Modem Control Register
#define UART_LSR  0x05 // Line Status Register
#define UART_MSR  0x06 // Modem Status Register
#define UART_SR   0x07 // Scratch Register

// Typical values for DLL and DLH registers
#define BAUD_115200 0x0001
#define BAUD_57600  0x0002
#define BAUD_38400  0x0003
#define BAUD_19200  0x0006
#define BAUD_9600   0x000C
#define BAUD_4800   0x0018
#define BAUD_2400   0x0030

// Interrupt Enable Register (IER)
#define UART_IE_RC 0x01 // Enable Received Data Available Interrupt
#define UART_IE_TX 0x02 // Enable Transmitter Holding Register Empty Interrupt
#define UART_IE_LS 0x04 // Enable Receiver Line Status Interrupt
#define UART_IE_MS 0x08 // Enable Modem Status Interrupt
#define UART_IE_SM 0x10 // Enables Sleep Mode (16750)
#define UART_IE_LP 0x10 // Enables Low Power Mode (16750)

// FIFO Control Register (FCR)
// 7 & 6       Interrupt Trigger Level (16 byte)
//      0  0    1 Byte    1 Byte
//      0  1    4 Bytes  16 Bytes
//      1  0    8 Bytes  32 Bytes
//      1  1   14 Bytes  56 Bytes
// 5  Enable 64 Byte FIFO (16750)
// 4  Reserved
// 3  DMA Mode Select
// 2  Clear Transmit FIFO
// 1  Clear Receive FIFO
// 0  Enable FIFOs


// Line Control Register (LCR)
// 7  Divisor Latch Access Bit
// 6  Set Break Enable
// 3, 4 & 5	     Parity Select
//     0  0  0   No Parity
//     0  0  1   Odd Parity
//     0  1  1   Even Parity
//     1  0  1   Mark
//     1  1  1   Space
// 2  0  One Stop Bit
//    1  1.5 Stop Bits or 2 Stop Bits
// 0 & 1     Word Length
//    0  0	 5 Bits
//    0  1	 6 Bits
//    1  0	 7 Bits
//    1  1	 8 Bits


// Modem Control Register (MCR)
#define UART_MCR_AUTOFLOW 0x20 // Autoflow Control Enabled (16750)
#define UART_MCR_LOOPBACK 0x10 // Loopback Mode
#define UART_MCR_AUXOUT2  0x08 // Auxiliary Output 2
#define UART_MCR_AUXOUT1  0x04 // Auxiliary Output 1
#define UART_MCR_REQSEND  0x02 // Request To Send
#define UART_MCR_DATREADY 0x01 // Data Terminal Ready

// Line Status Register (LSR)
#define UART_LSR_RECV    0x80 // Error in Received FIFO
#define UART_LSR_HOLD    0x40 // Empty Data Holding Registers
#define UART_LSR_TX      0x20 // Empty Transmitter Holding Register
#define UART_LSR_BREAK   0x10 // Break Interrupt
#define UART_LSR_FRAME   0x08 // Framing Error
#define UART_LSR_PARITY  0x04 // Parity Error
#define UART_LSR_OVERRUN 0x02 // Overrun Error
#define UART_LSR_READY   0x01 // Data Ready

// Modem Status Register (MSR)
#define UART_MSR_CD   0x80 // Carrier Detect
#define UART_MSR_RI   0x40 // Ring Indicator
#define UART_MSR_DSR  0x20 // Data Set Ready
#define UART_MSR_CTS  0x10 // Clear To Send
#define UART_MSR_DDCD 0x08 // Delta Data Carrier Detect
#define UART_MSR_TERI 0x04 // Trailing Edge Ring Indicator
#define UART_MSR_DCSR 0x02 // Delta Data Set Ready
#define UART_MSR_DCTS 0x01 // Delta Clear To Send

#pragma pack(pop)
#endif // COM_H