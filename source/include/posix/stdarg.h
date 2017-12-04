#ifndef STDARG_H
#define STDARG_H

typedef int* va_list;

#define va_start(ap, param) do {ap = (va_list)&param; ap++;} while (0)
#define va_arg(ap, type)    (*((type*)(ap))++)
#define va_end(ap)

#endif // STD_ARG