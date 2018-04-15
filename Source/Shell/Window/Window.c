#include <Kernel.h>
#include <Shell.h>

import IThread   *Thread;
import IProcess  *Process;
import IPool     *Pool;
extern IFont     *Font;
extern IBitmap   *Bitmap;
extern IGraphics *Graphics;
extern IEvent    *Event;
extern ICanvas   *Canvas;
extern ICaret    *Caret;

static CFont   FontCourier;
static CFont   FontTerminal;
static CBitmap IconUp;
static CBitmap IconDown;
static CBitmap IconLeft;
static CBitmap IconRight;

int FrameProc(CWindow *win, CEvent *evt);

bool IWindow_Install(void) {
    if (!Font->LoadFont(&FontCourier,   "Media/Fonts/courier.bmp")) return false;
    if (!Font->LoadFont(&FontTerminal,  "Media/Fonts/terminal.bmp")) return false;
    if (!Bitmap->LoadSprite(&IconUp,    "Media/SysIcons/up.bmp")) return false;
    if (!Bitmap->LoadSprite(&IconDown,  "Media/SysIcons/down.bmp")) return false;
    if (!Bitmap->LoadSprite(&IconLeft,  "Media/SysIcons/left.bmp")) return false;
    if (!Bitmap->LoadSprite(&IconRight, "Media/SysIcons/right.bmp")) return false;
    return true;
}

bool IWindow_Destroy(CWindow *window) {
    if (!window) return false;
    CPeb *peb = Process->GetPeb();
    if (!peb) return false;
    return Pool->Remove(peb->WindowPool, window);
}

bool IWindow_SetTheme(CWindow *window, char *theme) {
    static int ThemeCycle = 0;
    static int Grey[32] = {
    //  BACKGROUND FOREGROUND BORDER     HILITE     3DLITE     SHADOW     3DSHADOW   ACCENT
        0x005B5B5B,0x009B9B9B,0x00000000,0x00717171,0x00414141,0x00414141,0x00717171,0x00000000, // normal
        0x00656565,0x00BBBBBB,0x00000000,0x00919191,0x00616161,0x00616161,0x00919191,0x00000000, // onenter
        0x00515151,0x007B7B7B,0x00000000,0x00575757,0x00414141,0x00414141,0x00575757,0x00000000, // onclick
        0x00651F1F,0x00BB5B5B,0x00000000,0x008B3F3F,0x00652F2F,0x00652F2F,0x008B3F3F,0x00000000  // onhanging
    };
    static int Blue[32] = {
    //  BACKGROUND FOREGROUND BORDER     HILITE     3DLITE     SHADOW     3DSHADOW   ACCENT
        0x00000000,0x007070E0,0x00000000,0x004040A0,0x00404080,0x00404080,0x004040A0,0x00000000, // normal
        0x00000000,0x007878F0,0x00000000,0x004848B0,0x00484890,0x00484890,0x004848B0,0x00000000, // onenter
        0x00000000,0x006868D0,0x00000000,0x00383890,0x00383870,0x00383870,0x00383890,0x00000000, // onclick
        0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000  // onhanging
    };
    static int Green[32] = {
    //  BACKGROUND FOREGROUND BORDER     HILITE     3DLITE     SHADOW     3DSHADOW   ACCENT
        0x00000000,0x0068D068,0x00000000,0x0040A040,0x00408040,0x00408040,0x0040A040,0x00000000, // normal
        0x00000000,0x0070E070,0x00000000,0x0048B048,0x00489048,0x00489048,0x0048B048,0x00000000, // onenter
        0x00000000,0x0060C060,0x00000000,0x00389038,0x00387038,0x00387038,0x00389038,0x00000000, // onclick
        0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000  // onhanging
    };
    static int Red[32] = {
    //  BACKGROUND FOREGROUND BORDER     HILITE     3DLITE     SHADOW     3DSHADOW   ACCENT
        0x00000000,0x00D07878,0x00000000,0x00A04040,0x00804040,0x00804040,0x00A04040,0x00000000, // normal
        0x00000000,0x00E07070,0x00000000,0x00B04848,0x00904848,0x00904848,0x00B04848,0x00000000, // onenter
        0x00000000,0x00C06060,0x00000000,0x00903838,0x00703838,0x00703838,0x00903838,0x00000000, // onclick
        0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000  // onhanging
    };
    static int Light[32] = {
    //  BACKGROUND FOREGROUND BORDER     HILITE     3DLITE     SHADOW     3DSHADOW   ACCENT
        0x00000000,0x00D0D0D0,0x00000000,0x00A0A0A0,0x00808080,0x00808080,0x00A0A0A0,0x00000000, // normal
        0x00000000,0x00E0E0E0,0x00000000,0x00B0B0B0,0x00909090,0x00909090,0x00B0B0B0,0x00000000, // onenter
        0x00000000,0x00C0C0C0,0x00000000,0x00909090,0x00707070,0x00707070,0x00909090,0x00000000, // onclick
        0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000  // onhanging
    };
    static int Dark[32] = {
    //  BACKGROUND FOREGROUND BORDER     HILITE     3DLITE     SHADOW     3DSHADOW   ACCENT
        0x00000000,0x00606060,0x00000000,0x00505050,0x00303030,0x00303030,0x00505050,0x00000000, // normal
        0x00000000,0x00707070,0x00000000,0x00606060,0x00404040,0x00404040,0x00606060,0x00000000, // onenter
        0x00000000,0x00505050,0x00000000,0x00404040,0x00505050,0x00505050,0x00404040,0x00000000, // onclick
        0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000  // onhanging
    };

    int *clut = nullptr;
    if (StrCmp(theme, "Grey" ) == 0) clut = Grey;
    if (StrCmp(theme, "Red"  ) == 0) clut = Red;
    if (StrCmp(theme, "Green") == 0) clut = Green;
    if (StrCmp(theme, "Blue" ) == 0) clut = Blue;
    if (StrCmp(theme, "Light") == 0) clut = Light;
    if (StrCmp(theme, "Dark" ) == 0) clut = Dark;

    if (!clut) {
        static void *Theme[6] = {
            Grey, Blue, Green, Red, Light, Dark
        };
        clut = Theme[ThemeCycle++];
        ThemeCycle = ThemeCycle % 4;
    }

    movsd(window->Clut, clut,  32);
    return true;
}

