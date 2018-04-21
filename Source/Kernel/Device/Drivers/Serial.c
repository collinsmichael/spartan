#define IMPL_IPIPE
#include <Kernel.h>

extern IDevice *Device;
extern IPipe   *Pipe;
static CPipeAsync *com1_recv;
static CPipeAsync *com1_send;
static CPipeAsync *com2_recv;
static CPipeAsync *com2_send;

export
bool SerialPutb(CPipeAsync *pipe) {
    int port = pipe->Lock;
    while (pipe->Head != pipe->Tail) {
        if (pipe->Consumed == 0) pipe->Tail++;
        else pipe->Consumed--;
        u8 c  = pipe->Pointer[pipe->Head % pipe->Capacity];
        pipe->Head++;

        while ((inb(port+UART_LSR) & 0x20) == 0); // wait for byte to be sent
        outb(port+UART_TX, c); // send byte to serial port
    }
    return true;
}

u32 Serial(u32 esp, CPipeAsync *pipe) {
    int port = pipe->Lock;
    char c = inb(port+UART_LSR); // check to see if char has been received
    if (c & 1) {
        char byte = inb(port+UART_TX);
        Pipe->WriteAsync(pipe, &byte, 1);
    }
    return esp;
}

u32 SerialCom1(u32 err, u32 esp) {
    return Serial(esp, com1_recv);
}

u32 SerialCom2(u32 err, u32 esp) {
    return Serial(esp, com2_recv);
}

bool EnableCom(u32 port) {
    outb(port+UART_IE, UART_IE_RC); // Enable Recieve Data Interrupt
    outb(port+UART_LCR, 0x80); // SET DLAB ON
    outb(port+UART_DLL, 0x0C); // Set Baud rate - Divisor Latch Low Byte
    outb(port+UART_DLH, 0x00); // Set Baud rate - Divisor Latch High Byte
    outb(port+UART_LCR, 0x03); // 8 Bits, No Parity, 1 Stop Bit
    outb(port+UART_FCR, 0xC7); // FIFO Control Register
    outb(port+UART_MCR, 0x0B); // Turn on DTR, RTS, and OUT2
    return true;
}

bool InstallCom1(u8 *base, u32 size) {
    stosd(base, 'xxxx', size/4);

    com1_recv = (CPipeAsync*)&base[0*KB];
    Pipe->CreateAsync(com1_recv, nullptr, SerialPutb);
    com1_recv->Writer = (void*)SerialPutb;
    com1_recv->Lock = COM1;

    com1_send = (CPipeAsync*)&base[4*KB];
    Pipe->CreateAsync(com1_send, SerialPutb, nullptr);
    com1_send->Reader = (void*)SerialPutb;
    com1_send->Lock = COM1;
    return true;
}

bool InstallCom2(u8 *base, u32 size) {
    stosd(base, 'xxxx', size/4);

    com2_recv = (CPipeAsync*)&base[0*KB];
    Pipe->CreateAsync(com2_recv, nullptr, SerialPutb);
    com2_recv->Writer = (void*)SerialPutb;
    com2_recv->Lock = COM2;

    com2_send = (CPipeAsync*)&base[4*KB];
    Pipe->CreateAsync(com2_send, SerialPutb, nullptr);
    com2_send->Reader = (void*)SerialPutb;
    com2_send->Lock = COM2;
    return true;
}

bool EnableSerial(u8 *base, u32 size) {
    if (!EnableCom(COM1)) return false;
    if (!EnableCom(COM2)) return false;
    Device->Latch(IRQ_COM2, SerialCom2);
    Device->Latch(IRQ_COM1, SerialCom1);
    return true;
}