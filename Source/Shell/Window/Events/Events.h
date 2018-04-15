#ifndef SHELL_WINDOW_EVENTS_H
#define SHELL_WINDOW_EVENTS_H
#include <oop.h>
#include <Shell\Window\Window.h>

typedef struct CEvent {
    int      SequenceNo;
    int      TimeStamp;
    CWindow *Source;
    CWindow *Target;
    int      Code;
    char     Data[108];
} CEvent;

typedef struct CEvtArea {
    int      SequenceNo;
    int      TimeStamp;
    CWindow *Source;
    CWindow *Target;
    int      Code;

    int      PosX;
    int      PosY;
    int      Wide;
    int      High;
} CEvtArea;

typedef struct CEvtKey {
    int      SequenceNo;
    int      TimeStamp;
    CWindow *Source;
    CWindow *Target;
    int      Code;

    u8       Scan;
    u8       Down;
    u8       Key;
    char     Ascii;
    u8       Ctrl;
    u8       Shift;
    u8       Alt;
    u8       Locks;
} CEvtKey;

typedef struct CEvtMouse {
    int      SequenceNo;
    int      TimeStamp;
    CWindow *Source;
    CWindow *Target;
    int      Code;

    CMouse   NewMouse;
    CMouse   OldMouse;
} CEvtMouse;

typedef struct IEvent {
    bool (*DebugEvent )(void *evt);
    bool (*RaiseEvent )(void *evt);
    bool (*UpdateEvent)(void *evt);
    bool (*GetEvent   )(void *evt);
    bool (*PeekEvent  )(void *evt);
    bool (*HandleEvent)(void *evt);
    bool (*Forward    )(void *win, void *evt);
    bool (*IsAscii    )(CEvtKey *evt);
} IEvent;

#define EVT_ONQUIT      0x0000
#define EVT_ONLOAD      0x0001
#define EVT_ONHIDE      0x0002
#define EVT_ONSHOW      0x0003
#define EVT_ONMOVE      0x0004
#define EVT_ONSIZE      0x0005
#define EVT_ONDRAW      0x0006
#define EVT_ONACTION    0x0007
#define EVT_ONFOCUSLOST 0x0008
#define EVT_ONFOCUS     0x0009
#define EVT_ONKEYUP     0x000A
#define EVT_ONKEYDOWN   0x000B
#define EVT_ONMOUSE     0x000C
#define EVT_ONWHEEL     0x000D
#define EVT_ONLEAVE     0x000E
#define EVT_ONENTER     0x000F
#define EVT_ONHOVER     0x0010
#define EVT_ONCLICK     0x0011
#define EVT_ONRELEASE   0x0012
#define EVT_ONDRAG      0x0013
#define EVT_ONDROP      0x0014
#define EVT_ONTIMER     0x0015
#define EVT_DEFAULTMAX  0x0015


#define KEY_ESC         0x01
#define KEY_BACK        0x0E
#define KEY_TAB         0x0F
#define KEY_ENTER       0x1C
#define KEY_SPACE       0x39
#define KEY_LWIN        0x48
#define KEY_RWIN        0x49
#define KEY_LALT        0x4A
#define KEY_RALT        0x4B
#define KEY_LSHIFT      0x4C
#define KEY_RSHIFT      0x4D
#define KEY_LCTRL       0x4E
#define KEY_RCTRL       0x4F

#define KEY_PRINT       0x50

#define KEY_CAPSLOCK    0x51
#define KEY_NUMLOCK     0x52
#define KEY_SCRLLOCK    0x53
#define KEY_PRNTSCRN    0x54
#define KEY_BREAK       0x55
#define KEY_INSERT      0x56
#define KEY_DELETE      0x57

#define KEY_HOME        0x58
#define KEY_END         0x59
#define KEY_PGUP        0x5A
#define KEY_PGDN        0x5B
#define KEY_UP          0x5C
#define KEY_DOWN        0x5D
#define KEY_LEFT        0x5E
#define KEY_RIGHT       0x5F
#define KEY_NUM0        0x60
#define KEY_NUM1        0x61
#define KEY_NUM2        0x62
#define KEY_NUM3        0x63
#define KEY_NUM4        0x64
#define KEY_NUM5        0x65
#define KEY_NUM6        0x66
#define KEY_NUM7        0x67
#define KEY_NUM8        0x68
#define KEY_NUM9        0x69
#define KEY_NUMDOT      0x6A
#define KEY_NUMPLUS     0x6B
#define KEY_NUMMINUS    0x6C
#define KEY_NUMMULTIPLY 0x6D
#define KEY_NUMDIVIDE   0x6E
#define KEY_NUMENTER    0x6F
#define KEY_F1          0x71
#define KEY_F2          0x72
#define KEY_F3          0x73
#define KEY_F4          0x74
#define KEY_F5          0x75
#define KEY_F6          0x76
#define KEY_F7          0x77
#define KEY_F8          0x78
#define KEY_F9          0x79
#define KEY_F10         0x7A
#define KEY_F11         0x7B
#define KEY_F12         0x7C

#endif SHELL_WINDOW_EVENTS_H