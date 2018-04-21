#include <Kernel.h>

NOTE("************************************************************************")
NOTE("* Intel Architectures Reference Manual (https://goo.gl/46aNJU)         *")
NOTE("************************************************************************")

extern IMonitor *Monitor;
extern IThread  *Thread;
extern IDevice  *Device;

bool DebugCr0(void) {
    u32 cr0 = get_cr0();
    Logger(" CR0    = %X\n", cr0);
    Logger(" CR0.PE = %X (Protection Enabled     )\n", (cr0 & CR0_PE) ? 1 : 0);
    Logger(" CR0.MP = %X (Monitor Co-Processor   )\n", (cr0 & CR0_MP) ? 1 : 0);
    Logger(" CR0.EM = %X (Emulation              )\n", (cr0 & CR0_EM) ? 1 : 0);
    Logger(" CR0.TS = %X (Task Switched          )\n", (cr0 & CR0_TS) ? 1 : 0);
    Logger(" CR0.ET = %X (Extension Type         )\n", (cr0 & CR0_ET) ? 1 : 0);
    Logger(" CR0.NE = %X (Numberic Error         )\n", (cr0 & CR0_NE) ? 1 : 0);
    Logger(" CR0.WP = %X (Write Protect          )\n", (cr0 & CR0_WP) ? 1 : 0);
    Logger(" CR0.AM = %X (Alignment Mask         )\n", (cr0 & CR0_AM) ? 1 : 0);
    Logger(" CR0.NW = %X (Not Write Through      )\n", (cr0 & CR0_NW) ? 1 : 0);
    Logger(" CR0.CD = %X (Cache Disable          )\n", (cr0 & CR0_CD) ? 1 : 0);
    Logger(" CR0.PG = %X (Enables paging when set)\n", (cr0 & CR0_PG) ? 1 : 0);
    Logger("\n");
    return true;
}

bool DebugCr4(void) {
    u32 cr4 = get_cr4();
    Logger(" CR4            = %X\n", cr4);
    Logger(" CR4.VME        = %X (PCD Virtual-8086 Mode Extensions            \n", (cr4 & CR4_VME       ) ? 1 : 0);
    Logger(" CR4.PVI        = %X (PVI Protected-Mode Virtual Interrupts       \n", (cr4 & CR4_PVI       ) ? 1 : 0);
    Logger(" CR4.TSD        = %X (Time Stamp Disable                          \n", (cr4 & CR4_TSD       ) ? 1 : 0);
    Logger(" CR4.DE         = %X (Debugging Extensions                        \n", (cr4 & CR4_DE        ) ? 1 : 0);
    Logger(" CR4.PSE        = %X (Page Size Extensions                        \n", (cr4 & CR4_PSE       ) ? 1 : 0);
    Logger(" CR4.PAE        = %X (Physical Address Extensions                 \n", (cr4 & CR4_PAE       ) ? 1 : 0);
    Logger(" CR4.MCE        = %X (Machine Check Enable                        \n", (cr4 & CR4_MCE       ) ? 1 : 0);
    Logger(" CR4.PGE        = %X (Page Global Enable                          \n", (cr4 & CR4_PGE       ) ? 1 : 0);
    Logger(" CR4.PCE        = %X (Performance Monitoring                      \n", (cr4 & CR4_PCE       ) ? 1 : 0);
    Logger(" CR4.OSFXSR     = %X (OS Supports FXSAVE & FXRSTOR Enables SSE...)\n", (cr4 & CR4_OSFXSR    ) ? 1 : 0);
    Logger(" CR4.OSXMMEXCPT = %X (OS Supports SIMD Exceptions.                \n", (cr4 & CR4_OSXMMEXCPT) ? 1 : 0);
    Logger(" CR4.UMIP       = %X (User Mode Instruction Prevention.           \n", (cr4 & CR4_UMIP      ) ? 1 : 0);
    Logger(" CR4.VMXE       = %X (Virtual Machine Extensions Enable.          \n", (cr4 & CR4_VMXE      ) ? 1 : 0);
    Logger(" CR4.SMXE       = %X (Safer Mode Extensions Enable.               \n", (cr4 & CR4_SMXE      ) ? 1 : 0);
    Logger(" CR4.FSGSBASE   = %X (Enables RDFSBASE,RDGSBASE,WRFSBASE,WRGSBASE)\n", (cr4 & CR4_FSGSBASE  ) ? 1 : 0);
    Logger(" CR4.PCIDE      = %X (Process Context Identifier Enabled          \n", (cr4 & CR4_PCIDE     ) ? 1 : 0);
    Logger(" CR4.OSXSAVE    = %X (Enables the XSAVE extensions.               \n", (cr4 & CR4_OSXSAVE   ) ? 1 : 0);
    Logger(" CR4.SMEP       = %X (Supervisor Mode Access Enabled.             \n", (cr4 & CR4_SMEP      ) ? 1 : 0);
    Logger(" CR4.SMAP       = %X (Supervisor Mode Access Prevention.          \n", (cr4 & CR4_SMAP      ) ? 1 : 0);
    Logger(" CR4.PKE        = %X (Protection Key Enabled.                     \n", (cr4 & CR4_PKE       ) ? 1 : 0);
    Logger("\n");
    return true;
}

