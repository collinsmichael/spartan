#ifndef DRIVER_H
#define DRIVER_H
#include <stddef.h>
#pragma pack(push, 1)

FILE *dopen(char *path, char *access);

#pragma pack(pop)
#endif // DRIVER_H