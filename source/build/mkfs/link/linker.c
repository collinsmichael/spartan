#define _CRT_SECURE_NO_WARNINGS
#include <memory.h>
#include <stdio.h>
#include <string.h>
#include <inode.h>
#include <exe.h>

extern char *image;
extern volume_t *volume;
extern block_t  *blocks;
extern inode_t  *inodes;

int get_proc(char *base, char *name) {
	int ptr = ((mzhdr_t*)base)->e_lfanew + sizeof(pehdr_t) + sizeof(ophdr_t);
	pedir_t *dir = (pedir_t *)&base[ptr];
	peexport_t *exports = (peexport_t*)&base[dir->export_address];
	int *names = (int*)&base[exports->ptr_names];
	int *procs = (int*)&base[exports->ptr_functions];
	for (uint32_t count = 0; count < exports->num_functions; count++) {
        char *find = name;
        char *ascii = &base[names[count]];
        if (strcmp(find, ascii) == 0) return procs[count];
	}
    return 0;
}

XFILE *openlib(char *libname, char *mode) {
    static char path[1024];
    static char *dir[] = { // TODO REPLACE WITH ENVIRONMENT VARIABLES
        "sys/",
        "dev/",
        "bin/"
    };
    for (int i = 0; i < elementsof(dir); i++) {
        strcpy(path, dir[i]);
        strcat(path, libname);
        XFILE *file = xopen(path, mode);
        if (file) return file;
    }
    return 0;
}

int link(char *path, int base, int size) {
    //printf("[linker] %s\n", path);

    char *binary = &image[base];
    mzhdr_t *mzhdr = (mzhdr_t*)binary;
    pehdr_t *pehdr = (pehdr_t*)&binary[mzhdr->e_lfanew];
    ophdr_t *ophdr = (ophdr_t*)&binary[mzhdr->e_lfanew + sizeof(pehdr_t)];

    pedir_t *dir = (pedir_t*)&binary[mzhdr->e_lfanew + sizeof(pehdr_t) + sizeof(ophdr_t)];
    if (dir->import_table_size == 0 || dir->import_table_size == 0) return 1;

    peimport_t *imports = (peimport_t*)&binary[dir->import_address];
    while (imports->name) {
        char *library = &binary[imports->name];

        /*
        static char sys[1024];
        static char app[1024];
        sprintf(sys, "sys/%s", library);
        sprintf(app, "bin/%s", library);
        char *target = sys;
        XFILE *file = xopen(sys, "rb");
        if (!file) file = xopen(target = app, "rb");
        */
        static char target[1024];
        XFILE *file = openlib(library, "rb");
        if (!file) return 0 & printf("error file not found! '%s'\n", library);
        int dll_base = inodes[file->inode].base;
        char *dll = &image[dll_base];
        xclose(file);

        uint32_t *thunk = (uint32_t*)&binary[imports->first_thunk];
        while (*thunk) {
            peimportname_t *import = (peimportname_t*)&binary[*thunk];
            int proc = get_proc(dll, import->name);
            if (!proc) return 0 & printf("error dll %s does not contain function %s\n", library, import->name);

            uint32_t func = (uint32_t)(dll_base + proc);
            *thunk++ = (uint32_t)func;
            //printf("(%s) <- %s:%s\n", path, target, import->name);
        }
        imports++;
    }
    return 1;
}

int _linker(char *root) {
    char fname[256];
    char path[512];
    memset(fname, 0, sizeof(fname));
    int parent = ffind(root, fname, sizeof(fname));
    if (parent < 0) return 0;
    strcpy(fname, "..");
    for (;;) {
        int child = fnext(parent, fname, sizeof(fname));
        if (child <= 0) return 0;
        memset(path, 0, sizeof(path));
        sprintf(path, "%s/%s", root, fname);

        inode_t *inode = &inodes[child];
        if (inode->flags & DIR_MASK) {
            _linker(path);
        } else if (strstr(fname, ".exe") || strstr(fname, ".dll")) {
            link(path, inode->base, inode->size);
        }
    }
    return 1;
}

int linker(void) {
    char root[256];
    memset(root, 0, sizeof(root));
    int parent = ffind("", root, sizeof(root));
    if (parent < 0) return 0;
    strcpy(root, "..");
    for (;;) {
        int child = fnext(parent, root, sizeof(root));
        if (child <= 0) break;
        inode_t *inode = &inodes[child];
        if (inode->flags & DIR_MASK) _linker(root);
    }
    return 1;
}