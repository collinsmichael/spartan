#ifndef DESKTOP_IDLETHREAD_H
#define DESKTOP_IDLETHREAD_H
#include <oop.h>

typedef struct IIdleThread {
    bool (*Run)(void);
} IIdleThread;

#endif DESKTOP_IDLETHREAD_H