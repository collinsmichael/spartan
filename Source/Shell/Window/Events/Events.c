#include <Kernel.h>
#include <Shell.h>

import IClock   *Clock;
import IPool    *Pool;
import IVesa    *Vesa;
import IProcess *Process;
import IThread  *Thread;
extern IWindow  *Window;
extern ICanvas  *Canvas;

bool DefaultProc(CWindow *win, CEvent *evt);

bool IEvent_DebugEvent(CEvent *evt) {
    char *name[] = {
        "OnQuit",      "OnLoad",     "OnHide",     "OnShow",
        "OnMove",      "OnSize",     "OnDraw",     "OnAction",
        "OnFocusLost", "OnFocus",    "OnKeyUp",    "OnKeyDown",
        "OnMouse",     "OnWheel",    "OnLeave",    "OnEnter",
        "OnHover",     "OnClick",    "OnRelease",  "OnDrag",
        "OnDrop",      "OnTimer"
    };

    if (!evt) return false;
    int code = evt->Code;

    char *text = (code >= EVT_ONQUIT && code <= EVT_DEFAULTMAX)
               ? name[code] : "OnAppDefined";

    Debug(" evt            = %X\n", evt);
    Debug(" evt.SequenceNo = %X\n", evt->SequenceNo);
    Debug(" evt.TimeStamp  = %X\n", evt->TimeStamp);
    Debug(" evt.Source     = %X\n", evt->Source);
    Debug(" evt.Target     = %X (%s)\n", evt->Target, text);
    Debug(" evt.Code       = %X\n", evt->Code);
    Debug(" evt.Data       =");
    for (int i = 0; i < sizeof(evt->Data); i++) {
        u8 *c = (u8*)evt->Data;
        if (i % 0x10 == 0x00) Debug("\n   ");
        if (i % 0x04 == 0x00) Debug(" ");

        if (c[i] < 0x10) Debug(" 0%X", c[i]);
        else             Debug(" %X",  c[i]);
    }
    Debug("\n\n");
    return true;
}


bool IEvent_HandleEvent(CEvent *evt) {
    if (!evt) return false;
    CWindow *window = evt->Target;
    if (!window) return false;
    bool status = DefaultProc(window, evt);
    return status;
}

bool IEvent_Forward(CWindow *target, CEvent *evt) {
    //Debug(" Event->Forward(%X, %X)\n", target, evt);
    if (!target|| !evt) return true;
    bool result = true;
    if (target->CallBack) {
        CWindow *prev = evt->Target;
        evt->Target = target;
        result = target->CallBack(target, evt);
        evt->Target = prev;
    }
    return result;
}

bool IEvent_RaiseEvent(CEvent *evt) {
    if (!evt) return false;

    CWindow *target = evt->Target;
    if (!target) return false;

    CPcb *pcb = (CPcb*)target->Pcb;
    if (!pcb) return false;

    CPeb *peb = (CPeb*)pcb->Peb;
    if (!peb) return false;

    CEvtPool *pool = (CEvtPool*)peb->EventPool;
    if (!pool) return false;

    evt->TimeStamp = Clock->GetTicks();
    CEvent *obj = (CEvent*)&pool->Data[pool->Tail*pool->Gran];
    int sequence = pool->Tail;
    pool->Tail = ++pool->Tail % pool->Count;
    if (pool->Head == pool->Tail) {
        pool->Hanging = 1;
        pool->Head = ++pool->Head % pool->Count;
    }
    stosd(obj, 0, pool->Gran/4);
    movsd(obj, evt, sizeof(CEvent)/4);
    obj->SequenceNo = sequence;
    evt->SequenceNo = sequence;
    return true;
}

bool IEvent_UpdateEvent(CEvent *evt) {
    if (!evt) return false;
    CWindow *target = evt->Target;
    if (!target) return false;

    CPcb *pcb = (CPcb*)target->Pcb;
    if (!pcb) return false;

    CPeb *peb = (CPeb*)pcb->Peb;
    if (!pcb) return false;

    CEvtPool *pool = (CEvtPool*)peb->EventPool;
    if (!pool) return false;

    bool updated = false;
    CEvent *obj = (CEvent*)&pool->Data[pool->Tail*pool->Gran];
    if (pool->Head != pool->Tail)
    for (int i = pool->Tail-1; i != pool->Head-1; i--) {
        i = i % pool->Count;
        CEvent *old = (CEvent*)&pool->Data[i*pool->Gran];
        if (old->Code == evt->Code) {
            obj = old;
            updated = true;
            break;
        }
    }
    if (!updated) {
        pool->Tail = ++pool->Tail % pool->Count;
        if (pool->Head == pool->Tail) {
            pool->Hanging = 1;
            pool->Head = ++pool->Head % pool->Count;
        }
    }
    evt->TimeStamp = Clock->GetTicks();
    stosd(obj, 0, pool->Gran/4);
    movsd(obj, evt, sizeof(CEvent)/4);
    return true;
}

bool IEvent_GetEvent(CEvent *evt) {
    if (!evt) return false;
    stosd(evt, 0, sizeof(CEvent)/4);
    Thread->Yield();

    CPeb *peb = Process->GetPeb();
    if (!peb) return false;

    CEvtPool *pool = (CEvtPool*)peb->EventPool;
    if (!pool) return false;
    while (pool->Head == pool->Tail) {
        Thread->Yield();
    }

    CEvent *src = (CEvent*)&pool->Data[pool->Head*pool->Gran];
    pool->Head = ++pool->Head % pool->Count;
    pool->Hanging = 0;
    movsd(evt, src, sizeof(CEvent)/4);
    return true;
}

bool IEvent_PeekEvent(CEvent *evt) {
    if (!evt) return false;
    CPeb *peb = Process->GetPeb();
    CEvtPool *pool = (CEvtPool*)peb->EventPool;
    if (pool->Head == pool->Tail) return false;

    CEvent *src = (CEvent*)&pool->Data[pool->Head*pool->Gran];
    pool->Head = ++pool->Head % pool->Count;
    pool->Hanging = 0;
    movsd(evt, src, sizeof(CEvent)/4);
    return true;
}

bool IEvent_IsAscii(CEvtKey *evt) {
    if (!evt) return false;
    unsigned char key = evt->Key & 0x7F;
    return
    "\x00\x00\x01\x01\x01\x01\x01\x01\x01\x01\x01\x01\x01\x01\x01\x01"
    "\x01\x01\x01\x01\x01\x01\x01\x01\x01\x01\x01\x01\x01\x00\x01\x01"
    "\x01\x01\x01\x01\x01\x01\x01\x01\x01\x01\x00\x01\x01\x01\x01\x01"
    "\x01\x01\x01\x01\x01\x01\x00\x01\x00\x01\x00\x00\x00\x00\x00\x00"
    "\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x01\x00\x00\x00\x01\x00"
    "\x00\x00\x00\x00\x00\x00\x01\x00\x00\x00\x00\x00\x00\x00\x00\x00"
    "\x01\x01\x01\x01\x01\x01\x01\x01\x01\x01\x01\x01\x01\x01\x01\x01"
    "\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00"[key];
}

static IEvent events = {
    IEvent_DebugEvent,
    IEvent_RaiseEvent,
    IEvent_UpdateEvent,
    IEvent_GetEvent,
    IEvent_PeekEvent,
    IEvent_HandleEvent,
    IEvent_Forward,
    IEvent_IsAscii
};

export IEvent *Event = &events;