bool IWindow_Inherit(CWindow *window) {
    CWindow *frame = window;
    while (frame->Parent) frame = frame->Parent;
    movsd(window->Clut, frame->Clut, 32);
    return true;
}

bool IWindow_SetFont(CWindow *window, char *path) {
    CFont *font = nullptr;
    if (StrCmp(path, "Media/Fonts/courier.bmp") == 0) font = &FontCourier;
    if (StrCmp(path, "Media/Fonts/terminal.bmp") == 0) font = &FontTerminal;
    if (!font) return false;
    window->Font = font;
    return true;
}

CBitmap *IWindow_GetIcon(char *path) {
    CBitmap *icon = nullptr;
    if (StrCmp(path, "Media/SysIcons/up.bmp")    == 0) icon = &IconUp;
    if (StrCmp(path, "Media/SysIcons/down.bmp")  == 0) icon = &IconDown;
    if (StrCmp(path, "Media/SysIcons/left.bmp")  == 0) icon = &IconLeft;
    if (StrCmp(path, "Media/SysIcons/right.bmp") == 0) icon = &IconRight;
    return icon;
}

CWindow *IWindow_Spawn(CWindow *parent, void *callback) {
    CPeb *peb = Process->GetPeb();
    if (!peb) {
        Logger(" [FAIL] Window->Spawn peb = %X\n", peb);
        return 0;
    }

    CCanvas *canvas = Process->GetCanvas();
    if (!canvas) {
        Logger(" [FAIL] Window->Spawn canvas = %X\n", canvas);
        return 0;
    }

    CWindow blank;
    stosd(&blank, 0, sizeof(CWindow)/4);
    blank.Pcb      = Process->GetPcb();
    blank.Parent   = parent;
    blank.Title    = " Window Title";
    blank.CallBack = callback;
    blank.Canvas   = canvas;
    blank.Font     = &FontTerminal;
    blank.Visible  = true;
    blank.Dirty    = true;
    return (CWindow*)Pool->Insert(peb->WindowPool, &blank);
}

bool IWindow_CreateEvents(CWindow *window) {
    CEvent blank;

    CEvent *load = &blank;
    stosd(load, 0, sizeof(CEvent)/4);
    load->Target = window;
    load->Code = EVT_ONLOAD;
    if (!window->CallBack(window, load)) return false;

    CEvtArea *size = (CEvtArea*)&blank;
    stosd(size, 0, sizeof(CEvent)/4);
    size->Target = window;
    size->Code = EVT_ONSIZE;
    size->PosX = window->PosX;
    size->PosY = window->PosY;
    size->Wide = window->Wide;
    size->High = window->High;
    if (!window->CallBack(window, size)) return false;

    CEvent *draw = &blank;
    stosd(draw, 0, sizeof(CEvent)/4);
    draw->Target = window;
    draw->Code = EVT_ONDRAW;
    Event->RaiseEvent(draw);
    return true;
}

