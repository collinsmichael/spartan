/* ************************************************************************** */
/* source/kernel/pipe.c                                                       */
/* Pipes are used for buffered IO. Pipes have a fixed sized buffer. They have */
/* a read head and a write head.                                              */
/* When a thread reads from an empty pipe then it becomes IO blocked and will */
/* reawakend when data is written to the pipe.                                */
/* When a thread writes to a full pipe it becomes IO blocked  and will awaken */
/* after data has been read from the pipe.                                    */
/* ************************************************************************** */
#include <limits.h>
#include <libk.h>
#include <pipe.h>

PIPE *pipe(void) {
    PIPE *p = (PIPE*)kalloc(sizeof(PIPE) + PIPE_LIMIT);
    if (!p) return 0;
    int fd = (int)p;
    p->head = p->tail = 0;
    p->size = PIPE_LIMIT;
    p->ring = (uint8_t*)(fd + sizeof(PIPE));
    return p;
}

int pclose(PIPE *fd) {
    if (!fd) return 0;
    kfree(fd, sizeof(PIPE) + PIPE_LIMIT);
    return 1;
}

int read(PIPE *fd, char *buffer, int size) {
    if (!fd) return 0;
    while (size--) {
        if (fd->head == fd->tail) pblock(fd);
        fd->ring[fd->head++] = *buffer++;
        fd->head %= fd->size;
    }
    return 1;
}

int write(PIPE *fd, char *buffer, int size) {
    if (!fd) return 0;
    while (size--) {
        if (fd->head == fd->tail) pblock(fd);
        fd->ring[fd->tail++] = *buffer++;
        fd->tail %= fd->size;
    }
    return 1;
}

int pblock(PIPE *fd) {
    if (!fd) return 0;
    // tell the kernel that this thread is IO blocked
    return 1;
}

int punblock(PIPE *fd) {
    if (!fd) return 0;
    // tell the kernel that this thread is ready to run
    return 1;
}