#include <Kernel.h>

extern IMonitor *Monitor;
extern IMemory  *SysMem;
extern ILoader  *Loader;
extern IPool    *Pool;

bool IThread_Block(int pid);
u32 IThread_Yield(void);
int RoundRobin(int err, int esp);

extern bool fpu;
extern bool mmx;
extern bool sse;

static ICanvas *Canvas;
static CCanvas *screen;
static CWindow *cursor;

bool IThread_Debug(void);

int IThread_GetPid(void) {
    CTask *TaskPool = (CTask*)MMAP_TASKPOOL;
    return TaskPool->ReadyQ[TaskPool->ReadyHead];
}

CContext *IThread_Spawn(u32 cr3, u32 eip, u32 esp0, u32 esp3) {
    CTask *TaskPool = (CTask*)MMAP_TASKPOOL;
    int pid = Pool->Alloc((CPool*)TaskPool);
    if (pid < 0 || pid >= TaskPool->Count) return 0;

    CContext *ctx = (CContext*)(esp0 - sizeof(CContext));
    TaskPool->CtxList[pid] = (u32)&ctx->cr3;

    int shell = (int)Loader->GetEntry("System/Shell.dll");
    stosd(ctx, 0, sizeof(CContext)/4);
    ctx->cr3    = cr3;
    ctx->cs     = RING3_CODE;
    ctx->ds     = RING3_DATA;
    ctx->ss     = RING3_DATA;
    ctx->eflags = EFLAGS_IOPL + EFLAGS_IF;
    ctx->esp0   = esp3;
    ctx->esp    = (int)&ctx->eip;
    ctx->eip    = (shell) ? shell : eip;
    ctx->pid    = pid;

    if (fpu) {
        extern char x87[128];
        u32 *ctx_fpu = (u32*)(esp0+0x100);
        movsd(ctx_fpu, x87, sizeof(x87)/4);
    }

    if (sse) {
        extern char simd[320];
        u32 *ctx_fpu = (u32*)(esp0+0x200);
        movsd(ctx_fpu, simd, sizeof(simd)/4);
    }

    TaskPool->BlockQ[TaskPool->BlockTail++] = pid;
    return ctx;
}

u32 DoKill(u32 err, u32 esp) {
    CTask *TaskPool = (CTask*)MMAP_TASKPOOL;
    while (TaskPool->KillHead != TaskPool->KillTail) {
        int pid = TaskPool->KillQ[TaskPool->KillHead];
        TaskPool->KillQ[TaskPool->KillHead] = 0;
        TaskPool->KillHead = ++TaskPool->KillHead % (4*KB);
        if (pid <= 0) continue;

        IThread_Debug();

        Logger("DoKill (Blocking Pid %d)\n", pid);
        IThread_Block(pid);
        IThread_Debug();

        Logger("Flushing Blocked Q (%d)\n", pid);
        for (int i = 0; i < TaskPool->KillTail; i++) {
            if (TaskPool->BlockQ[i] == pid) {
                int tail = TaskPool->BlockQ[--TaskPool->BlockTail];
                TaskPool->BlockQ[i] = tail;
                TaskPool->BlockQ[TaskPool->BlockTail] = 0;
                TaskPool->BlockHead = 0;
            }
        }
        IThread_Debug();

        for (int i = 0; i < 32; i++) Logger("%c", TaskPool->Node[i]); Logger("\n");
        Pool->Release((CPool*)TaskPool, pid);
        for (int i = 0; i < 32; i++) Logger("%c", TaskPool->Node[i]); Logger("\n");

        CPcb *pcb = (CPcb*)TaskPool->PcbList[pid];
        CPeb *peb = (CPeb*)TaskPool->PebList[pid];
        u32  *esp = (u32 *)TaskPool->CtxList[pid];
    
        Logger(" [info] Process %d pcb=%X peb=%X\n", pid, pcb, peb);
        if (peb) {
            CCanvas *canvas = peb->Canvas;
            Logger(" [info] Process %d canvas=%X\n", pid, canvas);
            if (canvas) {
                CWindow *frame = canvas->TopMost;
                Logger(" [info] Process %d frame=%X\n", pid, frame);
                if (frame) {
                    CWindow *prev = frame->Prev;
                    CWindow *next = frame->Next;
                    Logger(" [info] Process %d prev=%X next=%X\n", pid, prev, next);
                    if (prev) prev->Next = next;
                    if (next) next->Prev = prev;
                }
            }
        }

        if (screen) {
            for (CWindow *list = screen->TopMost; list; list = list->Next) {
                list->Dirty = 2;
            }
        }

        SysMem->Free((void*)pcb->Binary);
        TaskPool->PcbList[pid] = 0;
        TaskPool->PebList[pid] = 0;
        TaskPool->CtxList[pid] = 0;
        Logger(" [info] Process %d terminated\n", pid);
    }
    return esp;
}

