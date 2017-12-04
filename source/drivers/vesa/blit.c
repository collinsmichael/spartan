#include <memory.h>

_declspec(dllexport)
int blit(void *screen) {
    int resx = 1024;
    int resy =  768;
    int bits =   32;
    int scrx = 1024;
    char *frame = 0;
    char *image = 0;
    for (int y = 0; y < resy; y++) {
        memcpy(&frame[y*scrx], &image[y*resx], resx*bits/8);
    }
    return 1;
}