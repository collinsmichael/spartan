#include <Kernel.h>

extern IFileIO *FileIO;

#ifdef AUTHOR_RAMDISK
extern char *boot;
extern char *RAM;
#else
extern char *boot;
#define RAM ((char*)0)
#endif

bool ILoader_Reloc(char *path, char *base, int size) {
    char *binary = &RAM[(int)base];
    MZHDR *mzhdr = (MZHDR*)binary;
    PEHDR *pehdr = (PEHDR*)&binary[mzhdr->e_lfanew];
    OPHDR *ophdr = (OPHDR*)&binary[mzhdr->e_lfanew + sizeof(PEHDR)];

    PEDIR *dir = (PEDIR*)&binary[mzhdr->e_lfanew + sizeof(PEHDR) + sizeof(OPHDR)];
    PERELOC *reloc = (PERELOC*)&binary[dir->RelocAddress];
    int length = dir->RelocSize;
    if (length <= 0) return true;

    int target = (int)base;
    int source = ophdr->ImageBase;
    if (target == source) return true;
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
    #ifdef AUTHOR_RAMDISK
    if (StrCmp(path, boot) == 0) {
        FileIO.Disk->Entry = ophdr->ImageBase + ophdr->Entry;
    }
    #endif
    return true;
}

static int ILoader_GetProc(char *base, char *name) {
	int ptr = ((MZHDR*)base)->e_lfanew + sizeof(PEHDR) + sizeof(OPHDR);
	PEDIR *dir = (PEDIR*)&base[ptr];
	PEEXPORT *exports = (PEEXPORT*)&base[dir->ExportAddress];
	int *names = (int*)&base[exports->PtrNames];
	int *procs = (int*)&base[exports->PtrFunctions];
	for (u32 count = 0; count < exports->NumFunctions; count++) {
        char *find = name;
        char *ascii = &base[names[count]];
        if (StrCmp(find, ascii) == 0) return procs[count];
	}
    return 0;
}

static CFile *ILoader_OpenLib(char *libname, char *mode) {
    static char path[1024];
    static char *dir[] = { // TODO REPLACE WITH ENVIRONMENT VARIABLES
        "System/",
        "Apps/"
    };
    for (int i = 0; i < elementsof(dir); i++) {
        StrCpy(path, dir[i]);
        StrCat(path, libname);
        CFile *file = FileIO->Open(path, mode);
        if (file) return file;
    }
    return 0;
}

bool ILoader_Link(char *path, char *base, int size) {
    char *binary = &RAM[(int)base];
    MZHDR *mzhdr = (MZHDR*)binary;
    PEHDR *pehdr = (PEHDR*)&binary[mzhdr->e_lfanew];
    OPHDR *ophdr = (OPHDR*)&binary[mzhdr->e_lfanew + sizeof(PEHDR)];

    PEDIR *dir = (PEDIR*)&binary[mzhdr->e_lfanew + sizeof(PEHDR) + sizeof(OPHDR)];
    if (dir->ImportTableSize == 0 || dir->ImportTableSize == 0) return true;

    PEIMPORT *imports = (PEIMPORT*)&binary[dir->ImportAddress];
    while (imports->Name) {
        char *library = &binary[imports->Name];

        static char target[1024];
        CFile *file = ILoader_OpenLib(library, "-rwx");
        if (!file) return false;

        int dll_base = (int)file->Node->FileData;
        char *dll = &RAM[dll_base];
        file->Close(file);

        int *thunk = (int*)&binary[imports->FirstThunk];
        while (*thunk) {
            PEIMPORTNAME *imp = (PEIMPORTNAME*)&binary[*thunk];
            int proc = ILoader_GetProc(dll, imp->Name);
            if (!proc) return false;
            *thunk++ = dll_base + proc;
        }
        imports++;
    }
    return true;
}

u32 ILoader_CalcFootPrint(char *image, CPcb *pcb, int overhead, int alignment) {
    MZHDR *mz = (MZHDR*)image;
    OPHDR *op = (OPHDR*)&image[mz->e_lfanew + sizeof(PEHDR)];

    int extent = op->SizeHeapCommit + op->SizeStackCommit + op->ImageSize + overhead;
    extent = (extent + alignment-1) & ~(alignment-1);
    if (!pcb->Extent) pcb->Extent = extent;
    pcb->Heap    = (op->ImageSize + 4*KB-1) & ~(4*KB-1);
    pcb->Stack   = pcb->Extent - pcb->Stack;
    pcb->Peb     = (CPeb*)(pcb->Extent - (int)pcb->Peb);
    pcb->Pipes   = pcb->Extent - pcb->Pipes;
    pcb->Events  = pcb->Extent - pcb->Events;
    pcb->Windows = pcb->Extent - pcb->Windows;
    pcb->Screen  = pcb->Extent - pcb->Screen;
    pcb->MemMap  = pcb->Extent - pcb->MemMap;
    return pcb->Extent;
}

static void *ILoader_GetEntry(char *path) {
    CFile *file = FileIO->Open(path, "-rwx");
    if (!file) return 0;

    char *image = FileIO->Base(file);
    int size = FileIO->Size(file);
    FileIO->Close(file);

    MZHDR *mz = (MZHDR*)image;
    OPHDR *op = (OPHDR*)&image[mz->e_lfanew + sizeof(PEHDR)];
    return (void*)&image[op->Entry];
}

static ILoader loader = {
    ILoader_Reloc,
    ILoader_Link,
    ILoader_GetEntry,
    ILoader_CalcFootPrint
};

export ILoader *Loader = &loader;
