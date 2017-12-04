#include <libk.h>
#include <stddef.h>
#include <stdio.h>
#include <string.h>

driver_t *rd(void);  // ramdisk
driver_t *kb(void);  // keyboard
driver_t *cga(void); // cga
driver_t *tty(void); // terminal


_declspec(dllexport)
int driver_init(void) {
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