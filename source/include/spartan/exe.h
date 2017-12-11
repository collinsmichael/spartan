#ifndef EXE_H
#define EXE_H
#include <stdint.h>

#pragma pack(push, 1)
typedef struct MZHDR {
    uint16_t e_magic;
    uint16_t e_cblp;
    uint16_t e_cp;
    uint16_t e_crlc;
    uint16_t e_cparhdr;
    uint16_t e_minalloc;
    uint16_t e_maxalloc;
    uint16_t e_ss;
    uint16_t e_sp;
    uint16_t e_csum;
    uint16_t e_ip;
    uint16_t e_cs;
    uint16_t e_lsarlc;
    uint16_t e_ovno;
    uint16_t e_res[4];
    uint16_t e_oemid;
    uint16_t e_oeminfo;
    uint16_t e_res2[10];
    uint32_t e_lfanew;
} MZHDR;
#pragma pack(pop)

#pragma pack(push, 1)
typedef struct PEHDR {
	uint32_t pesig;
    uint16_t machine;
    uint16_t sections;
    uint32_t timestamp;
    uint32_t ptr_symbols;
    uint32_t num_symbols;
    uint16_t size_ophdr;
    uint16_t characteristics;
} PEHDR;
#pragma pack(pop)

#pragma pack(push, 1)
typedef struct OPHDR {
    uint16_t magic;
    uint8_t  major_linker;
    uint8_t  minor_linker;
    uint32_t size_code;
    uint32_t size_initialized;
    uint32_t size_uninitialized;
    uint32_t entry;
    uint32_t codebase;
    uint32_t database;
    uint32_t imagebase;
    uint32_t sect_align;
    uint32_t file_align;
    uint16_t major_os;
    uint16_t minor_os;
    uint16_t major_image;
    uint16_t minor_image;
    uint16_t major_subsystem;
    uint16_t minor_subsystem;
    uint32_t win32_version;
    uint32_t imagesize;
    uint32_t headersize;
    uint32_t checksum;
    uint16_t subsystem;
    uint16_t dll_characteristics;
    uint32_t size_stack_reserve;
    uint32_t size_stack_commit;
    uint32_t size_heap_reserve;
    uint32_t size_heap_commit;
    uint32_t loader_flags;
    uint32_t num_rvas;
} OPHDR;
#pragma pack(pop)

#pragma pack(push, 1)
typedef struct PE_SECTION {
    char Name[8];
    uint32_t virt_size;
    uint32_t virt_address;
    uint32_t size_rawdata;
    uint32_t ptr_rawdata;
    uint32_t ptr_relocs;
    uint32_t ptr_linenumbers;
    uint16_t num_relocs;
    uint16_t num_linenumbers;
    uint32_t characteristics;
} PE_SECTION;
#pragma pack(pop)

#pragma pack(push, 1)
typedef struct PE_DIR {
    uint32_t export_address;
    uint32_t export_size;
    uint32_t import_address;
    uint32_t import_size;
    uint32_t resource_address;
    uint32_t resource_size;
    uint32_t excep_address;
    uint32_t excep_size;
    uint32_t cert_address;
    uint32_t cert_size;
    uint32_t reloc_address;
    uint32_t reloc_size;
    uint32_t debug_address;
    uint32_t debug_size;
    uint32_t arch_address;
    uint32_t arch_size;
    uint32_t gpr_address;
    uint32_t gpr_size;
    uint32_t tls_address;
    uint32_t tls_size;
    uint32_t config_address;
    uint32_t config_size;
    uint32_t bound_address;
    uint32_t bound_size;
    uint32_t import_table_address;
    uint32_t import_table_size;
    uint32_t delay_address;
    uint32_t delay_size;
    uint32_t clr_address;
    uint32_t clr_size;
    uint32_t reserve_address;
    uint32_t reserve_size;
} PE_DIR;
#pragma pack(pop)

#pragma pack(push, 1)
typedef struct PE_EXPORT {
    uint32_t characteristics;
    uint32_t timestamp;
    uint16_t major_version;
    uint16_t minor_version;
    uint32_t name;
    uint32_t base;
    uint32_t num_functions;
    uint32_t num_names;
    uint32_t ptr_functions;
    uint32_t ptr_names;
    uint32_t ptr_ordinals;
} PE_EXPORT;
#pragma pack(pop)

#pragma pack(push, 1)
typedef struct PE_IMPORT {
    uint32_t characteristics;
    uint32_t timestamp;
    uint32_t forward_chain;
    uint32_t name;
    uint32_t first_thunk;
} PE_IMPORT;
#pragma pack(pop)

#pragma pack(push, 1)
typedef struct PE_IMPORTNAME {
    uint16_t hint;
    char name[1];
} PE_IMPORTNAME;
#pragma pack(pop)

#pragma pack(push, 1)
typedef struct PE_RELOC {
    uint32_t address;
    uint32_t blocksize;
} PE_RELOC;
#pragma pack(pop)

#endif // EXE_H