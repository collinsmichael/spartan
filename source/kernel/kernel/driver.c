#include <boot.h>
#include <exe.h>
#include <fs.h>
#include <libk.h>
#include <multiboot.h>
#include <stddef.h>
#include <stdio.h>
#include <string.h>

driver_t *rd(void);  // ramdisk
driver_t *kb(void);  // keyboard
driver_t *cga(void); // cga
driver_t *tty(void); // terminal


int driver_init(int magic, multiboot_info *mboot, MBHDR *mbhdr) {
    char fname[256];
    int dir = ffind("/dev/..", fname, sizeof(fname));
    if (dir < 0) return 0;
    for (;;) {
        int fd = fnext(dir, fname, sizeof(fname));
        if (fd <= 0) break;
        for (int i = 0; fname[i]; fname[i] = tolower(fname[i]));
        if (strstr(fname, ".dll") != 0) continue;
        MZHDR *mzhdr = (MZHDR*)fblock(fd);
    }

/*
    devices[0] = rd();
    devices[(int)stdin]  = kb();
    devices[(int)stdout] = cga();
    devices[(int)stderr] = tty();
    for (int dev = 0; dev <= (int)stderr; dev++) {
        if (!devices[dev]->xinit) continue;
        devices[dev]->xinit(devices[dev]->path, devices[dev]->access);
    }
    count = (int)stderr+1;
*/
    return 1;
}