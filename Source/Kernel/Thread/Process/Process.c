#include <Shell.h>
#include <Kernel.h>
#include <Library.h>

extern IMonitor *Monitor;
extern IMemory  *SysMem;
extern IPaging  *Paging;
extern IThread  *Thread;
extern IRamDisk *RamDisk;
extern IFileIO  *FileIO;
extern IVesa    *Vesa;
extern IPool    *Pool;
extern ILoader  *Loader;
bool FromRing0;


static CCanvas *screen;
static ICanvas *Canvas;
static CWindow *cursor;

bool IProcess_Inject(ICanvas *canvas, CCanvas *scr, CWindow *curs) {
    if (screen == nullptr) screen = scr;
    if (Canvas == nullptr) Canvas = canvas;
    if (cursor == nullptr) cursor = curs;
    return true;
}

CPcb *IEnvironment_SetUp(CPcb *mockup, char *base, char *path, char *args);

void IProcess_DebugPcb(CPcb *pcb) {
    Debug(" pcb            = %X\n", pcb);
    Debug(" pcb.ProcessID  = %X\n", pcb->ProcessID);
    Debug(" pcb.AppMain    = %X\n", pcb->AppMain  );
    Debug(" pcb.Binary     = %X\n", pcb->Binary   );
    Debug(" pcb.Heap       = %X\n", pcb->Heap     );
    Debug(" pcb.Stack      = %X\n", pcb->Stack    );
    Debug(" pcb.Peb        = %X\n", pcb->Peb      );
    Debug(" pcb.MemMap     = %X\n", pcb->MemMap   );
    Debug(" pcb.Pipes      = %X\n", pcb->Pipes    );
    Debug(" pcb.Events     = %X\n", pcb->Events   );
    Debug(" pcb.Windows    = %X\n", pcb->Windows  );
    Debug(" pcb.Screen     = %X\n", pcb->Screen   );
    Debug(" pcb.Extent     = %X\n", pcb->Extent   );
}

void IProcess_DebugPeb(CPeb *peb) {
    Logger(" peb            = %X\n", peb);
    Logger(" pcb.HeapPool   = %X\n", peb->HeapPool  );
    Logger(" pcb.PipePool   = %X\n", peb->PipePool  );
    Logger(" pcb.EventPool  = %X\n", peb->EventPool );
    Logger(" pcb.WindowPool = %X\n", peb->WindowPool);
    Logger(" pcb.Canvas     = %X\n", peb->Canvas    );
    Logger(" pcb.Context    = %X\n", peb->Context   );
    Logger(" pcb.Pages      = %X\n", peb->Pages     );
    Logger(" pcb.Stack0     = %X\n", peb->Stack0    );
}

CPcb *IProcess_GetPcb(void) {
    int pid = Thread->GetPid();
    if (pid < 0) return 0;
    CPcb **pcb = (CPcb**)Monitor->GetBase('PCB ');
    return pcb[pid];
}

CPeb *IProcess_GetPeb(void) {
    int pid = Thread->GetPid();
    if (pid < 0) return 0;
    CPeb **peb = (CPeb**)Monitor->GetBase('PEB ');
    return peb[pid];
}

CCanvas *IProcess_GetCanvas(void) {
    CPeb *peb = IProcess_GetPeb();
    if (!peb) return 0;
    return peb->Canvas;
}

