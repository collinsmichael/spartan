#include <libk.h>
#include <stddef.h>
#include <stdio.h>
#include <string.h>

_declspec(dllexport)
driver_t *devices[256];
static int count;

#undef dopen
_declspec(dllexport)
FILE *dopen(char *path, char *access) {
    for (int dev = 1; dev < count; dev++) {
        if (strcmp(path, devices[dev]->path) != 0) continue;
        // TODO: Check access rights
        return (FILE*)devices[dev];
    }
    return 0;
}