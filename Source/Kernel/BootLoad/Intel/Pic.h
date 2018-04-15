#ifndef KERNEL_BOOTLOAD_INTEL_PIC_H
#define KERNEL_BOOTLOAD_INTEL_PIC_H

/* ************************************************************************** */
/* Programmable Interrupt Controller Port Mappings                            */
/* ************************************************************************** */
#define PIC1_CMD  0x20
#define PIC2_CMD  0xA0
#define PIC1_DATA 0x21
#define PIC2_DATA 0xA1

/* ************************************************************************** */
/* End of Interrupt Signal                                                    */
/* ************************************************************************** */
#define EOI       0x20

/* ************************************************************************** */
/* Internal Registers                                                         */
/* ************************************************************************** */
#define PIC_CR       // CR (-W) Command Register holds the commands sent to the PIC
#define PIC_SR       // SR (R-) Status Register
#define PIC_IR       // IR (--) Interrupt Request Register (internal bit flags for pending interrupts)
#define PIC_IS       // IS (--) In-Service Register (internal bit flags for interrupts which have not yet been acknowleged)
#define PIC_IM       // IM (RW) Interrupt Mask Register (mask bits for disabled interrupt requests)

/* ************************************************************************** */
/* Control Words (Commands)                                                   */
/* ************************************************************************** */
#define ICW1      0x00 // This control word begins initialization process
#define ICW2      0x00 // This control word is the vector IRQ mappings begin from
#define ICW3      0x00 // This control word defines the master slave connection topology
#define ICW4      0x00 // This control word sets interrupt mode

#define ICW1_INIT 0x10 // Reset and Initialize PIC
#define ICW1_ICW4 0x01 // indicates that we wish to also initialize ICW4
#define ICW3_IRQ  0x04 // This tells master what IRQ to assign to slave
#define ICW3_ID   0x02 // This tells slave its ID (2 = slave)
#define ICW4_8086 0x01 // This control word sets interrupt mode

/* ************************************************************************** */
/* Device IRQ's                                                               */
/* ************************************************************************** */
#define PIC1_SPK  0x01 // IRQ0        0x08    Timer                                              
#define PIC1_KBD  0x02 // IRQ1        0x09    Keyboard                                           
#define PIC1_PIC2 0x04 // IRQ2        0x0A    Cascade for 8259A Slave controller                 
#define PIC1_COM1 0x08 // IRQ3        0x0B    Serial port 2                                      
#define PIC1_COM2 0x10 // IRQ4        0x0C    Serial port 1                                      
#define PIC1_NUL1 0x20 // IRQ5        0x0D    AT systems: Parallel Port 2. PS/2 systems: reserved
#define PIC1_DISK 0x40 // IRQ6        0x0E    Diskette drive                                     
#define PIC1_PAR1 0x80 // IRQ7        0x0F    Parallel Port 1                                    
#define PIC2_RTC  0x01 // IRQ8/IRQ0   0x70    CMOS Real time clock                               
#define PIC2_CGA  0x02 // IRQ9/IRQ1   0x71    CGA vertical retrace                               
#define PIC2_NUL1 0x04 // IRQ10/IRQ2  0x72    Reserved                                           
#define PIC2_NUL2 0x08 // IRQ11/IRQ3  0x73    Reserved                                           
#define PIC2_AUX  0x10 // IRQ12/IRQ4  0x74    AT systems: reserved. PS/2: auxiliary device       
#define PIC2_FPU  0x20 // IRQ13/IRQ5  0x75    FPU                                                
#define PIC2_HDD  0x40 // IRQ14/IRQ6  0x76    Hard disk controller                               
#define PIC2_NUL3 0x80 // IRQ15/IRQ7  0x77    Reserved                                           

#endif KERNEL_BOOTLOAD_INTEL_PIC_H