CPcb *IProcess_Load(char *path) {
    extern IMemory Ring0Heap;
    CTask *TaskPool = (CTask*)MMAP_TASKPOOL;

    char *args = path;
    while (*args > ' ') args++;
    *args++ = 0;

    CFile *file = FileIO->Open(path, "-rwx");
    if (!file) return 0;
    char *image = FileIO->Base(file);
    int size = FileIO->Size(file);
    FileIO->Close(file);

    MZHDR *mz = (MZHDR*)image;
    OPHDR *op = (OPHDR*)&image[mz->e_lfanew + sizeof(PEHDR)];
    int ImageSize = MAX(1*MB, op->ImageSize);
    int HeapSize  = MAX(1*MB, MAX(op->SizeHeapReserve,  op->SizeHeapCommit));
    int StackSize = MAX(1*MB, MAX(op->SizeStackReserve, op->SizeStackCommit));
    ImageSize = align(ImageSize, 1*MB);
    HeapSize  = align(HeapSize,  1*MB);
    StackSize = align(StackSize, 1*MB);
    int FootPrint = ImageSize + HeapSize + StackSize;
    if (!FromRing0) FootPrint = align(FootPrint, 4*MB);
    Debug(" ImageSize = %X\n", ImageSize);
    Debug(" HeapSize  = %X\n", HeapSize);
    Debug(" StackSize = %X\n", StackSize);
    Debug(" FootPrint = %X\n", FootPrint);

    CPcb mockup;
    stosd(&mockup, 0, sizeof(CPcb)/4);
    mockup.AppMain = op->Entry;
    mockup.Binary  = 0x000000;
    mockup.Heap    = ImageSize;
    mockup.Stack   = FootPrint - StackSize;

    static int memmap[] = {
        0x00400000, // Peb
        0x00500000, // MemMap
        0x00600000, // Pipes
        0x00700000, // Events
        0x00800000, // Windows
        0x00C00000, // Screen
        0x01400000  // Extent
    };
    mockup.Peb     = (CPeb*)align(FootPrint + memmap[0] - memmap[0], 1*MB);
    mockup.MemMap  = align(FootPrint + memmap[1] - memmap[0], 1*MB);
    mockup.Pipes   = align(FootPrint + memmap[2] - memmap[0], 1*MB);
    mockup.Events  = align(FootPrint + memmap[3] - memmap[0], 1*MB);
    mockup.Windows = align(FootPrint + memmap[4] - memmap[0], 1*MB);
    mockup.Screen  = align(FootPrint + memmap[5] - memmap[0], 1*MB);
    mockup.Extent  = align(FootPrint + memmap[6] - memmap[0], 4*MB);

    char *base = (FromRing0) ? (char*)(0x0D*MB) : SysMem->Alloc(mockup.Extent);
    if (!base) return 0;
    stosd(base, 0, (mockup.Extent+3)/4);
    movsd(base, image, (size+3)/4);

    Loader->Reloc(path, base, size);
    Loader->Link(path, base, size);
    CPcb *pcb = IEnvironment_SetUp(&mockup, base, path, args);
    pcb->AppMain = (u32)&base[op->Entry];
    return pcb;
}

CContext *IProcess_Enqueue(CPcb *pcb) {
    u32 esp3 = (u32)pcb->Stack + 1*MB;
    u32 esp0 = (u32)pcb->Peb->Stack0;
    u32 cr3  = (u32)pcb->Peb->Pages;
    u32 eip  = (u32)pcb->AppMain;

    Paging->AppPages(pcb);
    CContext *ctx = Thread->Spawn(cr3, eip, esp0, esp3);
    if (!ctx) return 0;

    CPeb *peb = pcb->Peb;
    peb->Context = ctx;
    pcb->ProcessID = ctx->pid;

    CTask *TaskPool = (CTask*)MMAP_TASKPOOL;
    TaskPool->PcbList[ctx->pid] = (int)pcb;
    TaskPool->PebList[ctx->pid] = (int)peb;
    return ctx;
}

CPcb *IProcess_Spawn(char *path) {
    extern IMemory Ring0Heap;

    CPcb *pcb = IProcess_Load(path);
    if (pcb) {
        CContext *ctx = IProcess_Enqueue(pcb);
        if (ctx) {
            Debug(" Process Created\n");
            IProcess_DebugPcb(pcb);
            IProcess_DebugPeb(pcb->Peb);
            return pcb;
        }
        else Ring0Heap.Free(pcb);
    }
    return 0;
}

bool IProcess_Run(CPcb *pcb) {
    if (!pcb) return false;
    Thread->Run(pcb->ProcessID);
    return true;
}

CPcb *IProcess_Fork(char *path) {
    CPcb *pcb = IProcess_Spawn(path);
    if (!pcb) return false;
    IProcess_Run(pcb);
    return pcb;
}

u32 DoFork(u32 err, u32 esp, char *path) {
    CContext *ctx = (CContext*)esp;
    ctx->eax = (u32)IProcess_Fork(path);
    return esp;
}

CPcb *IProcess_Create(char *path) {
    return (CPcb*)SysCall(SYS_FORK,(int)path,0,0,0,0,0);
}

void IProcess_Exit(int code) {
    Thread->Exit(0);
}

IProcess process = {
    IProcess_Create,
    IProcess_Fork,
    IProcess_Spawn,
    IProcess_Run,
    IProcess_Exit,
    IProcess_GetPcb,
    IProcess_GetPeb,
    IProcess_GetCanvas,
    IProcess_Inject,
    IProcess_DebugPcb,
    IProcess_DebugPeb
};

export IProcess *Process = &process;