void (*DeathLoop)(void);

u32 Crash(void) {
    Logger(" [FAIL] Crashing Gracefully\n");

    CTask *TaskPool = (CTask*)MMAP_TASKPOOL;
    int pid = Thread->GetPid();

    CContext *ctx = (CContext*)TaskPool->CtxList[pid];
    Logger(" pid        = %X\n", pid         );
    Logger(" ctx        = %X\n", ctx         );

    CPcb *pcb = (CPcb*)TaskPool->PcbList[pid];
    Logger(" pcb        = %X\n", pcb         );

    CPeb *peb = (CPeb*)TaskPool->PebList[pid];
    Logger(" peb        = %X\n", peb         );
    Logger(" ctx.cr3    = %X\n", ctx->cr3   );
    Logger(" ctx.ds     = %X\n", ctx->ds    );
    Logger(" ctx.edi    = %X\n", ctx->edi   );
    Logger(" ctx.esi    = %X\n", ctx->esi   );
    Logger(" ctx.ebp    = %X\n", ctx->ebp   );
    Logger(" ctx.esp    = %X\n", ctx->esp   );
    Logger(" ctx.ebx    = %X\n", ctx->ebx   );
    Logger(" ctx.edx    = %X\n", ctx->edx   );
    Logger(" ctx.ecx    = %X\n", ctx->ecx   );
    Logger(" ctx.eax    = %X\n", ctx->eax   );
    Logger(" ctx.eip    = %X\n", ctx->eip   );
    Logger(" ctx.cs     = %X\n", ctx->cs    );
    Logger(" ctx.eflags = %X\n", ctx->eflags);
    Logger(" ctx.esp0   = %X\n", ctx->esp0  );
    Logger(" ctx.ss     = %X\n", ctx->ss    );
    Logger(" ctx.pid    = %X\n", ctx->pid   );
    Logger("\n");

    Logger(" peb        = %X\n", peb         );
    Logger(" ctx.cr3    = %X\n", ctx->cr3   );
    Logger(" ctx.ds     = %X\n", ctx->ds    );
    Logger(" ctx.edi    = %X\n", ctx->edi   );
    Logger(" ctx.esi    = %X\n", ctx->esi   );
    Logger(" ctx.ebp    = %X\n", ctx->ebp   );
    Logger(" ctx.esp    = %X\n", ctx->esp   );
    Logger(" ctx.ebx    = %X\n", ctx->ebx   );
    Logger(" ctx.edx    = %X\n", ctx->edx   );
    Logger(" ctx.ecx    = %X\n", ctx->ecx   );
    Logger(" ctx.eax    = %X\n", ctx->eax   );
    Logger(" ctx.eip    = %X\n", ctx->eip   );
    Logger(" ctx.cs     = %X\n", ctx->cs    );
    Logger(" ctx.eflags = %X\n", ctx->eflags);
    Logger(" ctx.esp0   = %X\n", ctx->esp0  );
    Logger(" ctx.ss     = %X\n", ctx->ss    );
    Logger(" ctx.pid    = %X\n", ctx->pid   );
    Logger("\n");

    CCanvas *canvas = peb->Canvas;
    stosd(canvas->Draw, 0x00AF2F2F, canvas->ResY*canvas->ScanLine/4);
    stosd(canvas->Disp, 0x00AF2F2F, canvas->ResY*canvas->ScanLine/4);
    Thread->Exit(-1);
    for (;;);
}

