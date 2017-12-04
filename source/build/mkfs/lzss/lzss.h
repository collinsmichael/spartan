#ifndef LZSS_H
#define LZSS_H
#include <stdint.h>

#pragma pack(push, 1)
typedef struct lzhdr_t {
    uint32_t magic;
    uint32_t ecount;
    uint32_t dcount;
    uint8_t  EI;
    uint8_t  EJ;
    uint8_t  P;
    uint8_t  padding;
} lzhdr_t;
#pragma pack(pop)

#endif // LZSS_H