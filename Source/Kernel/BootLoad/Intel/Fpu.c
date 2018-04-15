#include <Kernel.h>

bool fpu = false;
bool mmx = false;
bool sse = false;
bool tsc = false;

bool SseEnable(void) {
    u32 reg[4];
    cpuid(0x000000001, reg);
    mmx = ((reg[3] & (1 << 23)) != 0); // CPUID page 1, in EDX bit 23
    sse = ((reg[3] & (1 << 25)) != 0); // CPUID page 1, in EDX bit 25
    if (sse) {
        u32 cr4 = get_cr4() | CR4_OSFXSR|CR4_OSXMMEXCPT;
        set_cr4(cr4);
        // when CR4.OSFXSR     = 0 fnsave and frstor save x87 FPU and MMX state
        // when CR4.OSFXSR     = 1 fxsave and fxrstor save SSE XMM state
        // when CR4.OSXMMEXCPT = 0 SIMD errors generate invalid opcode exceptions
        // when CR4.OSXMMEXCPT = 1 SIMD errors generate SIMD FPU exceptions

        u32 mxcsr = 0;
        mxcsr |= MXCSR_FZ | MXCSR_RC;
        mxcsr |= (MXCSR_IM|MXCSR_DM|MXCSR_ZM|MXCSR_OM|MXCSR_UM|MXCSR_PM);
        ldmxcsr(&mxcsr);
    }
    return sse;
}

export bool FpuEnable(void) {
    u32 cr0 = (get_cr0() & ~(CR0_EM|CR0_MP)) | (CR0_NE);
    set_cr0(cr0);
    // when CR0.EM = 0 (Emulation) FPU is an internal to the processor and floating point opcodes may be executed
    // when CR0.EM = 1 (Emulation) FPU is an external device and executing any floating point opcodes raises an exception
    // when CR0.MP = 0 (Monitor FPU) FWAIT ignored
    // when CR0.MP = 1 (Monitor FPU) FWAIT generated device not available exception
    // when CR0.NE = 0 (Numeric Error) external FPU errors (raises IRQs)
    // when CR0.NE = 1 (Numeric Error) internal FPU method (exceptions which are much faster)

    fninit(); // initialize floating point unit
    u16 fpusw = 'xx';
    fnstsw(&fpusw);         // get the status word
    fpu = (fpusw != 'xx');  // sanity check
    if (!fpu) return false;

    u16 fpucw = FPUCW_PC_EXTENDED
              | FPUCW_RC_TONEAREST // |FPUCW_RC_TOZERO|FPUCW_RC_ROUNDDOWN|FPUCW_RC_ROUNDUP
              | FPUCW_IM | FPUCW_DM | FPUCW_ZM | FPUCW_OM
              | FPUCW_UM | FPUCW_PM | 0x40;
    fldcw(fpucw);
    return true;
}

bool TscEnable(void) {
    u32 cr4 = get_cr4();
    cr4 = cr4 & ~CR4_TSD; // enable user mode RDTSC
    cr4 = cr4 & ~CR4_PCE; // enable user mode RDPMC
    set_cr4(cr4);

    // when CR4.TSD = 0 RDTSC may only be executed from ring0
    // when CR4.TSD = 1 RDTSC may be executed from user mode
    // when CR4.PCE = 0 RDPMC may be executed from user mode
    // when CR4.PCE = 1 RDPMC may only be executed from ring0
    tsc = true;
    return true;
}


export bool EnableFpu(void) {
    FpuEnable();
    SseEnable();
    TscEnable();

    Logger(" [info] CPU Features %s %s %s %s\n",
        fpu ? "FPU" : "",
        mmx ? "MMX" : "",
        sse ? "SSE" : "",
        tsc ? "TSC" : "");
    return true;
}

char x87[128] =
"\x7F\x0F\xFF\xFF\x00\x00\xFF\xFF\xFF\xFF\xFF\xFF\x00\x00\x00\x00"
"\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\xFF\xFF\x00\x00\x00\x00"
"\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00"
"\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00"
"\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00"
"\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00"
"\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x78\x78\x78\x78"
"\x78\x78\x78\x78\x78\x78\x78\x78\x78\x78\x78\x78\x78\x78\x78\x78";

char simd[320] =
"\x7F\x0F\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00"
"\x00\x00\x00\x00\x00\x00\x00\x00\x80\xFF\x00\x00\xFF\xFF\x00\x00"
"\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00"
"\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00"
"\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00"
"\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00"
"\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00"
"\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00"
"\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00"
"\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00"
"\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00"
"\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00"
"\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00"
"\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00"
"\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00"
"\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00"
"\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00"
"\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00"
"\x78\x78\x78\x78\x78\x78\x78\x78\x78\x78\x78\x78\x78\x78\x78\x78"
"\x78\x78\x78\x78\x78\x78\x78\x78\x78\x78\x78\x78\x78\x78\x78\x78";
