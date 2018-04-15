#include <Library.h>
#include <Kernel.h>
#include <Shell.h>
#include <UnitTest.h>

import IMonitor *Monitor;
import IThread  *Thread;

extern char text[4*KB];
static CTask *TaskPool;
static char MockPartition[12*KB];
static u32  MockCr3;
static u32  MockEsp0;
static u32  MockEsp3;

static void StubProcess(void) {
    for (;;);
}

bool TestThread_SetUp(void) {
    TaskPool = (CTask*)MMAP_TASKPOOL;
    MockCr3  = (u32)Monitor->GetBase('PAGE');
    MockEsp3 = (u32)&MockPartition[4*KB];
    MockEsp0 = (u32)&MockPartition[8*KB];
    return true;
}

bool TestThread_TearDown(void) {
    TaskPool = nullptr;
    return true;
}

bool TestThread_WhenThreadsAreSpawnedAndKilled_ProcessIsDestroyed(int stress) {
    u32 actual_pid[4];
    u32 expect_pid[4];
    u32 actual_ctx[4];
    u32 expect_ctx[4];

    for (int i = 0; i < 4; i++) {
        CContext *ctx = Thread->Spawn(MockCr3, (u32)StubProcess, MockEsp0, MockEsp3);
        actual_pid[i] = ctx->pid;
        expect_pid[i] = i;
    }

    for (int i = 0; i < 4; i++) {
        Thread->Kill(actual_pid[i]);
        expect_ctx[i] = 0;
        actual_ctx[i] = TaskPool->CtxList[actual_pid[i]];
    }

    for (int i = 0; i < 4; i++) {
        expect_ctx[i] = 0;
        actual_ctx[i] = TaskPool->CtxList[actual_pid[i]];
        EXPECT(actual_pid[i] == expect_pid[i], " ProcessID is not what was expected!\n expected = %d actual = %d", expect_pid[i], actual_pid[i]);
        EXPECT(actual_ctx[i] == expect_ctx[i], " Process was not destroyed!\n expected = %d actual = %d", expect_ctx[i], actual_ctx[i]);
    }
    PASS;
}

bool TestThread_WhenThreadsSpawn_ProcessIdIsValid(int stress) {
    CContext *ctx0 = Thread->Spawn(MockCr3, (u32)StubProcess, MockEsp0, MockEsp3);
    u32 expect0 = 0;
    u32 actual0 = ctx0->pid;

    CContext *ctx1 = Thread->Spawn(MockCr3, (u32)StubProcess, MockEsp0, MockEsp3);
    u32 expect1 = 1;
    u32 actual1 = ctx1->pid;

    EXPECT(actual0 == expect0, " ProcessID is not what was expected!\n expected = %d actual = %d", expect0, actual0);
    EXPECT(actual1 == expect1, " ProcessID is not what was expected!\n expected = %d actual = %d", expect1, actual1);
    PASS;
}

bool TestThread_WhenThreadsSpawn_ContextIsReturned(int stress) {
    CContext *expect = (CContext*)MockEsp0;
    CContext *actual = Thread->Spawn(MockCr3, (u32)StubProcess, MockEsp0, MockEsp3);
    EXPECT(actual == expect, " Did not return expected context!\n expected = %d actual = %d", expect, actual);
    PASS;
}

bool TestThread_WhenThreadsAreKilled_ProcessIsDestroyed(int stress) {
    Thread->Kill(0);
    u32 expect0 = 0;
    u32 actual0 = TaskPool->CtxList[0];

    Thread->Kill(1);
    u32 expect1 = 0;
    u32 actual1 = TaskPool->CtxList[1];

    Thread->Kill(2);
    u32 expect2 = 0;
    u32 actual2 = TaskPool->CtxList[2];

    EXPECT(actual0 == expect0, " Process was not destroyed!\n expected = %d actual = %d", expect0, actual0);
    EXPECT(actual1 == expect1, " Process was not destroyed!\n expected = %d actual = %d", expect1, actual1);
    EXPECT(actual2 == expect2, " Process was not destroyed!\n expected = %d actual = %d", expect2, actual2);
    PASS;
}

bool TestThread_WhenThreadsSpawn_ThreadsAreBlocked(int stress) {
    int expect = TaskPool->BlockTail + 1;
    CContext *ctx = Thread->Spawn(MockCr3, (u32)StubProcess, MockEsp0, MockEsp3);
    int actual = TaskPool->BlockTail;
    if (ctx) Thread->Kill(ctx->pid);

    EXPECT((u32)ctx == MockEsp0, " Failed to create thread!\n ctx = %X", ctx);
    EXPECT(actual == expect, " Thread is not blocked!\n expected = %d actual = %d", expect, actual);
    PASS;
}

bool TestThread_WhenThreadsDie_ThreadsAreRemoved(int stress) {
    int expect = MAX(TaskPool->BlockTail + TaskPool->ReadyTail - 1, 0);
    CContext *ctx = Thread->Spawn(MockCr3, (u32)StubProcess, MockEsp0, MockEsp3);
    if (ctx) Thread->Kill(ctx->pid);
    int actual = TaskPool->BlockTail + TaskPool->ReadyTail;

    EXPECT((u32)ctx == MockEsp0, " Failed to create thread!\n ctx = %X", ctx);
    EXPECT(actual == expect, " Thread was not removed!\n expected = %d actual = %d", expect, actual);
    PASS;
}

bool TestThread_WhenThreadsSpawn_ContextIsCreated(int stress) {
    CContext *expect = (CContext*)MockEsp0;
    CContext *actual = Thread->Spawn(MockCr3, (u32)StubProcess, MockEsp0, MockEsp3);
    if (actual) Thread->Kill(actual->pid);
    EXPECT(actual == expect, " Failed to create context!\n expected = %X actual = %X", expect, actual);
    PASS;
}

bool TestThread_WhenThreadsRun_ReadyQueueIsAppended(int stress) {
    int expect = TaskPool->ReadyTail + 1;
    CContext *ctx = Thread->Spawn(MockCr3, (u32)StubProcess, MockEsp0, MockEsp3);
    if (ctx) Thread->Run(ctx->pid);
    int actual = TaskPool->ReadyTail;
    if (ctx) Thread->Kill(ctx->pid);

    EXPECT((u32)ctx == MockEsp0, " Failed to create thread!\n ctx = %X", ctx);
    EXPECT(actual == expect, " Failed to update ready queue!\n expected = %d actual = %d", expect, actual);
    PASS;
}

void *thread[] = {
    TestThread_SetUp, TestThread_TearDown,

    (void*)1, TestThread_WhenThreadsAreSpawnedAndKilled_ProcessIsDestroyed,
    (void*)1, TestThread_WhenThreadsSpawn_ProcessIdIsValid,
    (void*)1, TestThread_WhenThreadsSpawn_ContextIsReturned,
    (void*)1, TestThread_WhenThreadsAreKilled_ProcessIsDestroyed,

    (void*)1, TestThread_WhenThreadsSpawn_ThreadsAreBlocked,
    (void*)2, TestThread_WhenThreadsDie_ThreadsAreRemoved,
    (void*)2, TestThread_WhenThreadsSpawn_ContextIsCreated,
    (void*)2, TestThread_WhenThreadsRun_ReadyQueueIsAppended,

    0, 0 // end of tests
};