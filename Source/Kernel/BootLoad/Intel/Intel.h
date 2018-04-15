#ifndef KERNEL_BOOTLOAD_INTEL_H
#define KERNEL_BOOTLOAD_INTEL_H
#include <oop.h>

// string/memory operations
void  movsb(void *dst, void *src, int len);
void  movsw(void *dst, void *src, int len);
void  movsd(void *dst, void *src, int len);
int   cmpsb(void *dst, void *src, int len);
int   cmpsw(void *dst, void *src, int len);
int   cmpsd(void *dst, void *src, int len);
void  stosb(void *dst, int val, int len);
void  stosw(void *dst, int val, int len);
void  stosd(void *dst, int val, int len);
void *scasb(void *dst, int val, int len);
void *scasw(void *dst, int val, int len);
void *scasd(void *dst, int val, int len);
int   rol(int value, int count);
int   ror(int value, int count);
void  swint(int irq);

// device io
void sti(void);
void cli(void);
int  inb(int port);
int  inw(int port);
int  ind(int port);
void outb(int port, int value);
void outw(int port, int value);
void outd(int port, int value);

// control register manipulation
void lgdt(void *gdtr);
void lidt(void *idtr);
void ltr(int seg);
u64 rdtsc(void);

u32  get_cr0(void);
u32  set_cr0(u32 value);
u32  get_cr3(void);
u32  set_cr3(void *value);
u32  get_cr4(void);
u32  set_cr4(u32 value);
u64  cpuid(int feature, int reg[4]);


// jumps to user mode application
void ring3(void *ctx);

/* ************************************************************************** */
/* EFER Extended Features Enable Register                                     */
/* ************************************************************************** */
#define EFER_SCE        0x00000001 // SYSCALL Enable: IA32_EFER.SCE (R/W)
#define EFER_LME        0x00000100 // IA-32e Mode Enable: IA32_EFER.LME (R/W)
#define EFER_LMA        0x00000400 // IA-32e Mode Active: IA32_EFER.LMA (R)
#define EFER_NXE        0x00000800 // XD Execute Disable Bit Enable: IA32_EFER.NXE (R/W)

/* ************************************************************************** */
/* EFLAGS Extended Flags Register                                             */
/* ************************************************************************** */
#define EFLAGS_CF       0x00000001 // CF Carry Flag
#define EFLAGS_ONE      0x00000002 // Always 1
#define EFLAGS_PF       0x00000004 // PF Positive Flag
#define EFLAGS_AF       0x00000010 // AF Above (Greater Than) Flag
#define EFLAGS_ZF       0x00000040 // ZF Zero (Equal) Flag
#define EFLAGS_SF       0x00000080 // SF Signed (Negative) Flag
#define EFLAGS_TF       0x00000100 // TF Trap — Set to enable single-step mode for debugging;
#define EFLAGS_IF       0x00000200 // IF Interrupt Enable Flag
#define EFLAGS_IOPL     0x00003000 // IO Privilege Level (0-3)
#define EFLAGS_NT       0x00004000 // NT Nested Task Enable (Hardware Task Switching)
#define EFLAGS_RESUME   0x00010000 // RESUME Controls response to Break instructions
#define EFLAGS_V8086    0x00020000 // V8086 Virtual 8086 Mode Enabled
#define EFLAGS_AC       0x00040000 // AC Alignment Check (Enabled if AM bit in CR0 is set)
#define EFLAGS_VIF      0x00080000 // VIF Virtual Interrupt Flag (Virtual Image of VME flag during interrupts)
#define EFLAGS_VIP      0x00100000 // VIP Virtual Interrupt Pending
#define EFLAGS_CPUID    0x00200000 // CPUID If you can toggle this bit then CPUID opcode is available

/* ************************************************************************** */
/* Control Register 0                                                         */
/* ************************************************************************** */
#define CR0_PE          0x00000001 // Protection Enabled
#define CR0_MP          0x00000002 // Monitor Co-Processor
#define CR0_EM          0x00000004 // Emulation
#define CR0_TS          0x00000008 // Task Switched
#define CR0_ET          0x00000010 // Extension Type
#define CR0_NE          0x00000020 // Numberic Error
#define CR0_WP          0x00010000 // Write Protect
#define CR0_AM          0x00040000 // Alignment Mask
#define CR0_NW          0x20000000 // Not Write Through
#define CR0_CD          0x40000000 // Cache Disable
#define CR0_PG          0x80000000 // Enables paging when set

/* ************************************************************************** */
/* Control Register 1                                                         */
/* ************************************************************************** */
#define CR1_RESERVED    0x00000000 // All Fields Reserved by Intel

