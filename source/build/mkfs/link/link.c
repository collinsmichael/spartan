#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <inode.h>

extern char *image;
extern volume_t *volume;
extern block_t  *blocks;
extern inode_t  *inodes;

char *boot;
void init_image(void);
int relocate(void);
int linker(void);

void banner(void) {
    printf(" ============================================================== \n");
    printf(" linker - version 1.0 - Michael Collins (2017)                  \n");
    printf(" ============================================================== \n");
}

int main(int argc, char *argv[]) {
    if (argc < 3) return printf(" usage: linker ramdisk.img boot.exe\n");
    banner();
    boot = argv[2];

    init_image();

    FILE *imgfile = fopen(argv[1], "rb");
    if (!imgfile) return printf(" link [fail] file not found! '%s'\n", argv[1]);
    fseek(imgfile, 0, SEEK_END);
    int imgsize = ftell(imgfile);
    fseek(imgfile, 0, SEEK_SET);
    fread(volume, 1, imgsize, imgfile);
    fclose(imgfile);

    if (volume->linker != 1) relocate();
    volume->linker = 1;
    linker();

    imgfile = fopen(argv[1], "wb");
    if (!imgfile) return printf(" link [fail] cannot overwrite! '%s'\n", argv[1]);
    fwrite(volume, 1, imgsize, imgfile);
    fflush(imgfile);
    fclose(imgfile);
    return 0;
}