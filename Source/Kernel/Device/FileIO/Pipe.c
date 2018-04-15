#include <Kernel.h>

extern IThread *Thread;

export bool IPipe_Debug(CPipe *pipe) {
    Debug(" pipe           = %X\n", pipe          );
    Debug(" pipe->Pointer  = %X\n", pipe->Pointer );
    Debug(" pipe->Lock     = %X\n", pipe->Lock    );
    Debug(" pipe->Capacity = %X\n", pipe->Capacity);
    Debug(" pipe->Consumed = %X\n", pipe->Consumed);
    Debug(" pipe->Reader   = %X\n", pipe->Reader  );
    Debug(" pipe->Writer   = %X\n", pipe->Writer  );
    Debug(" pipe->Head     = %X\n", pipe->Head    );
    Debug(" pipe->Tail     = %X\n", pipe->Tail    );
    return true;
}

bool IPipe_Create(CPipe *pipe, u32 Reader, u32 Writer) {
    if (!pipe) return false;
    u8 *base = (u8*)pipe;
    pipe->Pointer = &base[sizeof(CPipe)];
    pipe->Lock = 0;
    pipe->Capacity = 4*KB - sizeof(CPipe);
    pipe->Consumed = 0;
    pipe->Reader = Reader;
    pipe->Writer = Writer;
    pipe->Head = 0;
    pipe->Tail = 0;
    return true;
}

bool IPipe_CreateAsync(CPipeAsync *pipe, void *Reader, void *Writer) {
    if (!pipe) return false;
    u8 *base = (u8*)pipe;
    pipe->Pointer = &base[sizeof(CPipe)];
    pipe->Lock = 0;
    pipe->Capacity = 4*KB - sizeof(CPipe);
    pipe->Consumed = 0;
    pipe->Reader = Reader;
    pipe->Writer = Writer;
    pipe->Head = 0;
    pipe->Tail = 0;
    return true;
}

bool IPipe_Destroy(CPipe *pipe) {
    if (!pipe) return false;
    if (!pipe->Pointer) return false;
    stosd(pipe, 0, 4*KB/4);
    return true;
}

bool IPipe_Read(CPipe *pipe, u8 *buf, u32 len) {
    if (!pipe || !buf || len == 0) return false;
    if (!pipe->Pointer) return false;
    while (len > 0) {
        while (pipe->Consumed == 0) Thread->Yield();
        do {
            while (pipe->Lock) Thread->Yield();
            pipe->Lock = pipe->Reader;
        } while (pipe->Lock != pipe->Reader);

        pipe->Consumed--;
        *buf++ = pipe->Pointer[pipe->Head % pipe->Capacity];
        pipe->Head++;
        len--;
        pipe->Lock = 0;
    }
    return true;
}

bool IPipe_Write(CPipe *pipe, u8 *buf, u32 len) {
    if (!pipe || !buf || len == 0) return false;
    if (!pipe->Pointer) return false;
    while (len > 0) {
        while (pipe->Consumed == pipe->Capacity) Thread->Yield();
        do {
            while (pipe->Lock) Thread->Yield();
            pipe->Lock = pipe->Writer;
        } while (pipe->Lock != pipe->Writer);

        pipe->Consumed++;
        pipe->Pointer[pipe->Tail % pipe->Capacity] = *buf++;
        pipe->Tail++;
        len--;
        pipe->Lock = 0;
    }
    return true;
}

bool IPipe_ReadNonBlocking(CPipe *pipe, u8 *buf, u32 len) {
    if (!pipe || !buf || len == 0) return false;
    if (!pipe->Pointer) return false;
    while (len > 0) {
        if (pipe->Consumed == 0) pipe->Tail++;
        else pipe->Consumed--;

        *buf++ = pipe->Pointer[pipe->Head % pipe->Capacity];
        pipe->Head++;
        len--;
    }
    return true;
}

bool IPipe_WriteNonBlocking(CPipe *pipe, u8 *buf, u32 len) {
    if (!pipe || !buf || len == 0) return false;
    if (!pipe->Pointer) return false;
    while (len > 0) {
        if (pipe->Consumed == pipe->Capacity) pipe->Head++;
        else pipe->Consumed++;

        pipe->Pointer[pipe->Tail % pipe->Capacity] = *buf++;
        pipe->Tail++;
        len--;
    }
    return true;
}

bool IPipe_ReadAsync(CPipeAsync *pipe, u8 *buf, u32 len) {
    if (!pipe || !buf || len == 0) return false;
    if (!pipe->Pointer) return false;
    while (len > 0) {
        if (pipe->Consumed == 0) {
            if (pipe->Writer) pipe->Writer((CPipe*)pipe);
            else pipe->Tail++;
        } else pipe->Consumed--;

        *buf++ = pipe->Pointer[pipe->Head % pipe->Capacity];
        pipe->Head++;
        len--;
    }
    if (pipe->Writer) pipe->Writer((CPipe*)pipe);
    return true;
}

bool IPipe_WriteAsync(CPipeAsync *pipe, u8 *buf, u32 len) {
    if (!pipe || !buf || len == 0) return false;
    if (!pipe->Pointer) return false;
    while (len > 0) {
        if (pipe->Consumed == pipe->Capacity) {
            if (pipe->Reader) pipe->Reader((CPipe*)pipe);
            else pipe->Head++;
        } else pipe->Consumed++;

        pipe->Pointer[pipe->Tail % pipe->Capacity] = *buf++;
        pipe->Tail++;
        len--;
    }
    if (pipe->Reader) pipe->Reader((CPipe*)pipe);
    return true;
}

IPipe pipe = {
    IPipe_Create,
    IPipe_CreateAsync,
    IPipe_Destroy,
    IPipe_Read,
    IPipe_Write,
    IPipe_ReadNonBlocking,
    IPipe_WriteNonBlocking,
    IPipe_ReadAsync,
    IPipe_WriteAsync
};

export IPipe *Pipe = &pipe;