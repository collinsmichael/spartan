/* ************************************************************************** */
/* source/stdlib/memory.c                                                     */
/* string runtime support functions                                           */
/* ************************************************************************** */
#include <libk.h>

char lower[0x100] =
"................................ !\"#$%&'()*+,-./0123456789:;<=>?"
"@abcdefghijklmnopqrstuvwxyz[\\]^_`abcdefghijklmnopqrstuvwxyz{|}~."
"€ ‚ƒ„…†‡ˆ‰Š‹Œ   ‘’“”•–-.™š›œ.Ÿ ¡¢£¤¥¦§¨©ª«¬.®¯°±²³´µ¶·¸¹º»¼½¾¿"
"ÀÁÂÃÄÅÆÇÈÉÊËÌÍÎÏĞÑÒÓÔÕÖ×ØÙÚÛÜİŞßàáâãäåæçèéêëìíîïğñòóôõö÷øùúûüışÿ";

char upper[0x100] =
"................................ !\"#$%&'()*+,-./0123456789:;<=>?"
"@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\\]^_`ABCDEFGHIJKLMNOPQRSTUVWXYZ{|}~."
"€ ‚ƒ„…†‡ˆ‰Š‹Œ   ‘’“”•–-.™š›œ.Ÿ ¡¢£¤¥¦§¨©ª«¬.®¯°±²³´µ¶·¸¹º»¼½¾¿"
"ÀÁÂÃÄÅÆÇÈÉÊËÌÍÎÏĞÑÒÓÔÕÖ×ØÙÚÛÜİŞßàáâãäåæçèéêëìíîïğñòóôõö÷øùúûüışÿ";

_declspec(dllexport)
char tolower(char c) {
    return lower[c];
}

_declspec(dllexport)
char toupper(char c) {
    return upper[c];
}

_declspec(dllexport)
int strlen(const char *text) {
    if (!text) return 0;
    int i = 0;
    do {
        i++;
    } while (text[i]);
    return i;
}

_declspec(dllexport)
char *strcat(char *target, const char *source) {
    if (!target || !source) return 0;
    while (*target) target++;
    while (*source) *target++ = *source++;
    *target = 0;
    return target;
}

_declspec(dllexport)
char *strchr(const char *text, int c) {
    if (!text) return 0;
    while (*text) {
        if (*text == c) return (char*)text;
        else text++;
    }
    return 0;
}

_declspec(dllexport)
int strcmp(const char *target, const char *source) {
    if (!target || !source) return 0;
    for (;;) {
        char a = *target++;
        const char b = *source++;
        if (a < b) return -1;
        if (a > b) return 1;
        if (a == 0) return 0;
    }
}

_declspec(dllexport)
char *strcpy(char *target, const char *source) {
    if (!target || !source) return 0;
    char *trg = target;
    const char *src = source;
    while (*src) *trg++ = *src++;
    *trg = 0;
    return target;
}

_declspec(dllexport)
char *strdup(const char *text) {
    if (!text) return 0;
    char *data = kalloc(strlen(text)+1);
    if (data) strcpy(data, text);
    return data;
}

_declspec(dllexport)
int stricmp(const char *target, const char *source) {
    if (!target || !source) return 0;
    for (;;) {
        char a = lower[(unsigned)*target++];
        char b = lower[(unsigned)*source++];
        if (a < b) return -1;
        if (a > b) return 1;
        if (a == 0) return 0;
    }
}

_declspec(dllexport)
char *strlwr(char *text) {
    if (!text) return 0;
    while (*text) *text++ = lower[(unsigned)*text];
    return text;
}

_declspec(dllexport)
char *strupr(char *text) {
    if (!text) return 0;
    while (*text) *text++ = upper[(unsigned)*text];
    return text;
}

_declspec(dllexport)
char *strncat(char *target, const char *source, int size) {
    if (!target || !source) return 0;
    while (size && *target) {
        target++;
        size--;
    }
    while (size-- && *source) *target++ = *source++;
    if (size == 0) return 0;
    *target = 0;
    return target;
}

_declspec(dllexport)
int strncmp(const char *target, const char *source, int size) {
    if (!target || !source) return 0;
    while (size--) {
        char a = *target++;
        char b = *source++;
        if (a < b) return -1;
        if (a > b) return 1;
        if (a == 0) break;
    }
    return 0;
}

_declspec(dllexport)
char *strncpy(char *target, const char *source, int size) {
    if (!target || !source) return 0;
    while (size--) {
        char c = *source++;
        *target++ = c;
        if (!c) break;
    }
    return target;
}

_declspec(dllexport)
int strnicmp(const char *target, const char *source, int size) {
    if (!target || !source) return 0;
    while (size--) {
        unsigned char a = *target++;
        unsigned char b = *source++;
        a = lower[a];
        b = lower[b];
        if (a < b) return -1;
        if (a > b) return 1;
        if (a == 0) break;
    }
    return 0;
}

_declspec(dllexport)
int strnlen(const char *text, int size) {
    if (!text) return 0;
    int i = 0;
    while (++i <= size) if (!text[i]) return i;
    return 0;
}

_declspec(dllexport)
char *strstr(const char *target, const char *source) {
    if (!target || !source) return 0;
    while (*target) {
        const char *a = target;
        const char *b = source;
        while (*a && *b) {
            if (*a != *b) break;
            a++;
            b++;
        }
        if (*b == 0) return (char*)target;
        target++;
    }
    return 0;
}