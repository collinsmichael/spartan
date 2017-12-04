#ifndef MKFS_H
#define MKFS_H

int mkvol(char *root, char *img, int len);
int relocate(void);
int linker(void);
int fsstart(char *ram, int disk);

#endif // MKFS_H