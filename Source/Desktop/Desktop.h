#ifndef DESKTOP_H
#define DESKTOP_H
#include <oop.h>
#include <Library.h>
#include <Kernel.h>
#include <Shell.h>

#include <Desktop\DeskView\DeskView.h>
#include <Desktop\DeskView\TaskBar.h>
#include <Desktop\DeskView\Cursor.h>
#include <Desktop\Compositor\Compositor.h>
#include <Desktop\IdleThread\IdleThread.h>

bool IThread_Killer(void *a, void *b, void *c);

#endif DESKTOP_H