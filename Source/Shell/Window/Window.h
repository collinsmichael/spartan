#ifndef SHELL_GRAPHICS_WINDOW_H
#define SHELL_GRAPHICS_WINDOW_H
#include <oop.h>
#include <Shell\RunTime\Graphics\Bitmap.h>
#include <Shell\RunTime\Graphics\Font.h>

#pragma pack(push, 1)

typedef struct CWindow {
    void *Prev, *Next, *Head, *Tail;
    short PosX,  PosY,  Wide,  High;
    short NewX,  NewY,  NewW,  NewH;

    void *Parent;
    void *Canvas;
    short OldX,  OldY,  OldW,  OldH;

    void    *Pcb;
    char    *Title;
    int    (*CallBack)(void *win, void *evt);

    int      Dirty;
    int      WindowID;
    int      Visible;
    void    *CustomData;
    int      CustomSize;

    int      Orientation;
    int      Active;
    int      MinValue;
    int      CurValue;
    int      MaxValue;
    int      StepValue;
    int      ThumbWidth;

    CFont   *Font;
    char    *Text;
    int      TextLen;
    int      Caret;
    int      CaretX;
    int      CaretY;

    CBitmap *Bitmap;
    int      ForeGround[4];
    int      BackGround[4];
    int      Lut;
    int      Clut[32];
    char     Extra[256];
} CWindow;

typedef struct IWindow {
    bool     (*Install)(void);
    CWindow *(*CreateFrame)(void *callback);
    CWindow *(*Create )(CWindow *parent, void *callback);
    bool     (*Destroy)(CWindow *window);
    bool     (*SetFont)(CWindow *window, char *path);

    bool     (*PutC    )(CWindow *window, char c);
    bool     (*Print   )(CWindow *window, char *format, ...);
    bool     (*TextOut )(CWindow *window, int x, int y, char *format, ...);
    bool     (*Scroll  )(CWindow *window, int x1, int y1, int x2, int y2, int w, int h, int delta);
    bool     (*Resize  )(CWindow *window, int x, int y, int w, int h);
    bool     (*Redraw  )(CWindow *window);
    bool     (*SetTheme)(CWindow *window, char *theme);
    bool     (*Inherit )(CWindow *window);
} IWindow;

#define WIN_ORIENT_HORZ 0x01
#define WIN_ORIENT_VERT 0x02

#define COLOUR_BACKGROUND  0
#define COLOUR_FOREGROUND  1
#define COLOUR_BORDER      2
#define COLOUR_HILITE      3
#define COLOUR_3DLITE      4
#define COLOUR_SHADOW      5
#define COLOUR_3DSHADOW    6
#define COLOUR_ACCENT      7

#pragma pack(pop)

#endif SHELL_GRAPHICS_WINDOW_H