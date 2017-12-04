/* ************************************************************************** */
/* source/stdlib/memory.c                                                     */
/* memory runtime support functions                                           */
/* ************************************************************************** */
_declspec(dllexport)
void *memchr(const char *buffer, int c, int size) {
    while (size--) {
        if (*buffer == c) return (void*)buffer;
        else buffer++;
    }
    return 0;
}
/*
_declspec(dllexport)
int memcmp(const char *target, const char *source, int size) {
    while (size--) {
        char a = *target++;
        char b = *source++;
        if (a < b) return -1;
        if (a > b) return 1;
    }
    return 0;
}

_declspec(dllexport)
void *memcpy(char *target, const char *source, int size) {
    while (size--) *target++ = *source++;
    return target;
}

_declspec(dllexport)
void *memset(char *buffer, int c, int size) {
    while (size--) *buffer++ = c;
    return buffer;
}
*/

_declspec(dllexport)
void *memset(void *dst, int val, int len) {
    long *ltarget = (long*)dst;
    long  lvalue = val*0x01010101;
    while (len >= 4) {
        *ltarget++ = lvalue;
        len -= 4;
    }
    char *btarget = (char*)ltarget;
    char bvalue = (char)val;
    while (len--) *btarget++ = bvalue;
    return dst;
}

_declspec(dllexport)
void *memcpy(void *dst, void *src, int len) {
    long *ltarget = (long*)dst;
    long *lsource = (long*)src;
    while (len >= 4) {
        *ltarget++ = *lsource++;
        len -= 4;
    }
    char *btarget = (char*)ltarget;
    char *bsource = (char*)lsource;
    while (len--) *btarget++ = *bsource++;
    return dst;
}

_declspec(dllexport)
int memcmp(void *dst, void *src, int len) {
    long *ltarget = (long*)dst;
    long *lsource = (long*)src;
    while (len >= 4) {
        int n = (*ltarget++) - (*lsource++);
        if (n != 0) return n;
        len -= 4;
    }
    char *btarget = (char*)ltarget;
    char *bsource = (char*)lsource;
    while (len--) {
        char n = (*btarget++) - (*bsource++);
        if (n != 0) return (int)n;
    }
    return 0;
}