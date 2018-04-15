#ifndef SHELL_WINDOW_WIDGET_CARET_H
#define SHELL_WINDOW_WIDGET_CARET_H
#include <oop.h>

typedef struct ICaret {
    bool (*Scroll)(CWindow *window, int x1, int y1, int x2, int y2, int w, int h, int delta);
    bool (*Show)(CWindow *window);
    bool (*Hide)(CWindow *window);
    bool (*Advance)(CWindow *window);
    bool (*PutC)(CWindow *window, char c);
    bool (*Print)(CWindow *window, char *format, ...);
    bool (*TextOut)(CWindow *window, int x, int y, char *format, ...);
} ICaret;

#endif SHELL_WINDOW_WIDGET_CARET_H