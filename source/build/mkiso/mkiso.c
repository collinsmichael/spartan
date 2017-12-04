//BOOT.BIN (LBA + LEN)
//#         LBA LBA LBA LBA LBA LBA LEN LEN LEN LEN LEN LEN
//0000A930: D0 15 00 00 00 00 15 D0 24 13 00 00 00 00 13 24
//
//
//DISK.IMG (LBA + LEN)
//#         LBA LBA LBA LBA LBA LBA LEN LEN LEN LEN LEN LEN
//0000A9A8: D0 15 00 00 00 00 15 D0 24 13 00 00 00 00 13 24
//
//
//PrimaryVolumeDescriptor.TotalSectors
//#         NUM NUM NUM NUM NUM NUM
//00008050: D5 15 00 00 00 00 15 D5
#include <stdio.h>

int main(int argc, char *argv[]) {
    printf(" ==============================================================\n");
    printf(" mkiso - version 1.0 - Michael Collins (2017)                  \n");
    printf(" ==============================================================\n");
    if (argc < 5) return printf(" usage: mkiso in.iso boot.bin initrd.img out.iso\n");

    char *blank = argv[1];
    char *boot  = argv[2];
    char *disk  = argv[3];
    char *out   = argv[4];
    printf(" mkiso: input  = %s\n", blank);
    printf(" mkiso: boot   = %s\n", boot);
    printf(" mkiso: disk   = %s\n", disk);
    printf(" mkiso: output = %s\n", out);

    FILE *cfile = fopen(blank, "rb");
    if (!cfile) return printf(" mkiso: file not found! %s\n", blank);
    FILE *afile = fopen(boot, "rb");
    if (!afile) return printf(" mkiso: file not found! %s\n", boot);
    FILE *bfile = fopen(disk, "rb");
    if (!bfile) return printf(" mkiso: file not found! %s\n", disk);
    FILE *file = fopen(out, "wb");
    if (!file) return printf(" mkiso: cannot create file! %s\n", out);

    fseek(cfile, 0, SEEK_END);
    int clen = ftell(cfile);
    fseek(cfile, 0, SEEK_SET);
    char *iso = (char*)malloc(clen);
    if (!iso) return printf("mkiso: out of memory for file! %s\n", blank);
    fseek(cfile, 0, SEEK_SET);
    fread(iso, 1, clen, cfile);

    fseek(afile, 0, SEEK_END);
    int alen = ftell(afile);
    int alba = clen / 2048;
    int apad = (alen + 4095) & (~4095);
    char *dir = &iso[0xA930];
    dir[0x07] = dir[0x00] = (alba    ) & 255;
    dir[0x06] = dir[0x01] = (alba>> 8) & 255;
    dir[0x05] = dir[0x02] = (alba>>16) & 255;
    dir[0x04] = dir[0x03] = (alba>>24) & 255;

    dir[0x0F] = dir[0x08] = (apad    ) & 255;
    dir[0x0E] = dir[0x09] = (apad>> 8) & 255;
    dir[0x0D] = dir[0x0A] = (apad>>16) & 255;
    dir[0x0C] = dir[0x0B] = (apad>>24) & 255;

    fseek(bfile, 0, SEEK_END);
    int blen = ftell(bfile);
    int blba = (clen + apad) / 2048;
    int bpad = (blen + 4095) & (~4095);
    dir = &iso[0xA9A8];
    dir[0x07] = dir[0x00] = (blba    ) & 255;
    dir[0x06] = dir[0x01] = (blba>> 8) & 255;
    dir[0x05] = dir[0x02] = (blba>>16) & 255;
    dir[0x04] = dir[0x03] = (blba>>24) & 255;

    dir[0x0F] = dir[0x08] = (bpad    ) & 255;
    dir[0x0E] = dir[0x09] = (bpad>> 8) & 255;
    dir[0x0D] = dir[0x0A] = (bpad>>16) & 255;
    dir[0x0C] = dir[0x0B] = (bpad>>24) & 255;

    int clba = (clen+apad+bpad) / 2048;
    char *pvd = &iso[0x8050];
    pvd[0x07] = pvd[0x00] = (clba    ) & 255;
    pvd[0x06] = pvd[0x01] = (clba>> 8) & 255;
    pvd[0x05] = pvd[0x02] = (clba>>16) & 255;
    pvd[0x04] = pvd[0x03] = (clba>>24) & 255;

    fseek(file, 0, SEEK_SET);
    fwrite(iso, 1, clen, file);
    fflush(file);

    int base = 0x00100000;
    int ptr = base + apad;
    fseek(afile, 0, SEEK_SET);
    for (int a = 0; a < alen; a++) {
        if (a == 0x14 || a == 0x18 || a == 0x3C) {
            fgetc(afile); fputc((ptr    ) & 255, file);
            fgetc(afile); fputc((ptr>> 8) & 255, file);
            fgetc(afile); fputc((ptr>>16) & 255, file);
            fgetc(afile); fputc((ptr>>24) & 255, file);
            if (a == 0x14) a = 0x17;
            if (a == 0x18) a = 0x1B;
            if (a == 0x3C) a = 0x3F;
        } else fputc(fgetc(afile), file);
    }
    for (int a = alen; a < apad; a++) fputc(0, file);
    fflush(file);

    fseek(bfile, 0, SEEK_SET);
    for (int b = 0;    b < blen; b++) fputc(fgetc(bfile), file);
    for (int b = blen; b < bpad; b++) fputc(0, file);
    fflush(file);

    free(iso);
    fflush(file);
    fclose(file);
    fclose(afile);
    fclose(bfile);
    fclose(cfile);


    printf(" mkiso: boot lba = %08X (%d)\n", alba, alba);
    printf(" mkiso: boot len = %08X (%d) of %08X (%d)\n", alen, alen, apad, apad);
    printf(" mkiso: disk ptr = %08X (%d)\n", base, base);
    printf(" mkiso: disk lba = %08X (%d)\n", blba, blba);
    printf(" mkiso: disk len = %08X (%d) of %08X (%d)\n", blen, blen, bpad, bpad);
    printf(" mkiso: disk ptr = %08X (%d)\n", ptr, ptr);
    printf(" mkiso: success!\n\n");
    return 0;
}