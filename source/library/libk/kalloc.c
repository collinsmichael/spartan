/* ========================================================================= */
/* File Name : source/library/malloc.c                                       */
/* File Date : 5 February 2015                                               */
/* Author(s) : Michael Collins                                               */
/* ------------------------------------------------------------------------- */
/* basic memory management system using a first fit algorithm.               */
/* ========================================================================= */
#include <inode.h>
#include <stdint.h>
#include <stddef.h>

struct block_t *memmap;

/* --------------------------------------------------------- */
/* Allocate size units  from the given map.  Return the base */
/* of the allocated space. Algorithm is first fit.           */
/* --------------------------------------------------------- */
_declspec(dllexport)
void *kalloc(uint32_t size) {
    register struct block_t *bp;
    for (bp = memmap; bp->size; bp++) {
        if (bp->size >= size) {
            register int a = bp->base;
            bp->base += size;
            if ((bp->size -= size) == 0) do {
                bp++;
                (bp-1)->base = bp->base;
            } while ((bp-1)->size = bp->size);
            return (void*)a;
        }
    }
    return 0;
}

/* --------------------------------------------------------- */
/* Free the  previously allocated  space addr of  size units */
/* into the specified map. Sort addr into map and combine on */
/* one or both ends if possible.                             */
/* --------------------------------------------------------- */
_declspec(dllexport)
void kfree(void *addr, uint32_t size) {
	register struct block_t *bp;
	register uint32_t t;
	register uint32_t s;
	register uint32_t a = (uint32_t )addr;
	for (bp = memmap; bp->base <= a && bp->size != 0; bp++);
	if (bp > memmap && (bp-1)->base + (bp-1)->size == a) {
		(bp-1)->size += size;
		if (a + size == bp->base) {
			(bp-1)->size += bp->size;
			while (bp->size) {
				bp++;
				(bp-1)->base = bp->base;
				(bp-1)->size = bp->size;
			}
		}
	} else {
		if (a + size == bp->base && bp->size) {
			bp->base -= size;
			bp->size += size;
		} else if (size) do {
			t = bp->base;
			bp->base = a;
			a = t;
			s = bp->size;
			bp->size = size;
			bp++;
		} while (size = s);
	}
}

_declspec(dllexport)
void mmstart(struct block_t *mm) {
    memmap = mm;
}