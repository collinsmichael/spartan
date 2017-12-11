#ifndef ARCH_H
#define ARCH_H
#include <stdint.h>
#include <pic.h>
#pragma pack(push, 1)

typedef struct TSS {
    uint32_t BackLink;
    uint32_t esp0;
    uint32_t ss0;
    uint32_t esp1;
    uint32_t ss1;
    uint32_t esp2;
    uint32_t ss2;
    uint32_t cr3;
    uint32_t eip;
    uint32_t eflags;
    uint32_t eax;
    uint32_t ecx;
    uint32_t edx;
    uint32_t ebx;
    uint32_t esp;
    uint32_t ebp;
    uint32_t esi;
    uint32_t edi;
    uint32_t es;
    uint32_t cs;
    uint32_t ss;
    uint32_t ds;
    uint32_t fs;
    uint32_t gs;
    uint32_t ldt;
    uint32_t pio;
    uint32_t pad[4];
} TSS;

void cli(void);
void sti(void);
void lgdt(void *gdtr, int len);
void lidt(void *idtr, int len);
void ltr(int tss);

int inb(int port);
int inw(int port);
int ind(int port);
int outb(int port, int value);
int outw(int port, int value);
int outd(int port, int value);

void gdt_reset(void);
void tss_reset(void);
void idt_reset(void);
void idt_install(int irq, void *(*handler)(void *esp));
void idt_remove(int irq);

#define GDT_NULL       (0x00+0)
#define GDT_RING0_CODE (0x08+0)
#define GDT_RING0_DATA (0x10+0)
#define GDT_RING3_CODE (0x18+3)
#define GDT_RING3_DATA (0x20+3)
#define TSS_RING3_TASK (0x28+3)

#define GDT_BASE ((char *)(0x00000600))
#define IDT_BASE ((char *)(0x00004000))
#define TSS_BASE ((TSS  *)(0x00000680))
#define PGTBL    ((void**)(0x00000000))

#pragma pack(pop)
#endif // ARCH_H