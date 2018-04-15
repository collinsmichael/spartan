#define IMPL_IMONITOR
#include <Kernel.h>

NOTE("************************************************************************")
NOTE("* Intel Architectures Reference Manual (https://goo.gl/46aNJU)         *")
NOTE("*                                                                      *")
NOTE("* the Task State Segment contains context during context switches      *")
NOTE("* entries are (mostly) the contents of registers                       *")
NOTE("*                                                                      *")
NOTE("* Jargon: Registers are on-chip memory the CPU uses for rough work     *")
NOTE("* Jargon: Context switching allows the CPU to switch between tasks     *")
NOTE("************************************************************************")

extern IMonitor *Monitor;

bool InstallTss(u8 *base, u32 size) {
    CTss *tss = (CTss*)base;
    stosd(tss, 0, sizeof(CTss)/4);
    tss->esp0   = (u32)Monitor->GetBase('STAK')+4*KB;
    tss->esp    = (u32)Monitor->GetBase('TOPS')+4*KB;
    tss->cr3    = (u32)Monitor->GetBase('PAGE');
    tss->ss0    = RING0_DATA;
    tss->ss     = RING3_DATA;
    tss->cs     = RING3_CODE;
    tss->ds     = RING3_DATA;
    tss->es     = RING3_DATA;
    tss->fs     = RING3_DATA;
    tss->gs     = RING3_DATA;
    tss->eflags = EFLAGS_IOPL + EFLAGS_IF;
    return true;
}

bool EnableTss(u8 *base, u32 size) {
    ltr(RING3_TASK); // load task register with ring3 task
    return true;
}