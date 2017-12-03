/* ************************************************************************** */
/* author(s) : Michael Collins                                                */
/* file name : include/spartan/boot.h                                         */
/* file date : 2017/12/03                                                     */
/* file info : multi-boot definitions                                         */
/* ************************************************************************** */
#ifndef BOOT_H
#define BOOT_H
#include <stdint.h>
#pragma pack(push, 1)

typedef struct MBHDR {
    uint32_t magic;
    uint32_t flags;
    uint32_t check;
    uint32_t header;
    uint32_t text_base;
    uint32_t text_end;
    uint32_t bss_end;
    uint32_t entry;
    uint32_t vesa_type;
    uint32_t vesa_resx;
    uint32_t vesa_resy;
    uint32_t vesa_bits;
    // spartan additions
    uint32_t magic2;
    uint32_t info;
    uint32_t mzhdr;
    uint32_t initrd;
} MBHDR;

#define MBHDR_MAGIC  (0x1BADB002)
#define MBHDR_FLAGS  (0x00010007)
#define MBHDR_CHECK  (-(MBHDR_MAGIC+MBHDR_FLAGS))

#pragma pack(pop)
#endif // BOOT_H