#include <Library.h>
#include <Kernel.h>
#include <Shell.h>

import IMemory  *Memory;
import IProcess *Process;
import IMonitor *Monitor;
import IPool    *Pool;
import IPipe    *Pipe;
import IDevice  *Device;

bool IPipe_Debug(CPipe *pipe);

static int  recvpos;
static char recvbuf[8];
static int  sendpos;
static char sendbuf[8];
static CPipeAsync *com1_recv;
static CPipeAsync *com1_send;
static CPipeAsync *com2_recv;
static CPipeAsync *com2_send;
static CPipeAsync *pipes[256];

bool initialized = false;

static bool INetwork_GetB(CPipe *pipe) {
    if (pipe->Consumed < sizeof(CMouse)) return true;
    for (u32 i = 0; i < pipe->Consumed; i++) {
        recvbuf[recvpos++] = pipe->Pointer[(pipe->Head + i) % pipe->Capacity];
        if (recvpos == 2) {
            recvpos = 0;
            u8 port = recvbuf[0];
            u8 byte = recvbuf[1];
            if (port != 0) {
                Pipe->WriteAsync(pipes[port], &byte, 1);
            }
        }
    }
    pipe->Head = pipe->Tail;
    pipe->Consumed = 0;
    return true;
}

bool INetwork_PutB(CPipeAsync *pipe) {
    while (pipe->Head != pipe->Tail) {
        if (pipe->Consumed == 0) pipe->Tail++;
        else pipe->Consumed--;

        sendbuf[sendpos++] = pipe->Pointer[pipe->Head % pipe->Capacity];
        pipe->Head++;
        if (sendpos == 2) {
            sendpos = 0;
            u8 port = sendbuf[0];
            u8 byte = sendbuf[1];
            if (port != 0) {
                Pipe->WriteAsync(com1_send, sendbuf, 2);
            }
        }
    }
    return true;
}

bool INetwork_Install(void) {
    if (initialized == true) return true;

    CPeb *peb = Process->GetPeb();
    if (!peb) return false;

    char *com1 = Monitor->GetBase('COM1');
    char *com2 = Monitor->GetBase('COM2');
    if (!com1 || !com2) return false;
    com1_recv = (CPipeAsync*)&com1[0*KB];
    com1_send = (CPipeAsync*)&com1[4*KB];
    com2_recv = (CPipeAsync*)&com2[0*KB];
    com2_send = (CPipeAsync*)&com2[4*KB];
    
    CPool *pipepool = peb->PipePool;
    if (!pipepool) return false;

    char *pool = pipepool->Data;
    if (!pool) {
        Debug(" [FAIL] Network.Install Out of Memory!\n");
        return false;
    }

    for (int port = 1; port < 256; port++) {
        pipes[port] = (CPipeAsync*)&pool[port*4*KB];
        Pool->Insert(pipepool, pipes[port]);
        Pipe->CreateAsync(pipes[port], INetwork_GetB, INetwork_PutB);
    }

    Device->Hook('COM1', INetwork_GetB);
    Device->Hook('COM2', INetwork_GetB);
    initialized = true;
    return true;
}

bool INetwork_Hook(int port, void *callback) {
    if (port <= 0 || port > 255) return false;

    CPipeAsync *pipe = pipes[port];
    if (!pipe) return false;
    pipe->Reader = callback;
    return true;
}

bool INetwork_Unhook(int port, void *callback) {
    if (port <= 0 || port > 255) return false;

    CPipeAsync *pipe = pipes[port];
    if (!pipe) return false;
    pipe->Reader = nullptr;
    return true;
}

bool INetwork_Recv(int port, char *packet, int size) {
    if (port <= 0 || port > 255) return false;
    if (!packet || size <= 0) return false;

    static char buf[8];
    for (int i = 0; i < size; i++) {
        Pipe->ReadAsync(pipes[port], buf, 2);
        packet[i] = buf[1];
    }
    return true;
}

bool INetwork_Send(int port, char *packet, int size) {
    if (port <= 0 || port > 255) return false;
    if (!packet || size <= 0) return false;
    
    if (port == 255) { // loopback
        CPipeAsync *bind = pipes[port];
        Pipe->WriteAsync(bind, packet, size);
    } else {
        static u8 buf[8];
        buf[0] = port;
        CPipeAsync *bind = com1_send;
        for (int i = 0; i < size; i++) {
            buf[1] = packet[i];
            Pipe->WriteAsync(bind, buf, 2);
        }
    }
    return true;
}

bool INetwork_PrintF(int port, char *format, ...) {
    char text[0x400];

    va_list *list;
    va_start(list, format);
    VaFormat(text, format, list);
    va_end(list);

    for (char *c = text; *c; c++) {
        INetwork_Send(port, c, 1);
    }
    return true;
}

static INetwork inetwork = {
    INetwork_Install,
    INetwork_Unhook,
    INetwork_Hook,
    INetwork_Recv,
    INetwork_Send,
    INetwork_PrintF
};

export INetwork *Network = &inetwork;