#ifndef OOP_H
#define OOP_H

#define NOTE(TEXT)
#define TODO(TEXT)
#define HACK(TEXT)

NOTE(" ************************************************************************** ")
NOTE(" Object Oriented programming macros for mocking classes as pure C structs.  ")
NOTE(" This is the approach Bjarne Stroustrup used when creating C++              ")
NOTE("                                                                            ")
NOTE(" I am not intimate enough with the GCC command line switches to remove all  ")
NOTE(" references to the compiler generated code. I am however able to do so with ")
NOTE(" MSVC when compiling Pure C. These macros are the closest I'm willing to go ")
NOTE(" towards implementing an OOP kernel as that requires constant battles with  ")
NOTE(" the MSVC compiler.                                                         ")
NOTE(" ************************************************************************** ")
NOTE("                                                                            ")
NOTE(" ************************************************************************** ")
NOTE(" [IMPORTANT LIMITATION] it is required to seperate data members and methods ")
NOTE(" All methods must be encapsulated within an associated interface.           ")
NOTE(" As an added benefit this approach enforces good testability practise which ")
NOTE(" accomodate mock and stub injections.                                       ")
NOTE("                                                                            ")
NOTE(" All data members are converted to a pure c data structure. This avoids the ")
NOTE(" need to initialize method members for each instance which is inconvenient. ")
NOTE(" Also this approach ensures consistent behaviour accross all instances that ")
NOTE(" share a common interface.                                                  ")
NOTE("                                                                            ")
NOTE(" Inheritance and polymorphism are achieved by defining alternative data and ")
NOTE(" interface structures which may share implementations or differ as needed.  ")
NOTE(" There is an unavoidable need to cast between inherited objects.            ")
NOTE(" ************************************************************************** ")

// oop macros
#define import           _declspec(dllimport)
#define export           _declspec(dllexport)

// standard types
#define nullptr          (0)
#define null             (0)
#define bool             int
#define false            (0)
#define true             (!false)
typedef signed   __int8  s8;
typedef signed   __int16 s16;
typedef signed   __int32 s32;
typedef signed   __int64 s64;
typedef unsigned __int8  u8;
typedef unsigned __int16 u16;
typedef unsigned __int32 u32;
typedef unsigned __int64 u64;
typedef char*            str;

typedef int* va_list;
#define va_start(ap, param) do {ap = (va_list*)&param; ap++;} while (0)
#define va_arg(ap, type)    (*((type*)(ap))++)
#define va_end(ap)

// constants
#define KB (1024)
#define MB (1024*KB)
#define GB (1024*MB)
#define TB (1024*GB)

#define Hz  (1)
#define KHz (1000*Hz)
#define MHz (1000*KHz)
#define GHz (1000*MHz)
#define THz (1000*GHz)

// standard macros
#define align(a, b)      (((a) + (b) - 1) & ~((b)-1))
#define elementsof(a)    (sizeof(a) / sizeof(a[0]))
#define MIN(a, b)        (((a) < (b)) ? (a) : (b))
#define MAX(a, b)        (((a) > (b)) ? (a) : (b))
#define ABS(n)           (((n) < 0) ? -(n) : (n))

#endif OOP_H // OBJECT ORIENTED PROGRAMMING