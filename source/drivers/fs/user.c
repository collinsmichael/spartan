#include <string.h>

_declspec(dllexport)
int mkflags(char *access) {
    int mask = 0;
    while (*access) {
        mask = mask + mask;
        if (*access++ != '-') mask++;
    }
    return mask;
}

_declspec(dllexport)
int user(void) {
    return 0x00;
}

_declspec(dllexport)
int group(void) {
    return 0x00;
}

_declspec(dllexport)
int chmod(char *path, char *access) {
    return 0;
}

static char dir[256];
_declspec(dllexport)
char *getdir(void) {
    return dir;
}

_declspec(dllexport)
void setdir(char *path) {
    strncpy(dir, path, sizeof(dir)-1);
}