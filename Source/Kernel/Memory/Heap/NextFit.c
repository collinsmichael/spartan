#include <oop.h>
#include <Kernel.h>

import IProcess *Process;

void *INextFit_Alloc(CPool *heap, int len) {
    if (!heap || len <= 0) return nullptr;
    len = ((len + heap->Gran-1) & ~(heap->Gran-1)) / heap->Gran;
    for (int buf = heap->Next; buf + len != heap->Next; buf = ++buf % heap->Count) {
        if (buf + len >= heap->Count) {
            buf = heap->Count - 1;
            continue;
        }
        for (int n = 0; heap->Node[buf + n] == 'o'; n++) {
            if (n < len-1) continue;
            heap->Next = (buf + len) % heap->Count;
            for (heap->Node[buf] = 'X'; n > 0; heap->Node[buf+n--] = 'x');
            return &heap->Data[buf*heap->Gran];
        }
    }
    return nullptr;
}

bool INextFit_Free(CPool *heap, void *buf) {
    if (!heap || !buf) return false;
    int p = (int)((u32)buf / heap->Gran);
    if (p >= heap->Count) return false;
    if (heap->Node[p] != 'X') return false;
    for (heap->Node[p++] = 'o'; p < heap->Count; p++) {
        if (heap->Node[p] == 'x') heap->Node[p] = 'o';
        else break;
    }
    while (heap->Next > 0 && heap->Node[heap->Next-1] == 'o') {
        heap->Next--;
    }
    return true;
}

void *IMemory_Ring0Alloc(int len) {
    CPool *Heap = (CPool*)MMAP_HEAPPOOL;
    return INextFit_Alloc(Heap, len);
}

bool IMemory_Ring0Free(void *buf) {
    CPool *Heap = (CPool*)MMAP_HEAPPOOL;
    return INextFit_Free(Heap, buf);
}

void *IMemory_Ring3Alloc(int len) {
    CPeb *peb = Process->GetPeb();
    if (!peb) return 0;
    return INextFit_Alloc(peb->HeapPool, len);
}

bool IMemory_Ring3Free(void *buf) {
    CPeb *peb = Process->GetPeb();
    if (!peb) return 0;
    return INextFit_Free(peb->HeapPool, buf);
}

IMemory Ring0Heap = {
    IMemory_Ring0Alloc,
    IMemory_Ring0Free
};

IMemory Ring3Heap = {
    IMemory_Ring3Alloc,
    IMemory_Ring3Free
};

export IMemory *SysMem = &Ring0Heap;
export IMemory *Memory = &Ring3Heap;