#include <Shell.h>
#include <Kernel.h>
#include <Library.h>

extern IMonitor *Monitor;
extern IMemory  *Memory;
extern IPaging  *Paging;
extern IThread  *Thread;
extern IRamDisk *RamDisk;
extern IFileIO  *FileIO;
extern IVesa    *Vesa;
extern IPool    *Pool;
extern IPipe    *Pipe;
extern ILoader  *Loader;
extern bool FromRing0;

extern IMemory Ring0Heap;

CPcb *IEnvironment_SetUp(CPcb *pcb, char *base, char *path, char *args) {
    (int)pcb->Binary  += (int)base;
    (int)pcb->AppMain += (int)base;
    (int)pcb->Heap    += (int)base;
    (int)pcb->Stack   += (int)base;
    (int)pcb->Peb     += (int)base;
    (int)pcb->MemMap  += (int)base;
    (int)pcb->Pipes   += (int)base;
    (int)pcb->Events  += (int)base;
    (int)pcb->Windows += (int)base;
    (int)pcb->Screen  += (int)base;
    char *seg = (char*)pcb->Peb;
    if (!seg) return 0;

    CPeb *peb = (CPeb*)&seg[0x00000000];
    CPcb *raw = (CPcb*)&seg[0x00000100];
    movsd(raw, pcb, sizeof(CPcb)/4);

    peb->Canvas     = (CCanvas*)&seg[0x00000200];
    peb->HeapPool   = (CPool  *)&seg[0x00000300 + 0*align(sizeof(CPool), 64)];
    peb->PipePool   = (CPool  *)&seg[0x00000300 + 1*align(sizeof(CPool), 64)];
    peb->EventPool  = (CPool  *)&seg[0x00000300 + 2*align(sizeof(CPool), 64)];
    peb->WindowPool = (CPool  *)&seg[0x00000300 + 3*align(sizeof(CPool), 64)];
    peb->Pages      = (int    *)&seg[0x00001000];
    peb->Stack0     = (int    *)&seg[0x0007F000];

    char *pipes   = &seg[0x0007F800];
    char *events  = &seg[0x00080000];
    char *windows = &seg[0x000C0000];

    CCanvas *canvas  = peb->Canvas;
    canvas->ResX     = Vesa->ResX();
    canvas->ResY     = Vesa->ResY();
    canvas->ScanLine = Vesa->Bits()*peb->Canvas->ResX/8;
    canvas->Disp     = (char*)pcb->Screen;
    canvas->Draw     = &canvas->Disp[canvas->ResY*canvas->ScanLine];
    canvas->Peb      = peb;
    canvas->Pcb      = raw;
    stosd(canvas->Disp, 0, 8*MB/4);

    int pipesize  = 4*KB;
    int pipecount = 1*MB/pipesize;
    int evtsize  = align(sizeof(CEvent), 64);
    int evtcount = 1*MB/evtsize;
    int winsize  = align(sizeof(CWindow), 64);
    int wincount = 4*MB/winsize;

    Pool->CreateMeta(peb->HeapPool, 1*MB, 4*KB, 0, 0, "HEAP");
    Pool->CreateMeta(peb->PipePool, pipecount, 4*KB, pipes, (char*)pcb->Pipes, "PIPE");
    Pool->CreateMeta(peb->EventPool, evtcount, evtsize, events, (char*)pcb->Events, "EVTS");
    Pool->CreateMeta(peb->WindowPool, wincount, winsize, windows, (char*)pcb->Windows, "WINS");

    peb->HeapPool->Data = (char*)pcb->Heap;
    peb->HeapPool->Node = (char*)pcb->MemMap;
    peb->HeapPool->Next = 0;

    int count = (pcb->Stack-pcb->Heap) / (4*KB);
    stosd(&peb->HeapPool->Node[0], '----', 1*MB/4);
    stosd(&peb->HeapPool->Node[0], 'oooo', (count+3)/4);

    char *pipe = peb->PipePool->Data;
    int cmd = Pool->Alloc(peb->PipePool);
    peb->CommandLine = &pipe[cmd*4*KB];
    stosd(peb->CommandLine, 0, 4*KB/4);
    Format(peb->CommandLine, "%s %s", path, args);
    Debug(" Peb.CommandLine = (%X) %s\n", peb->CommandLine, peb->CommandLine);
    return raw;
}