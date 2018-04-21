#include <Library.h>
#include <Kernel\BootLoad\Intel\Intel.h>

export int Logger(char *format, ... ) {
    if (!format) return 0;
    char text[0x400];

    va_list *list;
    va_start(list, format);
    VaFormat(text, format, list);
    va_end(list);

    for (char *c = text; *c; c++) {
        outb(0xE9, *c);
    }
    return 1;
}

export int Debug(char *format, ... ) {
    if (!format) return 0;
    char text[0x400];

    va_list *list;
    va_start(list, format);
    VaFormat(text, format, list);
    va_end(list);

    for (char *c = text; *c; c++) {
        outb(0xE9, *c);
    }
    return 1;
}