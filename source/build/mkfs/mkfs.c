#define _CRT_SECURE_NO_WARNINGS
#include <malloc.h>
#include <stdio.h>
#include <windows.h>
#include <inode.h>
#include "mkfs.h"
#include "lzss\lzss.h"

void init_image(void);

int encode(char *decoded, int dsize, char *encoded, int esize, uint8_t EI, uint8_t EJ, uint8_t P);
int imagebase;
char fname[1024];
char payload[16*MB];
extern char *image;
extern volume_t *volume;
extern block_t  *blocks;
extern inode_t  *inodes;
char *boot;

void banner(void) {
    printf(" ============================================================== \n");
    printf(" mkfs - version 1.0 - Michael Collins (2017)                    \n");
    printf(" ============================================================== \n");
}

int usage(int argc, char *argv[]) {
    if (argc == 8) return 1;
    printf(" usage: mkfs base path/to/root disk.img boot.exe EI EJ P\n");
    return 0;
}

char *fload(char *path) {
    FILE *file = fopen(path, "wb");
    if (!file) return 0;
    fseek(file, 0, SEEK_END);
    int size = ftell(file);
    fseek(file, 0, SEEK_SET);
    char *data = malloc(size+1);
    if (data) {
        fread(data, 1, size, file);
        data[size] = 0;
    }
    fclose(file);
    return data;
}

int main(int argc, char *argv[]) {
    if (!usage(argc, argv)) return 0;
    banner();

    imagebase = strtol(argv[1],0,0);
    char *root = argv[2];
    char *disk = argv[3];
    boot = argv[4];
    uint8_t EI = (uint8_t)strtol(argv[5], 0, 0);
    uint8_t EJ = (uint8_t)strtol(argv[6], 0, 0);
    uint8_t P  = (uint8_t)strtol(argv[7], 0, 0);
    init_image();

    if (!SetCurrentDirectoryA(root)) {
        return printf(" mkfs [fail] directory not found! '%s'\n", root);
    }

    int size = mkvol(root, image, sizeof(image));
    if (!size) return 0;

    relocate();
    linker();
    volume->linker = 0x00010000; // version 1.0

    sprintf(fname, "%s.lz", disk);
    FILE *lz = fopen(fname, "wb");
    FILE *file = fopen(disk, "wb");

    for (;;) {
        char *win  = strchr(disk, '\\');
        char *unix = strchr(disk, '/');
        if (win) disk = ++win;
        else if (unix) disk = ++unix;
        else break;
    }

    if (!file) return printf(" mkfs [fail] cannot create file! '%s'\n", disk);
    if (!lz) return printf(" mkfs [fail] cannot create file! '%s.lz'\n", disk);

    int raw = FIRST + volume->disk_used - imagebase;
    int crunched = encode(&image[imagebase], raw, payload, sizeof(payload), EI, EJ, P);

    fwrite(&image[imagebase], 1, raw, file);
    fflush(file);
    fclose(file);

    lzhdr_t lzhdr;
    memset(&lzhdr, 0, sizeof(lzhdr));
    lzhdr.magic  = 'SSZL';
    lzhdr.ecount = crunched;
    lzhdr.dcount = raw;
    lzhdr.EI     = EI;
    lzhdr.EJ     = EJ;
    lzhdr.P      = P;

    fwrite(&lzhdr, 1, sizeof(lzhdr_t), lz);
    fflush(lz);
    fwrite(payload, 1, crunched, lz);
    fflush(lz);
    fclose(lz);

    printf(" mkfs [done] %s    (%d bytes)\n", disk, raw);
    printf(" mkfs [lzss] %s.lz (%d bytes)\n", disk, crunched);
    printf("\n");
    printf(" mkfs [stat] image base = 0x%08X (%d MB)\n",   imagebase, imagebase/MB);
    printf(" mkfs [stat] nodes      = %9d (of %d)\n", volume->node_count, volume->max_nodes);
    printf(" mkfs [stat] free       = %9d bytes\n",   volume->disk_free);
    printf(" mkfs [stat] used       = %9d bytes\n",   volume->disk_used);
    printf(" mkfs [stat] system     = %9d bytes\n",   SYS_NODES);
    printf(" mkfs [stat] total      = %9d bytes\n",   raw);
    printf(" mkfs [stat] compressed = %9d bytes\n",   crunched);
    printf(" mkfs [stat] ratio      = %9.2f %%\n",    crunched*100.0f/max(raw, 1));
    printf("\n");
    return 0;
}