export bool Install_DeathLoop(void *callback) {
    if (!DeathLoop) DeathLoop = callback;
    return true;
}

bool DebugContext(char *format, ...) {
    static char text[256];

    va_list *list;
    va_start(list, format);
    VaFormat(text, format, list);
    va_end(list);

    CTask *TaskPool = (CTask*)MMAP_TASKPOOL;
    int pid = Thread->GetPid();
    CContext *ctx = (CContext*)TaskPool->CtxList[pid];
    ctx->eip = (DeathLoop) ? (int)DeathLoop : (int)Crash;
    return true;
}

u32 Divide(u32 err, u32 esp) {
    DebugContext(" [FAIL] Divide %X %X\n", err, esp);
    return esp;
}

u32 DebugIrq(u32 err, u32 esp) {
    DebugContext(" [FAIL] Debug %X %X\n", err, esp);
    return esp;
}

u32 NmiInterrupt(u32 err, u32 esp) {
    DebugContext(" [FAIL] Nmi Interrupt %X %X\n", err, esp);
    return esp;
}

u32 Breakpoint(u32 err, u32 esp) {
    DebugContext(" [FAIL] Breakpoint %X %X\n", err, esp);
    return esp;
}

u32 Overflow(u32 err, u32 esp) {
    DebugContext(" [FAIL] Overflow %X %X\n", err, esp);
    return esp;
}

u32 BoundRangeExceeded(u32 err, u32 esp) {
    DebugContext(" [FAIL] Bound Range Exceeded %X %X\n", err, esp);
    return esp;
}

u32 InvalidOpcode(u32 err, u32 esp) {
    DebugContext(" [FAIL] Invalid Opcode %X %X\n", err, esp);
    return esp;
}

u32 FpuNotAvailable(u32 err, u32 esp) {
    DebugContext(" [FAIL] Fpu Not Available %X %X\n", err, esp);
    return esp;
}

u32 DoubleFault(u32 err, u32 esp) {
    DebugContext(" [FAIL] Double Fault %X %X\n", err, esp);
    return esp;
}

u32 FpuSegmentOverrun(u32 err, u32 esp) {
    DebugContext(" [FAIL] Fpu Segment Overrun %X %X\n", err, esp);
    return esp;
}

u32 InvalidTss(u32 err, u32 esp) {
    DebugContext(" [FAIL] Invalid Tss %X %X\n", err, esp);
    return esp;
}

u32 SegmentNotPresent(u32 err, u32 esp) {
    DebugContext(" [FAIL] Segment Not Present %X %X\n", err, esp);
    return esp;
}

u32 SegmentFault(u32 err, u32 esp) {
    DebugContext(" [FAIL] Segment Fault %X %X\n", err, esp);
    return esp;
}

u32 GeneralProtection(u32 err, u32 esp) {
    DebugContext(" [FAIL] General Protection %X %X\n", err, esp);
    return esp;
}

u32 PageFault(u32 err, u32 esp) {
    DebugContext(" [FAIL] Page Fault %X %X\n", err, esp);
    return esp;
}

