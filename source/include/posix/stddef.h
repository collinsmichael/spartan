#ifndef STDDEF_H
#define STDDEF_H

#define elementsof(a)     (sizeof(a) / sizeof(a[0]))
#define align(a,b)        do { a = (((int)a)+((int)b)-1) & ~(((int)b)-1); } while (0)
#define swap(type, a, b)  do { type c = a; a = b; b = c; } while(0)
#define min(a, b)         (((a) < (b)) ? (a) : (b))
#define max(a, b)         (((a) > (b)) ? (a) : (b))

// this structure must match libk.h driver_t
typedef struct XFILE {
    int   inode;
    int   head;
    int   tail;
    char *ring;
} XFILE;
typedef struct FILE {
    int   inode;
    int   head;
    int   tail;
    char *ring;
    int (*xopen )(char *path, char *access);
	int (*xclose)(void *file);
	int (*xsize )(void *file);
	int (*xseek )(void *file, int offset, int method);
	int (*xgetc )(void *file);
	int (*xputc )(int ch, void *file);
    int reserved[6];
} FILE;


#define stdin  ((FILE*)1)
#define stdout ((FILE*)2)
#define stderr ((FILE*)3)
#define EOF (-1)
#define SEEK_SET (0)
#define SEEK_CUR (1)
#define SEEK_END (2)

#endif // STDDEF_H