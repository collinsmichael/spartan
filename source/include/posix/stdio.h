#ifndef STDIO_H
#define STDIO_H
#include <stdarg.h>
#include <stddef.h>

void *fopen(const char *path, const char *mode);
int   fclose(void *file);
int   fgetc(void *file);
int   fputc(int c, void *file);
int   fflush(void *file);
int   fseek(void *file, int offset, int method);
int   ftell(void *file);
int   feof(void *file);

char *fgets(char *buffer, int size, void *file);
int   fprintf(void *file, const char *format, ...);
int   fputs(const char *buffer, void *file);
int   fread(void *buffer, int size, int n, void *file);
int   fwrite(const void *buffer, int size, int n, void *file);

/*
API int   fscanf(void *file, const char *format, ...);

API char *gets(char *s);
API int   printf(const char *format, ...);
API int   puts(const char *s);
API int   scanf(const char *format, ...);

API int   sprintf(char *buffer, const char *format, ...);
API int   sscanf(const char *buffer, const char *format, ...);

int       vfprintf(void *file, const char *format, va_list arglist);
int       vfscanf(void *file, const char *format, va_list arglist);
int       vprintf(const char *format, va_list arglist);
int       vscanf(const char *format, va_list arglist);
int       vsprintf(char *buffer, const char *format, va_list arglist);
int       vsscanf(const char *buffer, const char *format, va_list arglist);

API int   fgetpos(void *file, int *pos);
API void *freopen(const char *path, const char *mode, void *file);
API int   fsetpos(void *file, const int *pos);
API int   rename(const char *oldname, const char *newname);
API void  rewind(void *file);
API void  setbuf(void *file, char *buf);
int       setvbuf(void *file, char *buf, int type, int size);
int       ungetc(int c, void *file);
*/

#endif // STDIO_H