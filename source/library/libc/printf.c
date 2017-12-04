/* ========================================================================= */
/* File Name : source/library/printf.c                                       */
/* File Date : 3 February 2015                                               */
/* Author(s) : Michael Collins                                               */
/* ------------------------------------------------------------------------- */
/* basic printing functionality. kfprintf supports %c %s %d %l %x and uses   */
/* stardard io to direct its output to the target device.                    */
/* ========================================================================= */
#include <driver.h>
#include <fs.h>
#include <stddef.h>
#include <inode.h>

/* ------------------------------------------------------------------------- */
/* prints a number Dennis Ritchie style supports bases from 2 ... 16         */
/* ------------------------------------------------------------------------- */
void xputn(FILE *file, uint32_t n, uint32_t b) {
	static char t[] = "0123456789ABCDEF";
	register int a;

	if (!file) return;
	if (b && b < sizeof(t)) {
		if (a = n/b) xputn(file, a, b);
		file->xputc(t[n%b], file);
	}
}

/* ------------------------------------------------------------------------- */
/* formatted printing. supports %s %c %l %d %x                               */
/* ------------------------------------------------------------------------- */
_declspec(dllexport)
int fprintf(FILE *file, char *fmt, ...) {
	register char  c;
	register char *s;
	register int  *x = (int*)&fmt;

	if (!file) return 0;
    switch ((int)file) {
    case stdin:  file = dopen("stdin", "wb"); break;
	case stdout: file = dopen("stdin", "wb"); break;
	case stderr: file = dopen("stdin", "wb"); break;
    }

	x++; /* now x is pointing at the variable args list */
	for (;;) {
		/* unformatted text */
		while ((c = *fmt++) != '%') {
			if (c == '\0') return 1;
			else file->xputc(c, file);
		}

		/* formatting */
		c = *fmt++;
		if (c >= 'A' && c <= 'Z') c = c - 'A' + 'a';
		if (c == 'd' || c == 'l') xputn(file, *x++, 10);
		else if (c == '%') file->xputc(c, file);
		else if (c == 'x') xputn(file, *x++, 16);
		else if (c == 'o') xputn(file, *x++, 8);
		else if (c == 's') for (s = (char*)*x++; c = *s++; file->xputc(c, file));
		else if (c == 'c') file->xputc(*x++, file);
	}
}