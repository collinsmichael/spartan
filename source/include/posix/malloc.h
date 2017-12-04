#ifndef MALLOC_H
#define MALLOC_H

void *malloc(int size);
int mfree(void *buffer, int size);

#endif // MALLOC_H