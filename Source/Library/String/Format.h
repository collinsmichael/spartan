#ifndef LIBRARY_STRING_FORMAT_H
#define LIBRARY_STRING_FORMAT_H

typedef int* va_list;
#define va_start(ap, param) do {ap = (va_list*)&param; ap++;} while (0)
#define va_arg(ap, type)    (*((type*)(ap))++)
#define va_end(ap)

char *xputn(char *str, int n, int b);
char *xputu(char *str, u32 n, int b);
bool  VaFormat(char *str, char *fmt, va_list *va);
bool  Format(char *text, char *format, ... );

#endif LIBRARY_STRING_FORMAT_H