#include <Kernel.h>
#include <Shell.h>
#include <UnitTest.h>

extern IProcess *Process;

bool JumpToRing3(char *path);

void main(void *grub) {
    if (!BootStrap(grub)) for (;;);
    for (;;) JumpToRing3("System/Desktop.dll");
}

int _stdcall start(int magic, void *grub, void *boot) {
    if (magic == 0x2BADB002) main(grub);
    return 1;
}