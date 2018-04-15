#include <Kernel.h>
#include <Shell.h>
#include <Desktop.h>

import IDevice *Device;
import IVesa   *Vesa;
import ICanvas *Canvas;
import IRender *Render;
static CCanvas *screen;

static int y;
static int Frequency;
static int CountDown;
static int RefreshHz;

#ifdef OPTIMIZED
#define FREQUENCY (33*Hz)
#else
#define FREQUENCY (60*Hz)
#endif

static int ICompositor_DrawScanLine(CCanvas *canvas, int y) {
    if (y < canvas->ResY) {
        if (canvas->Debug) {
            Canvas->DebugDirtyScanLine(canvas, y);
        } else {
            Canvas->FindDirtyScanLine(canvas, y);
        }
        y++;
    } else if (y == canvas->ResY) {
        Render->Decoder(canvas);
        y++;
    } else {
        for (CWindow *list = canvas->TopMost; list; list = list->Next) {
            if (list->NewX != list->PosX || list->NewY != list->PosY
            ||  list->NewW != list->Wide || list->NewH != list->High) {
                list->OldX = list->NewX; list->NewX = list->PosX;
                list->OldY = list->NewY; list->NewY = list->PosY;
                list->OldW = list->NewW; list->NewW = list->Wide;
                list->OldH = list->NewH; list->NewH = list->High;
                list->Dirty = true;
            } else {
                list->Dirty = MAX(list->Dirty - 1, false);
            }
            if (!list->Visible) list->Dirty = true;
        }
        y = 0;
    }
    return y;
}

static u32 ICompositor_VertBlank(u32 err, u32 esp) {
    y = ICompositor_DrawScanLine(screen, y);
    #ifndef OPTIMIZED
    outb(PIT_COMMAND, PIT_CH0|PIT_BOTH|PIT_RATE);
    outb(PIT_TIMER0, CountDown % 256);
    outb(PIT_TIMER0, CountDown / 256);
    #endif
    return esp;
}

static bool ICompositor_SetRefreshRate(int hertz) {
    // Programmable Interval Timer (PIT)
    // Divider frequency is given by the formula:
    //
    //                  1193180                        1193180
	//     COUNTDOWN = ---------          FREQUENCY = ---------
	//                 FREQUENCY                      COUNTDOWN
    if (hertz <= 0 || hertz > 1193180) return false;
    int divider = 1193180/(hertz*screen->ResY);
    if (divider <= 0 || divider > 65536) return false;
    RefreshHz = hertz;
    CountDown = divider;
    Frequency = 1193180/CountDown;

    Debug(" [info] PIT Timer Interval Updated\n");
    Debug(" [info] Worst case Refresh Rate = %d Hz (Under maximum strain)\n", RefreshHz);
    Debug(" [info] PIT Frequency = %d Hz\n", Frequency);
    Debug(" [info] PIT Divider   = %d\n", CountDown);

    outb(PIT_COMMAND, PIT_CH0|PIT_BOTH|PIT_RATE);
    outb(PIT_TIMER0, CountDown % 256);
    outb(PIT_TIMER0, CountDown / 256);
    return true;
}

static u32 HushStub(u32 err, u32 esp) {
    return esp;
}

static bool ICompositor_Install(void) {
    screen = Canvas->GetScreen();
    ICompositor_SetRefreshRate(FREQUENCY);
    Device->Latch(IRQ_PIT, ICompositor_VertBlank);
    Device->Latch(IRQ_PARA1, HushStub);
    return true;
}

static ICompositor compositor = {
    ICompositor_Install,
    ICompositor_DrawScanLine
};

ICompositor *Compositor = &compositor;