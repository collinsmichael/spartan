/* ************************************************************************** */
/* author(s) : Michael Collins                                                */
/* file name : include/posix/stdint.h                                         */
/* file date : 2017/12/03                                                     */
/* file info : libc type definitions                                          */
/* ************************************************************************** */
#ifndef STDINT_H
#define STDINT_H

typedef unsigned __int8  uint8_t;
typedef unsigned __int16 uint16_t;
typedef unsigned __int32 uint32_t;
typedef unsigned __int64 uint64_t;
typedef signed   __int8  int8_t;
typedef signed   __int16 int16_t;
typedef signed   __int32 int32_t;
typedef signed   __int64 int64_t;

typedef unsigned __int32 size_t;

#endif // STDINT_H