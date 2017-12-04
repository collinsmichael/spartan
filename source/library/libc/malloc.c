/* ========================================================================= */
/* File Name : source/library/malloc.c                                       */
/* File Date : 5 February 2015                                               */
/* Author(s) : Michael Collins                                               */
/* ------------------------------------------------------------------------- */
/* basic memory management system using a first fit algorithm.               */
/* ========================================================================= */
#include <libk.h>
#include <stdint.h>

_declspec(dllexport)
void *malloc(uint32_t size) {
    return kalloc(size);
}

_declspec(dllexport)
void mfree(void *addr, uint32_t size) {
    kfree(addr, size);
}