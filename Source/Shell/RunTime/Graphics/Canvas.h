#ifndef SHELL_GRAPHICS_CANVAS_H
#define SHELL_GRAPHICS_CANVAS_H
#include <oop.h>
#include <Shell\Window\Window.h>
#pragma pack(push, 1)

typedef struct CCanvas {
    void    *Peb;
    void    *Pcb;
    CWindow *TopMost;
    CWindow *BtmMost;
    CWindow *Dragged;
    CWindow *Focus;
    CWindow *Hover;
    char    *Disp;
    char    *Draw;
    int      ScanLine;
    bool     Debug;
    void    *DebugView;
    int      ResX;
    int      ResY;
    int      PosX;
    int      PosY;
    u8       Head;
    u8       Tail;
    u16      CmdPad;
    u32      Ring[256];
} CCanvas;

typedef struct ICanvas {
    void     (*Debug     )(CCanvas *canvas, bool toggle);
    CCanvas *(*GetScreen )(void);
    CCanvas *(*GetCanvas )(void);
    CWindow *(*FindWindow)(CWindow *head, int x, int y);
    CWindow *(*FindChild )(CWindow *parent, int x, int y);
    CWindow *(*FindById  )(CWindow *parent, int id);

    bool (*DrawDirtyScanLine)(int *pixel, CWindow *list, int y, int x1, int x4);
    void (*FindDirtyScanLine)(CCanvas *canvas, int y);
    void (*DebugDirtyScanLine)(CCanvas *canvas, int y);

    bool (*Push)(CCanvas *canvas, CWindow *window);
    bool (*Pop )(CCanvas *canvas, CWindow *window);
    bool (*Show)(CCanvas *canvas, CWindow *window);
    bool (*Hide)(CCanvas *canvas, CWindow *window);
    bool (*Move)(CCanvas *canvas, CWindow *window, int x, int y);
    bool (*Size)(CCanvas *canvas, CWindow *window, int w, int h);
    bool (*Flip)(CCanvas *canvas);
    bool (*Blit)(CCanvas *canvas, int x1, int y1, int x2, int y2);
} ICanvas;

#pragma pack(pop)
#endif SHELL_GRAPHICS_CANVAS_H