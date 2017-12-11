/* ************************************************************************** */
/* author(s) : Michael Collins                                                */
/* file name : include/spartan/arch.h                                         */
/* file date : 2017/12/03                                                     */
/* file info : architecture specific abstraction layer                        */
/* ************************************************************************** */
#ifndef ARCH_H
#define ARCH_H
#include <stdint.h>

uint8_t inb(uint16_t port);
uint16_t inw(uint16_t port);
uint32_t ind(uint16_t port);
void outb(uint16_t port, uint8_t value);
void outw(uint16_t port, uint8_t value);
void outd(uint16_t port, uint8_t value);

#endif // ARCH_H