#define _CRT_SECURE_NO_WARNINGS
#include <memory.h>
#include <stdio.h>
#include <windows.h>
#include <inode.h>

int stats(void);

// set up 15MB of disk space starting at physical address 1MB
extern char *image;
extern volume_t *volume;
extern block_t  *blocks;
extern inode_t  *inodes;

int  start;
int  count;
char text[4096][256];
char wildcard[512];

int scan_win32(char *home, char *root) {
    WIN32_FIND_DATAA find;
    sprintf(wildcard, "%s/%s/*", home, root);
    void *hFind = FindFirstFileA(wildcard, &find);
    if (hFind == INVALID_HANDLE_VALUE) return 0;
    do {
        if (find.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
            text[count][0] = 1;
            strcpy(&text[count++][1], find.cFileName);
        }
    } while (FindNextFileA(hFind, &find));
    FindClose(hFind);

    hFind = FindFirstFileA(wildcard, &find);
    if (hFind == INVALID_HANDLE_VALUE) return 0;
    do {
        if (find.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
            continue;
        }
        text[count][0] = 2;
        strcpy(&text[count++][1], find.cFileName);
    } while (FindNextFileA(hFind, &find));
    FindClose(hFind);
    return 1;
}

int dir_bfs(char *home, char *root) {
    int head = count;
    scan_win32(home, root);
    int tail = count;

    char path[512];
    for (int i = head; i < tail; i++) {
        if (text[i][0] != 1) continue;
        if (strcmp(".", &text[i][1]) == 0) continue;
        if (strcmp("..", &text[i][1]) == 0) continue;
        char *fname = &text[i][1];
        sprintf(path, "%s%s", root, fname);
        if (!mkdir(path)) {
            printf(" mkfs [fail] error creating directory '%s'\n", path);
            return 0;
        }
    }

    for (; head < tail; head++) {
        if (text[head][0] != 1) continue;
        if (strcmp(".", &text[head][1]) == 0) continue;
        if (strcmp("..", &text[head][1]) == 0) continue;
        char *fname = &text[head][1];
        sprintf(path, "%s%s/", root, fname);
        if (!dir_bfs(home, path)) return 0;
    }
    return 1;
}

int bin_bfs(char *home, char *root) {
    int head = count;
    scan_win32(home, root);
    int tail = count;
    char path[512];

    for (int i = head; i < tail; i++) {
        if (text[i][0] != 2) continue;
        if (strcmp(".", &text[i][1]) == 0) continue;
        if (strcmp("..", &text[i][1]) == 0) continue;
        char *fname = &text[i][1];
        sprintf(path, "%s%s", root, fname);

        FILE *ifile = fopen(path, "rb");
        if (!ifile) return 0 & printf(" mkfs [fail] hdd '%s'\n", fname);
        XFILE *ofile = xopen(path, "wb");
        if (!ofile) return 0 & printf(" mkfs [fail] ramdisk '%s'\n", path);

        while (!feof(ifile)) {
            int c = fgetc(ifile);
            if (c < 0) break;
            xputc(c, ofile);
        }
        fclose(ifile);
        xclose(ofile);
    }

    for (; head < tail; head++) {
        if (text[head][0] != 1) continue;
        if (strcmp(".", &text[head][1]) == 0) continue;
        if (strcmp("..", &text[head][1]) == 0) continue;
        char *fname = &text[head][1];
        sprintf(path, "%s%s/", root, fname);
        if (!bin_bfs(home, path)) return 0;
    }
    return 1;
}

int mkroot(char *img) {
    fnode_t root;
    memset(&root, 0, sizeof(root));
    root.length = sizeof(root);
    root.inode = mknode(2*root.length, 0x8000, 0x00, 0x00);
    inode_t *inode = &inodes[root.inode];
    inode->size = 0;

    // generate root directory records
    memset(&img[inode->base], 0, PAGE_SIZE);
    root.name[0] = '.';
    memcpy(&img[inode->base], &root, sizeof(root));
    inode->size += sizeof(root);
    root.name[1] = '.';
    memcpy(&img[inode->base+inode->size], &root, sizeof(root));
    inode->size += sizeof(root);
    return 0;
}

int mkvol(char *root, char *img, int len) {
    start = strlen(root)+1;
    mkroot(image);
    dir_bfs(root, "");
    bin_bfs(root, "");
    //stats();
    return len;
}