/* ************************************************************************** */
/* Control Register 2                                                         */
/* ************************************************************************** */
#define CR2_PFLA        0xFFFFFFFF // Page-Fault Linear Address

/* ************************************************************************** */
/* Control Register 3                                                         */
/* ************************************************************************** */
#define CR3_PWT         0x00000008 // PWT Page Level Write Through
#define CR3_PCD         0x00000010 // PCD Page Level Cache Disable

/* ************************************************************************** */
/* Control Register 4                                                         */
/* ************************************************************************** */
#define CR4_VME         0x00000001 // PCD Virtual-8086 Mode Extensions
#define CR4_PVI         0x00000002 // PVI Protected-Mode Virtual Interrupts
#define CR4_TSD         0x00000004 // Time Stamp Disable
#define CR4_DE          0x00000008 // Debugging Extensions
#define CR4_PSE         0x00000010 // Page Size Extensions
#define CR4_PAE         0x00000020 // Physical Address Extensions
#define CR4_MCE         0x00000040 // Machine Check Enable
#define CR4_PGE         0x00000080 // Page Global Enable
#define CR4_PCE         0x00000100 // Performance Monitoring
#define CR4_OSFXSR      0x00000200 // OS Support for FXSAVE and FXRSTOR Enables SSE/SSE2/SSE3/SSSE3/SSE4 support.
#define CR4_OSXMMEXCPT  0x00000400 // OS Support for SIMD Exceptions.
#define CR4_UMIP        0x00000800 // User Mode Instruction Prevention.
#define CR4_VMXE        0x00002000 // Virtual Machine Extensions Enable.
#define CR4_SMXE        0x00004000 // Safer Mode Extensions Enable.
#define CR4_FSGSBASE    0x00010000 // Enables the instructions RDFSBASE, RDGSBASE, WRFSBASE, and WRGSBASE.
#define CR4_PCIDE       0x00020000 // Process Context Identifier Enabled
#define CR4_OSXSAVE     0x00040000 // Enables the XSAVE extensions.
#define CR4_SMEP        0x00100000 // Supervisor Mode Access Enabled.
#define CR4_SMAP        0x00200000 // Supervisor Mode Access Prevention.
#define CR4_PKE         0x00400000 // Protection Key Enabled.

/* ************************************************************************** */
/* Control Register 8                                                         */
/* ************************************************************************** */
#define CR8_TPL         0x0000000F // Task Priority Level (0-7)

/* ************************************************************************** */
/* Interrupt Requests                                                         */
/* ************************************************************************** */
#define IRQ_DIVIDE               0x00
#define IRQ_DEBUG                0x01
#define IRQ_NMI_INTERRUPT        0x02
#define IRQ_BREAKPOINT           0x03
#define IRQ_OVERFLOW             0x04
#define IRQ_BOUND_RANGE_EXCEEDED 0x05
#define IRQ_INVALID_OPCODE       0x06
#define IRQ_FPU_NOT_AVAILABLE    0x07
#define IRQ_DOUBLE_FAULT         0x08 // (error code)
#define IRQ_FPU_SEGMENT_OVERRUN  0x09
#define IRQ_INVALID_TSS          0x0A // (error code)
#define IRQ_SEGMENT_NOT_PRESENT  0x0B // (error code)
#define IRQ_SEGMENT_FAULT        0x0C // (error code)
#define IRQ_GENERAL_PROTECTION   0x0D // (error code)
#define IRQ_PAGE_FAULT           0x0E // (error code)
#define IRQ_FPU_MATH             0x10
#define IRQ_ALIGNMENT            0x11 // (error code)
#define IRQ_MACHINE              0x12
#define IRQ_SIMD_FPU             0x13
#define IRQ_VIRTUALIZATION       0x14

#define IRQ_PIT                  0x20
#define IRQ_KBD                  0x21
#define IRQ_PIC1                 0x22
#define IRQ_COM2                 0x23
#define IRQ_COM1                 0x24
#define IRQ_PARA2                0x25
#define IRQ_FDD                  0x26
#define IRQ_PARA1                0x27
#define IRQ_RTC                  0x28
#define IRQ_ACPI                 0x29
#define IRQ_SCSI_NIC1            0x2A
#define IRQ_SCSI_NIC2            0x2B
#define IRQ_MOUSE                0x2C
#define IRQ_FPU                  0x2D
#define IRQ_ATA1                 0x2E
#define IRQ_ATA2                 0x2F

#define IRQ_SYS                  0x80
#define IRQ_YIELD                0x81

#endif KERNEL_BOOTLOAD_INTEL_H