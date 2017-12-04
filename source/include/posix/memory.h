#ifndef MEMORY_H
#define MEMORY_H

void *memchr(const void *buffer, int c, int size);
int   memcmp(const void *target, const void *source, int size);
void *memcpy(void *target, const void *source, int size);
void *memset(void *buffer, int c, int size);

#endif // MEMORY_H