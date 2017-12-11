/* ************************************************************************** */
/* author(s) : Michael Collins                                                */
/* file name : include/spartan/pipe.h                                         */
/* file date : 2017/12/03                                                     */
/* file info : inter-process communication                                    */
/* ************************************************************************** */
#ifndef PIPE_H
#define PIPE_H
#include <stdint.h>
#pragma pack(push, 1)

typedef struct PIPE {
    uint32_t head; // read head
    uint32_t tail; // write head
    uint32_t size; // size of ring buffer
    uint8_t *ring; // ring buffer
} PIPE;

PIPE *pipe(void);
int pclose(PIPE *fd);
int read(PIPE *fd, char *buffer, int size);
int write(PIPE *fd, char *buffer, int size);
int pblock(PIPE *fd);
int punblock(PIPE *fd);

#pragma pack(pop)
#endif // PIPE_H