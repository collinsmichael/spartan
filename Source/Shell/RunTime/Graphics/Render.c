#include <Kernel.h>
#include <Shell.h>

extern IEvent *Event;

static void IRender_Nop(CCanvas *canvas);
static void IRender_Push(CCanvas *canvas);
static void IRender_Pop(CCanvas *canvas);
static void IRender_Show(CCanvas *canvas);
static void IRender_Hide(CCanvas *canvas);
static void IRender_Move(CCanvas *canvas);
static void IRender_Size(CCanvas *canvas);
static void IRender_Draw(CCanvas *canvas);
static void IRender_Reveal(CCanvas *canvas);
static void IRender_Refresh(CCanvas *canvas);

// The IRender Virtual Machine has two main components: The Decoder and the Dispatcher.
// The Dispatcher accepts commands from applications and encodes them as opcodes which
// are added to a queue. The Decoder is executed by the Compositor when it comes time
// to prepare the next frame for display.
// Extending this Virtual Machine is easy and requires implementing the operation then
// adding a function pointer to the array in the Decoder and the number of arguments
// to the array in the Dispatcher.
static bool IRender_Decoder(CCanvas *canvas) {
    static void (*opcode[])(CCanvas *canvas) = {
        IRender_Nop,    IRender_Push,   IRender_Pop,    IRender_Show,
        IRender_Hide,   IRender_Move,   IRender_Size,   IRender_Draw,
        IRender_Reveal, IRender_Refresh
    };
    if (!canvas) return false;
    while (canvas->Head != canvas->Tail) {
        u32 op = canvas->Ring[canvas->Head++];
        if (op >= elementsof(opcode)) {
            canvas->Head = canvas->Tail;
            return false;
        }
        opcode[op](canvas);
    }
    canvas->Head = canvas->Tail;
    return true;
}

static bool IRender_Dispatch(CCanvas *canvas, u32 op, ...) {
    static u32 opcode[] = {
        RENDER_NUM_NOP,     RENDER_NUM_PUSH,    RENDER_NUM_POP,     RENDER_NUM_SHOW,
        RENDER_NUM_HIDE,    RENDER_NUM_MOVE,    RENDER_NUM_SIZE,    RENDER_NUM_DRAW,
        RENDER_NUM_REVEAL,  RENDER_NUM_REFRESH
    };
    if (op >= elementsof(opcode)) return false;
    int *x = &op;
    u8 tail = canvas->Tail;
    canvas->Ring[tail++] = *x++;
    for (u32 n = 0; n < opcode[op]; n++) {
        canvas->Ring[tail++] = *x++;
        if (tail == canvas->Head) return false;
    }
    canvas->Tail = tail;
    return true;
}

static void IRender_Nop(CCanvas *canvas) {
}

static void IRender_Push(CCanvas *canvas) {
    CWindow *item = (CWindow*)canvas->Ring[canvas->Head++];
    CWindow *head = canvas->TopMost;
    item->Prev = nullptr;
    item->Next = head;
    if (head) head->Prev = item;
    canvas->TopMost = item;
    item->Dirty = 2;
}

static void IRender_Pop(CCanvas *canvas) {
    CWindow *item = (CWindow*)canvas->Ring[canvas->Head++];
    CWindow *head = canvas->TopMost;
    CWindow *tail = canvas->BtmMost;
    CWindow *prev = item->Prev;
    CWindow *next = item->Next;

    if (prev) prev->Next = next;
    if (next) next->Prev = prev;
    if (head == item) canvas->TopMost = next;
    if (tail == item) canvas->BtmMost = prev;
    item->Prev = nullptr;
    item->Next = nullptr;

    int x1 = item->PosX, x2 = item->PosX + item->Wide;
    int y1 = item->PosY, y2 = item->PosY + item->High;
    for (CWindow *list = next; list; list = list->Next) {
        list->Dirty = 2;
    }

    if (canvas->Focus == item) {
        CEvent evt;
        stosd(&evt, 0, sizeof(CEvent)/4);
        evt.Code = EVT_ONFOCUSLOST;
        evt.Target = canvas->Focus;
        Event->RaiseEvent(&evt);

        canvas->Focus = next;
        evt.Code = EVT_ONFOCUS;
        evt.Target = canvas->Focus;
        Event->RaiseEvent(&evt);
    }
}

static void IRender_Show(CCanvas *canvas) {
    CWindow *item = (CWindow*)canvas->Ring[canvas->Head++];
    item->Visible = true;
    item->Dirty = 2;
}

static void IRender_Hide(CCanvas *canvas) {
    CWindow *item = (CWindow*)canvas->Ring[canvas->Head++];
    item->Visible = false;
    item->Dirty = 2;
}

static void IRender_Move(CCanvas *canvas) {
    CWindow *item = (CWindow*)canvas->Ring[canvas->Head++];
    item->PosX = (int)canvas->Ring[canvas->Head++];
    item->PosY = (int)canvas->Ring[canvas->Head++];
    item->Dirty = 2;
}

static void IRender_Size(CCanvas *canvas) {
    CWindow *item = (CWindow*)canvas->Ring[canvas->Head++];
    item->Wide = (int)canvas->Ring[canvas->Head++];
    item->High = (int)canvas->Ring[canvas->Head++];
    item->Dirty = 2;
}

static void IRender_Draw(CCanvas *canvas) {
    CWindow *item = (CWindow*)canvas->Ring[canvas->Head++];
    item->Dirty = 2;
}

static void IRender_Reveal(CCanvas *canvas) {
    int x1 = (int)canvas->Ring[canvas->Head++];
    int y1 = (int)canvas->Ring[canvas->Head++];
    int x2 = (int)canvas->Ring[canvas->Head++];
    int y2 = (int)canvas->Ring[canvas->Head++];
    for (CWindow *list = canvas->TopMost; list; list++) {
        if (list->PosY >= y1 && list->PosY + list->High < y2) {
            if (list->PosX >= x1 && list->PosX + list->Wide < x2) {
                list->Dirty = 2;
            }
        }
    }
}

static void IRender_Refresh(CCanvas *canvas) {
    for (CWindow *list = canvas->TopMost; list; list++) {
        list->Dirty = 2;
    }
}

static IRender render = {
    IRender_Dispatch,
    IRender_Decoder
};

export IRender *Render = &render;