#include <Library.h>
#include "ExeLoader.h"

extern IFileIO FileIO;
extern char *boot;
extern char *RAM;

int Reloc(char *path, int base, int size) {
    char *binary = &RAM[base];
    MZHDR *mzhdr = (MZHDR*)binary;
    PEHDR *pehdr = (PEHDR*)&binary[mzhdr->e_lfanew];
    OPHDR *ophdr = (OPHDR*)&binary[mzhdr->e_lfanew + sizeof(PEHDR)];

    PEDIR *dir = (PEDIR*)&binary[mzhdr->e_lfanew + sizeof(PEHDR) + sizeof(OPHDR)];
    PERELOC *reloc = (PERELOC*)&binary[dir->RelocAddress];
    int length = dir->RelocSize;
    if (length <= 0) return 1;

    int target = (int)base;
    int source = ophdr->ImageBase;
    if (target == source) return 1;
    int delta = target - source;

    while (length > 0) {
        char *start = (char*)reloc;
        int   count = (reloc->BlockSize-sizeof(PERELOC))/sizeof(u16);
        int   page = reloc->Address;
        u16 *list = (u16*)&start[sizeof(PERELOC)];
        while (count) {
            int *fixup;
            count--;
            u16 type = (list[0] & 0xF000) >> 12;
            u16 offset = list[0] & 0x0FFF;
            u32 rva = page + offset;
            if (type == 3) {
                fixup = (int*)&binary[rva];
                *fixup += delta;
            }
            list++;
        }
        length -= reloc->BlockSize;
        reloc = (PERELOC*)&start[reloc->BlockSize];
    }

    ophdr->ImageBase = target;
    if (StrCmp(path, boot) == 0) {
        FileIO.Disk->Entry = ophdr->ImageBase + ophdr->Entry;
    }
    return 1;
}

int GetProc(char *base, char *name) {
	int ptr = ((MZHDR*)base)->e_lfanew + sizeof(PEHDR) + sizeof(OPHDR);
	PEDIR *dir = (PEDIR*)&base[ptr];
	PEEXPORT *exports = (PEEXPORT*)&base[dir->ExportAddress];
	int *names = (int*)&base[exports->PtrNames];
	int *procs = (int*)&base[exports->PtrFunctions];
	for (u32 count = 0; count < exports->NumFunctions; count++) {
        char *find = name;
        char *ascii = &base[names[count]];
        if (strcmp(find, ascii) == 0) return procs[count];
	}
    return 0;
}

CFile *OpenLib(char *libname, char *mode) {
    static char path[1024];
    static char *dir[] = { // TODO REPLACE WITH ENVIRONMENT VARIABLES
        "System/",
        "Apps/"
    };
    for (int i = 0; i < elementsof(dir); i++) {
        StrCpy(path, dir[i]);
        StrCat(path, libname);
        CFile *file = FileIO.Open(path, mode);
        if (file) return file;
    }
    return 0;
}

int Link(char *path, int base, int size) {
    char *binary = &RAM[base];
    MZHDR *mzhdr = (MZHDR*)binary;
    PEHDR *pehdr = (PEHDR*)&binary[mzhdr->e_lfanew];
    OPHDR *ophdr = (OPHDR*)&binary[mzhdr->e_lfanew + sizeof(PEHDR)];

    PEDIR *dir = (PEDIR*)&binary[mzhdr->e_lfanew + sizeof(PEHDR) + sizeof(OPHDR)];
    if (dir->ImportTableSize == 0 || dir->ImportTableSize == 0) return 1;

    PEIMPORT *imports = (PEIMPORT*)&binary[dir->ImportAddress];
    while (imports->Name) {
        char *library = &binary[imports->Name];

        static char target[1024];
        CFile *file = OpenLib(library, "---x");
#ifdef MKRD
        if (!file) {
            printf(" mkrd [fail] DLL not found! %s required by '%s'\n", library, path);
            exit(-1);
        }
#else
        if (!file) return 0;
#endif

        int dll_base = (int)file->Node->FileData;
        char *dll = &RAM[dll_base];
        file->Close(file);

        int *thunk = (int*)&binary[imports->FirstThunk];
        while (*thunk) {
            PEIMPORTNAME *imports = (PEIMPORTNAME*)&binary[*thunk];
            int proc = GetProc(dll, imports->Name);
#ifdef MKRD
            if (!proc) {
                printf(" mkrd [fail] %s does not contain function %s\n", library, import->Name);
                exit(-1);
            }
#else
            if (!proc) return 0;
#endif
            *thunk++ = dll_base + proc;
        }
        imports++;
    }
    return 1;
}

struct IExeLoader ExeLoader = {
    Reloc,
    Link
};