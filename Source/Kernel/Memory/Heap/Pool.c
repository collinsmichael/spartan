#define IMPL_POOL
#include <Shell.h>

bool IPool_Debug(CPool *pool) {
    if (!pool) return false;
    Debug("pool        = %X\n", pool);
    Debug("pool->Gran  = %X\n", pool->Gran);
    Debug("pool->Count = %X\n", pool->Count);
    Debug("pool->Next  = %X\n", pool->Next);
    Debug("pool->Node  = %X\n", pool->Node);
    Debug("pool->Data  = %X\n", pool->Data);
    Debug("\n");
    return true;
}

bool IPool_CreateMeta(CPool *pool, int count, int granularity, char *nodes, char *data, char *name) {
    if (!pool) return false;
    pool->Gran  = granularity;
    pool->Count = count;
    pool->Next = 0;
    pool->Node = nodes;
    pool->Data = data;
    if (pool->Node) stosd(pool->Node, 'oooo', (pool->Count+3)/4);
    if (name) movsb(pool->Name, name, 4);
    return true;
}

bool IPool_Create(CPool *pool, int size, int granularity) {
    if (!pool) return false;
    pool->Gran  = (granularity + 3) & ~(3);
    if (pool->Gran <= 0) return false;
    pool->Count = align((size - align(sizeof(CPool), 64))/pool->Gran, pool->Gran);
    pool->Next  = 0;

    char *data = (char*)pool;
    pool->Data  = &data[(pool->Count + 63) & ~(63)];
    pool->Node  = &data[(sizeof(CPool) + 63) & ~(63)];

    int delta = (int)pool->Data - (int)data;
    int overhead = (delta/pool->Gran + 63) & ~(63);
    stosd(pool->Node, 'oooo', (pool->Count+3)/4);
    stosb(pool->Node, 'x', overhead);
    IPool_Debug(pool);
    return true;
}

int IPool_Alloc(CPool *pool) {
    if (!pool) return 0;
    for (int i = pool->Next; (i+1) % pool->Count != pool->Next; i = ++i % pool->Count) {
        if (pool->Node[i] != 'x') {
            pool->Node[i] = 'x';
            pool->Next = (i+1) % pool->Gran;
            return i;
        }
    }
    return -1;
}

bool IPool_Release(CPool *pool, int index) {
    if (!pool) return false;
    if (index < 0 || index > pool->Count) return false;
    if (pool->Node[index] != 'x') return false;
    pool->Node[index] = 'o';
    for (int i = pool->Next; i >= 0; i--) {
        if (pool->Node[i] == 'x') break;
        pool->Next = i;
    }
    return true;
}

void *IPool_Insert(CPool *pool, void *item) {
    if (!pool) return 0;

    int index = IPool_Alloc(pool);
    if (index < 0) return 0;

    char *data = &pool->Data[index*pool->Gran];
    if (item) movsd(data, item, pool->Gran/4);
    return data;
}

bool IPool_Remove(CPool *pool, void *item) {
    if (!pool || !item) return false;
    int index = ((int)item - (int)pool->Data) / pool->Gran;
    return IPool_Release(pool, index);
}

static IPool pool = {
    IPool_CreateMeta,
    IPool_Create,
    IPool_Alloc,
    IPool_Release,
    IPool_Insert,
    IPool_Remove,
    IPool_Debug
};

export IPool *Pool = &pool;