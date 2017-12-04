#define _CRT_SECURE_NO_WARNINGS
#include <inode.h>

extern char *image;
extern volume_t *volume;
extern block_t  *blocks;
extern inode_t  *inodes;

/* ************************************************************************** */
/* allocate a block of disk space to an inode                                 */
/* ************************************************************************** */
int dalloc(int size) {
    struct block_t *block;
    int base;
    align(size, PAGE_SIZE);
    for (block = blocks; block->size; block++) {
        if (block->size >= size) {
            base = block->base;
            block->base += size;
            if ((block->size -= size) == 0) do {
                block++;
                (block-1)->base = block->base;
            } while ((block-1)->size = block->size);
            volume->disk_free -= size;
            volume->disk_used += size;
            return base;
        }
    }
    return 0;
}

/* ************************************************************************** */
/* release a block of disk space which was assigned to an inode               */
/* ************************************************************************** */
void dfree(int addr, int size) {
	struct block_t *block;
	int len;
	int base = addr;
    align(size, PAGE_SIZE);
	for (block = blocks; block->base <= base && block->size != 0; block++);
	if (block > blocks && (block-1)->base + (block-1)->size == base) {
		(block-1)->size += size;
		if (base + size == block->base) {
			(block-1)->size += block->size;
			while (block->size) {
				block++;
				(block-1)->base = block->base;
				(block-1)->size = block->size;
			}
		}
	} else {
		if (base + size == block->base && block->size) {
			block->base -= size;
			block->size += size;
		} else if (size) do {
        	int temp = block->base;
			block->base = base;
			base = temp;
			len = block->size;
			block->size = size;
			block++;
		} while (size = len);
	}
    volume->disk_free += size;
    volume->disk_used -= size;
}