export bool IThread_Killer(void *a, void *b, void *c) {
    if (Canvas == nullptr) Canvas = a;
    if (screen == nullptr) screen = b;
    if (cursor == nullptr) cursor = c;
    if (Canvas != a) return false;
    if (screen != b) return false;
    if (cursor != c) return false;

    CTask *TaskPool = (CTask*)MMAP_TASKPOOL;
    if (TaskPool->KillHead != TaskPool->KillTail) {
        SysCall(SYS_KILL,0,0,0,0,0,0);
    }
    return true;
}

u32 IThread_Exit(int code) {
    CTask *TaskPool = (CTask*)MMAP_TASKPOOL;
    IThread_Yield();
    Logger("IThread_Exit\n");
    IThread_Debug();
    TaskPool->KillQ[TaskPool->KillTail] = IThread_GetPid();
    TaskPool->KillTail = ++TaskPool->KillTail % (4*KB);
    for (;;);
    return 0;
}

bool IThread_Kill(int pid) {
    CTask *TaskPool = (CTask*)MMAP_TASKPOOL;
    if (pid == IThread_GetPid()) IThread_Exit(0);
    IThread_Debug();
    IThread_Yield();
    Logger("IThread_Kill\n");
    TaskPool->KillQ[TaskPool->KillTail] = pid;
    TaskPool->KillTail = ++TaskPool->KillTail % (4*KB);
    return true;
}

bool IThread_Block(int pid) {
    CTask *TaskPool = (CTask*)MMAP_TASKPOOL;
    if (pid < 0 || pid >= TaskPool->Count) return false;
    for (int i = 0; i < TaskPool->ReadyTail; i++) {
        if (TaskPool->ReadyQ[i] == pid) {
            TaskPool->ReadyTail--;
            TaskPool->ReadyTail = MAX(0, TaskPool->ReadyTail);
            TaskPool->ReadyQ[i] = TaskPool->ReadyQ[TaskPool->ReadyTail];
            TaskPool->ReadyQ[TaskPool->ReadyTail] = 0;
            TaskPool->BlockQ[TaskPool->BlockTail++] = pid;
            return true;
        }
    }
    return false;
}

bool IThread_Unblock(int pid) {
    CTask *TaskPool = (CTask*)MMAP_TASKPOOL;
    if (pid < 0 || pid >= TaskPool->Count) return false;
    for (int i = 0; i < TaskPool->Count; i++) {
        if (TaskPool->BlockQ[i] == pid) {
            TaskPool->BlockTail--;
            TaskPool->BlockTail = MAX(0, TaskPool->BlockTail);
            TaskPool->BlockQ[i] = TaskPool->BlockQ[TaskPool->BlockTail];
            TaskPool->BlockQ[TaskPool->BlockTail] = 0;
            TaskPool->ReadyQ[TaskPool->ReadyTail++] = pid;
            return true;
        }
    }
    return false;
}

bool IThread_Run(int pid) {
    return IThread_Unblock(pid);
}

CContext *IThread_Fork(u32 cr3, u32 eip, u32 esp0, u32 esp3) {
    CContext *context = IThread_Spawn(cr3, eip, esp0, esp3);
    if (!context) return false;
    if (!IThread_Run(context->pid)) return 0;
    return context;
}

