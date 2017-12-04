#define _CRT_SECURE_NO_WARNINGS
#include <memory.h>
#include <stdio.h>
#include <string.h>
#include <inode.h>

typedef struct fnew_t {
    int length; // length of this fnode record (always a multiple of 16)
    int inode;  // inode number
    char name[248];  // actual length is len-5
} fnew_t;

int mkflags(char *access) {
    int mask = 0;
    while (*access) {
        mask = mask + mask;
        if (*access++ != '-') mask++;
    }
    return mask;
}

int user(void) {
    return 0x00;
}

int group(void) {
    return 0x00;
}

int chmod(char *path, char *access) {
    return 0;
}