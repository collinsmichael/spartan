#ifndef KERNEL_DEVICE_FILEIO_PIPE_H
#define KERNEL_DEVICE_FILEIO_PIPE_H
#include <oop.h>
#pragma pack(push, 1)

typedef struct CPipe {
    u8 *Pointer;
    u32 Lock;
    u32 Capacity;
    u32 Consumed;
    u32 Reader;
    u32 Writer;
    u32 Head;
    u32 Tail;
} CPipe;

typedef struct CPipeAsync {
    u8 *Pointer;
    u32 Lock;
    u32 Capacity;
    u32 Consumed;
    bool (*Reader)(CPipe *pipe);
    bool (*Writer)(CPipe *pipe);
    u32 Head;
    u32 Tail;
} CPipeAsync;
#define PIPE_BLOCKING 0x00
#define PIPE_NONBLOCK 0x10
#define PIPE_ASYNC    0x20
#define PIPE_R        0x02
#define PIPE_W        0x04
#define PIPE_RW       0x06

typedef struct IPipe {
    bool (*Create)(CPipe *pipe, u32 Reader, u32 Writer);
    bool (*CreateAsync)(CPipeAsync *pipe, void *Reader, void *Writer);
    bool (*Destroy)(CPipe *pipe);

    bool (*Read)(CPipe *pipe, u8 *buf, u32 len);
    bool (*Write)(CPipe *pipe, u8 *buf, u32 len);
    bool (*ReadNonBlocking)(CPipe *pipe, u8 *buf, u32 len);
    bool (*WriteNonBlocking)(CPipe *pipe, u8 *buf, u32 len);
    bool (*ReadAsync)(CPipeAsync *pipe, u8 *buf, u32 len);
    bool (*WriteAsync)(CPipeAsync *pipe, u8 *buf, u32 len);
} IPipe;

typedef struct IPipeAsync {
    bool (*Create)(CPipe *pipe, u32 Reader, u32 Writer);
    bool (*Destroy)(CPipe *pipe);
    bool (*Read)(CPipe *pipe, u8 *buf, u32 len);
    bool (*Write)(CPipe *pipe, u8 *buf, u32 len);
} IPipeAsync;

typedef struct IPipeNonBlocking {
    bool (*Create)(CPipe *pipe, u32 Reader, u32 Writer);
    bool (*Destroy)(CPipe *pipe);
    bool (*Read)(CPipe *pipe, u8 *buf, u32 len);
    bool (*Write)(CPipe *pipe, u8 *buf, u32 len);
} IPipeNonBlocking;

#pragma pack(pop)
#endif KERNEL_DEVICE_FILEIO_PIPE_H