u32 IThread_Yield(void) {
    SysCall(SYS_YIELD,0,0,0,0,0,0);
    return true;
}

u32 DoYield(u32 err, u32 esp) {
    return RoundRobin(err, esp);
}

bool IThread_Debug(void) {
    CTask *TaskPool = (CTask*)MMAP_TASKPOOL;
    Logger("\n");
    int  RH = TaskPool->ReadyHead;
    int  RT = TaskPool->ReadyTail;
    int *RQ = TaskPool->ReadyQ;
    Logger(" RDYQ"); for (int i = 0; i <= RT+2; i++) Logger((i == RH) ? " H" : " -"); Logger("\n");
    Logger(" RDYQ"); for (int i = 0; i <= RT+2; i++) Logger(" %d", RQ[i]);            Logger("\n");
    Logger(" RDYQ"); for (int i = 0; i <= RT+2; i++) Logger((i == RT) ? " T" : " -"); Logger("\n");

    int  BH = TaskPool->BlockHead;
    int  BT = TaskPool->BlockTail;
    int *BQ = TaskPool->BlockQ;
    Logger(" BLKQ"); for (int i = 0; i <= BT+2; i++) Logger((i == BH) ? " H" : " -"); Logger("\n");
    Logger(" BLKQ"); for (int i = 0; i <= BT+2; i++) Logger(" %d", BQ[i]);            Logger("\n");
    Logger(" BLKQ"); for (int i = 0; i <= BT+2; i++) Logger((i == BT) ? " T" : " -"); Logger("\n");

    int  KH = TaskPool->KillHead;
    int  KT = TaskPool->KillTail;
    int *KQ = TaskPool->KillQ;
    Logger(" KILL"); for (int i = 0; i <= KT+2; i++) Logger((i == KH) ? " H" : " -"); Logger("\n");
    Logger(" KILL"); for (int i = 0; i <= KT+2; i++) Logger(" %d", KQ[i]);            Logger("\n");
    Logger(" KILL"); for (int i = 0; i <= KT+2; i++) Logger((i == KT) ? " T" : " -"); Logger("\n");
    return true;
}

char *IThread_Status(char *text) {
    CTask *TaskPool = (CTask*)MMAP_TASKPOOL;
    Format(text,
    " Thread->Name       = %s\n" " Thread->Tss        = %X\n"
    " Thread->Node       = %X\n" " Thread->Data       = %X\n"
    " Thread->Gran       = %X\n"

    " Thread->Count      = %X\n" " Thread->Next       = %X\n"
    " Thread->ReadyQ     = %X\n" " Thread->BlockQ     = %X\n"

    " Thread->CtxList    = %X\n"
    " Thread->PcbList    = %X\n" " Thread->PebList    = %X\n"
    " Thread->ReadyHead  = %X\n" " Thread->ReadyTail  = %X\n"
    " Thread->BlockHead  = %X\n" " Thread->BlockTail  = %X\n"
    " Thread->ReadyQ[%d] = %X\n" " Thread->BlockQ[%d] = %X\n",
    TaskPool->Name,      TaskPool->Tss,
    TaskPool->Node,      TaskPool->Data,
    TaskPool->Gran,
    TaskPool->Count,     TaskPool->Next,
    TaskPool->CtxList,
    TaskPool->PcbList,   TaskPool->PebList,
    TaskPool->ReadyQ,    TaskPool->BlockQ,
    TaskPool->ReadyHead, TaskPool->ReadyTail,
    TaskPool->BlockHead, TaskPool->BlockTail,
    TaskPool->ReadyHead, TaskPool->ReadyQ[TaskPool->ReadyHead],
    TaskPool->BlockHead, TaskPool->BlockQ[TaskPool->BlockHead]);
    return text;
}

IThread thread = {
    IThread_Debug,
    IThread_Status,
    IThread_GetPid,
    IThread_Fork,
    IThread_Spawn,
    IThread_Run,
    IThread_Yield,
    IThread_Exit,
    IThread_Kill,
    IThread_Block,
    IThread_Unblock
};

export IThread *Thread = &thread;