u32 FpuMath(u32 err, u32 esp) {
    DebugContext(" [FAIL] Fpu Math %X %X\n", err, esp);
    return esp;
}

u32 Alignment(u32 err, u32 esp) {
    DebugContext(" [FAIL] Alignment %X %X\n", err, esp);
    return esp;
}

u32 Machine(u32 err, u32 esp) {
    DebugContext(" [FAIL] Machine %X %X\n", err, esp);
    return esp;
}

u32 SimdFpu(u32 err, u32 esp) {
    u32 mxcsr;
    stmxcsr(&mxcsr);
    Logger("MXCSR_IE  = %X\n", (mxcsr & MXCSR_IE ) ? 1 : 0);
    Logger("MXCSR_IE  = %X\n", (mxcsr & MXCSR_IE ) ? 1 : 0);
    Logger("MXCSR_DE  = %X\n", (mxcsr & MXCSR_DE ) ? 1 : 0);
    Logger("MXCSR_ZE  = %X\n", (mxcsr & MXCSR_ZE ) ? 1 : 0);
    Logger("MXCSR_OE  = %X\n", (mxcsr & MXCSR_OE ) ? 1 : 0);
    Logger("MXCSR_UE  = %X\n", (mxcsr & MXCSR_UE ) ? 1 : 0);
    Logger("MXCSR_PE  = %X\n", (mxcsr & MXCSR_PE ) ? 1 : 0);
    Logger("MXCSR_DAZ = %X\n", (mxcsr & MXCSR_DAZ) ? 1 : 0);
    Logger("MXCSR_IM  = %X\n", (mxcsr & MXCSR_IM ) ? 1 : 0);
    Logger("MXCSR_DM  = %X\n", (mxcsr & MXCSR_DM ) ? 1 : 0);
    Logger("MXCSR_ZM  = %X\n", (mxcsr & MXCSR_ZM ) ? 1 : 0);
    Logger("MXCSR_OM  = %X\n", (mxcsr & MXCSR_OM ) ? 1 : 0);
    Logger("MXCSR_UM  = %X\n", (mxcsr & MXCSR_UM ) ? 1 : 0);
    Logger("MXCSR_PM  = %X\n", (mxcsr & MXCSR_PM ) ? 1 : 0);
    Logger("MXCSR_RC  = %X\n", (mxcsr & MXCSR_RC ) ? 1 : 0);
    Logger("MXCSR_FZ  = %X\n", (mxcsr & MXCSR_FZ ) ? 1 : 0);
    DebugContext(" [FAIL] Simd Fpu %X %X\n", err, esp);
    return esp;
}

u32 Virtualization(u32 err, u32 esp) {
    DebugContext(" [FAIL] Virtualization  %X %X\n", err, esp);
    return esp;
}

bool IException_Install(void) {
    Device->Latch(0x00, Divide            );
    Device->Latch(0x01, DebugIrq          );
    Device->Latch(0x02, NmiInterrupt      );
    Device->Latch(0x03, Breakpoint        );
    Device->Latch(0x04, Overflow          );
    Device->Latch(0x05, BoundRangeExceeded);
    Device->Latch(0x06, InvalidOpcode     );
    Device->Latch(0x07, FpuNotAvailable   );
    Device->Latch(0x08, DoubleFault       );
    Device->Latch(0x09, FpuSegmentOverrun );
    Device->Latch(0x0A, InvalidTss        );
    Device->Latch(0x0B, SegmentNotPresent );
    Device->Latch(0x0C, SegmentFault      );
    Device->Latch(0x0D, GeneralProtection );
    Device->Latch(0x0E, PageFault         );
    Device->Latch(0x10, FpuMath           );
    Device->Latch(0x11, Alignment         );
    Device->Latch(0x12, Machine           );
    Device->Latch(0x13, SimdFpu           );
    Device->Latch(0x14, Virtualization    );
    return true;
}

static IException exception = {
    IException_Install
};

export IException *Exception = &exception;