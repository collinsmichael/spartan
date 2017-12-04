/* ************************************************************************** */
/* author(s) : Michael Collins                                                */
/* file name : include/spartan/libk.h                                         */
/* file date : 2017/12/03                                                     */
/* file info : kernel libc definitions                                        */
/* ************************************************************************** */
#ifndef LIBK_H
#define LIBK_H
#include <stdint.h>
#include <stddef.h>
#pragma pack(push, 1)

// this structure must match stddef.h FILE
typedef struct driver_t {
    int   inode;
    int   head;
    int   tail;
    char *ring;
	int (*xopen )(char *path, char *access);
	int (*xclose)(FILE *file);
	int (*xsize )(FILE *file);
	int (*xseek )(FILE *file, int offset, int method);
	int (*xgetc )(FILE *file);
	int (*xputc )(int ch, FILE *file);
	int (*xinit)(char *path, char *access);
	void (*isr)(void);
    int   interrupt;
    char *name;
    char *path;
    char *access;
} driver_t;

void *kalloc(uint32_t size);
void kfree(void *addr, uint32_t size);

#pragma pack(pop)
#endif // LIBK_H