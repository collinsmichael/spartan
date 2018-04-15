#ifndef DESKTOP_COMPOSITOR_CURSOR_H
#define DESKTOP_COMPOSITOR_CURSOR_H
#include <oop.h>

#define MOUSE_HOVER   0
#define MOUSE_CLICKED 1
#define MOUSE_RELEASE 2
#define MOUSE_DRAGGED 3

bool CaretInstall(void);
bool CursorProc(CWindow *win, CEvent *evt);

#endif DESKTOP_COMPOSITOR_CURSOR_H