CWindow *IWindow_CreateFrame(void *callback) {
    CCanvas *canvas = Process->GetCanvas();
    if (!canvas) return 0;
    CCanvas *screen = Canvas->GetScreen();
    if (!screen) return 0;

    CWindow *frame = IWindow_Spawn(nullptr, callback);
    if (!frame) return 0;

    static int DefPosX = 0;
    static int DefPosY = 0;
    frame->NewX = frame->PosX = frame->OldX = (DefPosX += 32);
    frame->NewY = frame->PosY = frame->OldY = (DefPosY += 32);
    if (DefPosX >= screen->ResX*1/2) DefPosX = 0;
    if (DefPosY >= screen->ResY*1/2) DefPosY = 0;
    frame->Wide = frame->Wide ? frame->Wide : screen->ResX/2;
    frame->High = frame->High ? frame->High : screen->ResY/2;
    frame->NewW = frame->OldW = frame->Wide;
    frame->NewH = frame->OldH = frame->High;

    if (!IWindow_CreateEvents(frame)) {
        IWindow_Destroy(frame);
        return 0;
    }

    CWindow *next = canvas->TopMost;
    frame->Next = next;
    if (next) next->Prev = frame;
    else canvas->BtmMost = frame;
    canvas->TopMost = frame;

    Thread->Yield();
    CWindow *cursor = screen->TopMost;
    if (cursor) {
        Canvas->Push(screen, frame);
        Canvas->Pop(screen, cursor);
        Canvas->Push(screen, cursor);
    }
    return frame;
}

CWindow *IWindow_Create(CWindow *parent, void *callback) {
    if (!callback) return 0;

    CCanvas *canvas = Process->GetCanvas();
    if (!canvas) return 0;

    if (!parent) {
        CWindow *frame = IWindow_CreateFrame(FrameProc);
        if (!frame) return 0;
        parent = frame;
    }

    CWindow *window = IWindow_Spawn(parent, callback);
    if (!window) {
        Logger(" Window->Create window = %X\n", window);
        return 0;
    }
    window->NewX = window->PosX = window->OldX =  8;
    window->NewY = window->PosY = window->OldY = 33;
    if (!window->Wide) window->Wide = parent->Wide - 16;
    if (!window->High) window->High = parent->High - 41;
    window->NewW = window->OldW = window->Wide;
    window->NewH = window->OldH = window->High;

    window->ForeGround[0] = 0x00E0E0EF;
    window->BackGround[0] = 0x00300A24;
    if (!IWindow_CreateEvents(window)) {
        IWindow_Destroy(window);
        return 0;
    }

    CWindow *next = parent->Head;
    window->Next = next;
    if (next) next->Prev = window;
    else parent->Tail = window;
    parent->Head = window;
    Canvas->Push(canvas, window);
    return window;
}

static bool IWindow_PutC(CWindow *window, char c) {
    return Caret->PutC(window, c);
}

static bool IWindow_Print(CWindow *window, char *format, ...) {
    char text[1024];
	va_list *list;
    va_start(list, format);
	VaFormat(text, format, list);
	va_end(list);
    return Caret->Print(window, text);
}

static bool IWindow_TextOut(CWindow *window, int x, int y, char *format, ...) {
    char text[1024];
	va_list *list;
    va_start(list, format);
	VaFormat(text, format, list);
	va_end(list);
    return Caret->TextOut(window, x, y, text);
}

static bool IWindow_Scroll(CWindow *window, int x1, int y1, int x2, int y2, int w, int h, int delta) {
    return Caret->Scroll(window, x1, y1, x2, y2, w, h, delta);
}

static bool IWindow_Resize(CWindow *window, int x, int y, int w, int h) {
    if (!window) return false;
    window->NewX = x; window->NewY = y;
    window->PosX = x; window->PosY = y;
    window->OldX = x; window->OldY = y;
    window->Wide = w; window->High = h;
    window->NewW = w; window->NewH = h;

    if (window->CallBack) {
        static CEvent blank;
        CEvtArea *area = (CEvtArea*)&blank;
        stosd(area, 0, sizeof(CEvent)/4);
        area->Source = nullptr;
        area->Target = window;
        area->PosX   = x;
        area->PosY   = y;
        area->Wide   = w;
        area->High   = h;
        area->Code   = EVT_ONSIZE;
        window->CallBack(window, area);
        area->Code   = EVT_ONMOVE;

        window->CallBack(window, area);
        area->Code   = EVT_ONDRAW;
        window->CallBack(window, area);

        CCanvas *screen = Canvas->GetScreen();
        CCanvas *canvas = Canvas->GetCanvas();
        Canvas->Show(screen, canvas->BtmMost);
    }
    return true;
}

static bool IWindow_Redraw(CWindow *window) {
    if (!window) return false;
    if (!window->Visible) return true;

    static CEvent draw;
    stosd(&draw, 0, sizeof(CEvent)/4);
    draw.Source = nullptr;
    draw.Target = window;
    draw.Code   = EVT_ONDRAW;
    Event->RaiseEvent(&draw);
    return true;
}

static IWindow window = {
    IWindow_Install,
    IWindow_CreateFrame,
    IWindow_Create,
    IWindow_Destroy,
    IWindow_SetFont,

    IWindow_PutC,
    IWindow_Print,
    IWindow_TextOut,
    IWindow_Scroll,
    IWindow_Resize,
    IWindow_Redraw,
    IWindow_SetTheme,
    IWindow_Inherit
};

export IWindow *Window = &window;