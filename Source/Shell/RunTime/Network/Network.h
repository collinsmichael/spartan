#ifndef SHELL_RUNTIME_NETWORK_H
#define SHELL_RUNTIME_NETWORK_H
#include <oop.h>
#pragma pack(push, 1)

typedef struct INetwork {
    bool (*Install)(void);
    bool (*Unhook )(int port, void *callback);
    bool (*Hook   )(int port, void *callback);
    bool (*Recv   )(int port, char *packet, int size);
    bool (*Send   )(int port, char *packet, int size);
    bool (*PrintF )(int port, char *format, ...);
} INetwork;

#pragma pack(pop)
#endif SHELL_RUNTIME_NETWORK_H