#include <boot.h>
#include <vesa.h>

int init(void);

int _stdcall main(int magic, BOOT_INFO *mboot, MBHDR *mbhdr) {
    init();
    test(magic, mboot, mbhdr);
    for (;;);
    return 1;
}