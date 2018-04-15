#ifndef KERNEL_THREAD_PROCESS_H
#define KERNEL_THREAD_PROCESS_H
#include <oop.h>
#include <Kernel\Memory\Heap\Pool.h>
#include <Kernel\Thread\Process\Thread.h>
#include <Shell\RunTime\Graphics\Canvas.h>
#include <Shell\Window\Events\Events.h>
#pragma pack(push, 1)

typedef struct CPeb {
    CPool    *HeapPool;
    CPool    *PipePool;
    CPool    *EventPool;
    CPool    *WindowPool;
    CCanvas  *Canvas;
    CContext *Context;
    int      *Pages;
    int      *Stack0;
    int       ClockFront;
    int       ClockBack;
    int       ArgC;
    char    **ArgV;
    char     *CommandLine;
} CPeb;

typedef struct CPcb {
    int    ProcessID;
    int    AppMain;
    int    Binary;
    int    Heap;
    int    Stack;
    CPeb  *Peb;
    int    MemMap;
    int    Pipes;
    int    Events;
    int    Windows;
    int    Screen;
    int    Extent;
} CPcb;

typedef struct IProcess {
    CPcb    *(*Create   )(char *path);
    CPcb    *(*Fork     )(char *path);
    CPcb    *(*Spawn    )(char *path);
    bool     (*Run      )(CPcb *pcb);
    void     (*Exit     )(int code);
    CPcb    *(*GetPcb   )(void);
    CPeb    *(*GetPeb   )(void);
    CCanvas *(*GetCanvas)(void);
    bool     (*Inject   )(ICanvas *canvas, CCanvas *scr, CWindow *curs);
    void     (*DebugPcb)(CPcb *pcb);
    void     (*DebugPeb)(CPeb *peb);
} IProcess;

#pragma pack(pop)
#endif KERNEL_THREAD_PROCESS_H