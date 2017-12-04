#ifndef FS_H
#define FS_H
#include <inode.h>
#include <stddef.h>
#include <stdint.h>
#pragma pack(push, 1)

XFILE *xopen(char *path, char *access);
int    xclose(XFILE *file);
int    xsize(XFILE *file);
int    xseek(XFILE *file, int offset, int method);
int    xgetc(XFILE *file);
int    xputc(int ch, XFILE *file);

int  dalloc(int size);
void dfree(int addr, int size);
int  mknode(int size, int flags, int uid, int gid);
int  delnode(int inode);
int  renode(int inode, int size);
int  defrag(void);
int  mkflags(char *access);
int  user(void);
int  group(void);
int  chmod(char *path, char *access);
int  mkbin(char *path);
int  mkdir(char *path);
int  rmbin(char *path);
int  rmdir(char *path);
int  ffind(char *path, char *buf, int len);
int  fnext(int inode, char *buf, int len);

#pragma pack(pop)
#endif // FS_H