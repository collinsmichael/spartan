#include <driver.h>
#include <fs.h>
#include <stdarg.h>
#include <stddef.h>
#include <string.h>

_declspec(dllexport)
FILE *fopen(char *path, char *access) {
    if (!path || !access) return 0;
    if (strstr(path, ":/")) return dopen(path, access);
    return (FILE*)xopen(path, access);
}

_declspec(dllexport)
int fclose(FILE *stream) {
    if (!stream || !stream->xclose) return EOF;
    return stream->xclose(stream);
}

_declspec(dllexport)
int fputc(int c, FILE *stream) {
    if (!stream || !stream->xputc) return EOF;
    return stream->xputc(c, stream);
}

_declspec(dllexport)
int fgetc(FILE *stream) {
    if (!stream || !stream->xgetc) return EOF;
    return stream->xgetc(stream);
}