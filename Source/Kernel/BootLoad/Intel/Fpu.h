#ifndef KERNEL_BOOTLOAD_INTEL_FPU_H
#define KERNEL_BOOTLOAD_INTEL_FPU_H
#include <oop.h>

/* ************************************************************************** */
/* FPU_SR Floating Point Unit Status Register                                 */
/* ************************************************************************** */
#define FPUSR_IE           0x0001 // IE Invalid Operation Exception
#define FPUSR_DE           0x0002 // DE Denormalized Operand Exception
#define FPUSR_ZE           0x0004 // ZE Zero Divide Exception
#define FPUSR_OE           0x0008 // OE Overflow Exception
#define FPUSR_UE           0x0010 // UE Underflow Exception
#define FPUSR_PE           0x0020 // PE Precision Exception
#define FPUSR_SF           0x0040 // SF Stack Fault
#define FPUSR_ES           0x0080 // Exception Summary Status
#define FPUSR_C0           0x0100 // C0 (meaning varies)
#define FPUSR_C1           0x0200 // C1 (meaning varies)
#define FPUSR_C2           0x0400 // C2 Partial Remainder or Incomplete Reduction
#define FPUSR_TOP          0x3800 // Top of Stack Register
#define FPUSR_C3           0x4000 // C3 (meaning varies)
#define FPUSR_B            0x8000 // Busy

/* ************************************************************************** */
/* FPU_CW Floating Point Unit Control Word Register                           */
/* ************************************************************************** */
#define FPUCW_IM           0x0001 // Invalid Operation
#define FPUCW_DM           0x0002 // Denormal Operand
#define FPUCW_ZM           0x0004 // Zero Divide
#define FPUCW_OM           0x0008 // Overflow
#define FPUCW_UM           0x0010 // Underflow
#define FPUCW_PM           0x0020 // Precision Mode
#define FPUCW_PC           0x0300 // Precision Control
#define FPUCW_RC           0x0C00 // Rounding Control
#define FPUCW_X            0x1000 // Infinity Control (legacy)
#define FPUCW_PC_SINGLE    0x0100 // Precision Control
#define FPUCW_PC_DOUBLE    0x0200 // Precision Control
#define FPUCW_PC_EXTENDED  0x0300 // Precision Control
#define FPUCW_RC_TONEAREST 0x0000 // Rounds to nearest integer
#define FPUCW_RC_ROUNDDOWN 0x0400 // down Round toward negative infinity
#define FPUCW_RC_ROUNDUP   0x0800 // Round up towards positive inifinity
#define FPUCW_RC_TOZERO    0x0C00 // Truncate towards zero

/* ************************************************************************** */
/* FPU Control Word Register                                                  */
/* ************************************************************************** */
#define FPUCR_IM        0x00000001 // Invalid Operation Mask
#define FPUCR_DM        0x00000002 // Denormal Operation Mask
#define FPUCR_ZM        0x00000004 // Divide By Zero Mask
#define FPUCR_OM        0x00000008 // Overflow Mask
#define FPUCR_UM        0x00000010 // Underflow Mask
#define FPUCR_PM        0x00000020 // Precision Mask
#define FPUCR_PC        0x00000300 // Rounding Control Mask
#define FPUCR_RC        0x00000C00 // Rounding Control Mask
#define FPUCR_X         0x00001000 // Flush to Zero Mask

/* ************************************************************************** */
/* MXCSR SIMD Control Register                                                */
/* ************************************************************************** */
#define MXCSR_IE        0x00000001 // Invalid Operation Flag
#define MXCSR_DE        0x00000002 // Denormal Flag
#define MXCSR_ZE        0x00000004 // Divide By Zero Flag
#define MXCSR_OE        0x00000008 // Overflow Flag
#define MXCSR_UE        0x00000010 // Underflow Flag
#define MXCSR_PE        0x00000020 // Precision Flag
#define MXCSR_DAZ       0x00000040 // Denormals Are Zero Flag
#define MXCSR_IM        0x00000080 // Invalid Operation Mask
#define MXCSR_DM        0x00000100 // Denormal Operation Mask
#define MXCSR_ZM        0x00000200 // Divide By Zero Mask
#define MXCSR_OM        0x00000400 // Overflow Mask
#define MXCSR_UM        0x00000800 // Underflow Mask
#define MXCSR_PM        0x00001000 // Precision Mask
#define MXCSR_RC        0x00006000 // Rounding Control Mask
#define MXCSR_FZ        0x00008000 // Flush to Zero Mask

void finit(void);
void fninit(void);
u16 *fnstsw(u16 *status);
void fldcw(u16 control);

void fnsave(float *state);
void frstor(float *state);
void fxsave(float *state);
void fxrstor(float *state);
void ldmxcsr(u32 *mxcsr);
void stmxcsr(u32 *mxcsr);
void clts(void);

#endif KERNEL_BOOTLOAD_INTEL_FPU_H