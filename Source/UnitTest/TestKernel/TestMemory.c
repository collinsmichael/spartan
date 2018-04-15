#include <Library.h>
#include <Kernel.h>
#include <Shell.h>
#include <UnitTest.h>

import IMemory *SysMem;
static IMemory *Memory;
static CPool *HeapPool;

bool TestMemory_SetUp(void) {
    Memory = SysMem;
    HeapPool = (CPool*)MMAP_HEAPPOOL;
    return true;
}

bool TestMemory_TearDown(void) {
    HeapPool = nullptr;
    Memory = nullptr;
    return true;
}

bool TestMemory_WhenAllocatingMemory_PartitionIsReturned(int stress) {
    char *actual = Memory->Alloc(4*KB);
    if (actual) Memory->Free(actual);

    EXPECT(actual != 0, " Failed to allocate partition!\n buf = %X", actual);
    PASS;
}

bool TestMemory_WhenFreeingMemory_PartitionIsReleased(int stress) {
    char *buf = Memory->Alloc(4*KB);

    bool expect = true;
    bool actual = Memory->Free(buf);

    EXPECT(buf != 0, " Failed to allocate partition!\n buf = %X", buf);
    EXPECT(expect == actual, " Failed to release partition!");
    PASS;
}

bool TestMemory_WhenPartitionIsReleased_PartitionIsReused(int stress) {
    char *expect = Memory->Alloc(4*KB);
    if (expect) Memory->Free(expect);

    char *actual = Memory->Alloc(4*KB);
    if (actual) Memory->Free(actual);

    EXPECT(actual != 0, " Failed to allocate partition!\n buf = %X", actual);
    EXPECT(expect == actual, " Failed to reuse partition!");
    PASS;
}

bool TestMemory_WhenFreeingMemory_PartitionsAreMerged(int stress) {
    char *expect = Memory->Alloc(4*KB);
    char *a = Memory->Alloc(4*KB);
    char *b = Memory->Alloc(4*KB);
    char *c = Memory->Alloc(4*KB);
    char *d = Memory->Alloc(4*KB);

    Memory->Free(expect);
    Memory->Free(d);
    Memory->Free(a);
    Memory->Free(b);
    Memory->Free(c);

    char *actual = Memory->Alloc(4*KB);
    Memory->Free(actual);

    EXPECT(expect == actual, " Failed to merge partitions!\n"
        " expected = %X actual = %X", expect, actual);
    PASS;
}

bool TestMemory_WhenSizeIsNull_AllocReturnsNull(int stress) {
    char *expect = 0;
    char *actual = Memory->Alloc(0);
    EXPECT(expect == actual, " Zero size ought to have failed!\n"
        " expected = %X actual = %X", expect, actual);
    PASS;
}

bool TestMemory_WhenSizeIsNegative_AllocReturnsNull(int stress) {
    char *expect = 0;
    char *actual = Memory->Alloc(-1);
    EXPECT(expect == actual, " Negative size ought to have failed!\n"
        " expected = %X actual = %X", expect, actual);
    PASS;
}

bool TestMemory_WhenBufferIsNull_FreeReturnsFalse(int stress) {
    bool expect = 0;
    bool actual = Memory->Free(0);
    EXPECT(expect == actual, " Null pointer ought to have failed!\n"
        " expected = %X actual = %X", expect, actual);
    PASS;
}

void *memory[] = {
    TestMemory_SetUp,
    TestMemory_TearDown,

    (void*)1, TestMemory_WhenAllocatingMemory_PartitionIsReturned,
    (void*)1, TestMemory_WhenFreeingMemory_PartitionIsReleased,
    (void*)1, TestMemory_WhenPartitionIsReleased_PartitionIsReused,
    (void*)1, TestMemory_WhenFreeingMemory_PartitionsAreMerged,
    (void*)1, TestMemory_WhenSizeIsNull_AllocReturnsNull,
    (void*)1, TestMemory_WhenSizeIsNegative_AllocReturnsNull,
    (void*)1, TestMemory_WhenBufferIsNull_FreeReturnsFalse,
    0, 0 // end of tests
};