#include <Kernel.h>

extern IDevice *Device;
extern IVesa *Vesa;
extern IPipe *Pipe;

void Ps2Send(u8 value);
u8 Ps2Read(void);

export int MOUSE_SPEED   = 66;
export int MOUSE_ASSIST  = 1;
export int MOUSE_FORMAT  = 0 + 2;
export int MOUSE_WHEEL   = 0;
export int MOUSE_BUTTONS = 3;

static CPipeAsync *pipe;
static u8 mbuf[4];
static u8 mpos;
export CMouse ms[3];
static int velocity = 0;

int IMouse_GetSpeed(void) {
    return MOUSE_SPEED;
}

bool IMouse_SetSpeed(int speed) {
    if (speed <= 5) return false;
    MOUSE_SPEED = speed;
    return true;
}

void ActiveAssist(int adj, int opp) {
    int hyp = pythagoras(adj, opp);
    int log2  = ipow(hyp,2);
    int log10 = ipow(hyp,10);
    velocity = log10 ? velocity/2 + log2*hyp/log10 : 0;
    int speed = (velocity) ? MAX(MOUSE_SPEED*velocity/128, 1) : 0;
    ms[1].PosX = ms[2].PosX + (hyp ? speed*adj/hyp : 0);
    ms[1].PosY = ms[2].PosY + (hyp ? speed*opp/hyp : 0);
}

void TranslateMouse(int adj, int opp) {
    movsd(&ms[2], &ms[1], sizeof(CMouse)/4); // old = new
    movsd(&ms[1], &ms[0], sizeof(CMouse)/4); // new = cur
    if (MOUSE_ASSIST) ActiveAssist(adj, opp);
    else {
        ms[1].PosX = ms[2].PosX + adj;
        ms[1].PosY = ms[2].PosY + opp;
    }
    ms[1].PosX = MIN(MAX(0, ms[1].PosX), Vesa->ResX()-1);
    ms[1].PosY = MIN(MAX(0, ms[1].PosY), Vesa->ResY()-1);
}

static int count = 0;
u32 MouseIsr(u32 err, u32 esp) {
    //char round = (MOUSE_FORMAT & 4) ? 4 : 3;
    char round = MOUSE_BUTTONS + MOUSE_WHEEL;

    mbuf[mpos = mpos % round] = inb(PS2_DATA);

    mpos = ++mpos % round;
    if (mpos == 0) {
        int status = mbuf[0];
        if (status & 0x08) {
            int x = (unsigned char)mbuf[1];
            int y = (unsigned char)mbuf[2];
            int w = (signed char)mbuf[3];
            if (MOUSE_WHEEL == 0) w = 0;

            if (MOUSE_FORMAT & 1) {
                if (status & 0x10) x |= 0xFFFFFE00;
                if (status & 0x20) y |= 0xFFFFFE00;
                if (status & 0x40) x |= 0x100;
                if (status & 0x80) y |= 0x100;
            } else {
                if (status & 0x10) x |= 0xFFFFFF00;
                if (status & 0x20) y |= 0xFFFFFF00;
            }

            ms[0].DeltaX = (w) ? 0 : +x;
            ms[0].DeltaY = (w) ? 0 : -y;
            ms[0].Wheel  = w;
            ms[0].Left   = (status & 1);
            ms[0].Right  = (status & 2)/2;
            ms[0].Middle = (status & 4)/4;

            /*
            Logger("\n [info] Mouse [");
            for (int i = 0; i < round; i++) Logger(" %X ", mbuf[i]);
            Logger(" ]\n");
            Logger("x=%d y=%d w=%d b=%s%s%s\n",
                ms[0].DeltaX,
                ms[0].DeltaY,
                ms[0].Wheel,
                ms[0].Left   ? "L" : "-",
                ms[0].Middle ? "M" : "-",
                ms[0].Right  ? "R" : "-");
            */

            TranslateMouse(ms[0].DeltaX, ms[0].DeltaY);
            Pipe->WriteAsync(pipe, (u8*)&ms[1], sizeof(CMouse));
        } else ++mpos;
    }
    return esp;
}

bool InstallMouse(u8 *base, u32 size) {
    pipe = (CPipeAsync*)base;
    Pipe->CreateAsync(pipe, null, null);
    stosd(&ms, 0, sizeof(ms)/4);
    return true;
}

void IMouse_Flush(void) {
    inb(PS2_STATUS);
    inb(PS2_DATA);
    outb(PIC2_CMD, EOI);
    outb(PIC1_CMD, EOI);
}

bool IMouse_Enable(void) {
    inb(PS2_STATUS);
    inb(PS2_DATA);
    Ps2Send(0xF2); Ps2Read();
	char old = Ps2Read();

    Ps2Send(0xF3); Ps2Read();
    Ps2Send(0xC8); Ps2Read();
    Ps2Send(0xF3); Ps2Read();
    Ps2Send(0x64); Ps2Read();
    Ps2Send(0xF3); Ps2Read();
    Ps2Send(0x50); Ps2Read();
    Ps2Send(0xF2); Ps2Read();
    if (Ps2Read() == 3) MOUSE_WHEEL = 1;
    if (MOUSE_WHEEL) Logger(" Mouse Wheel Detected\n");
    MOUSE_BUTTONS = 3;

    ms[2].PosX = ms[1].PosX = ms[0].PosX = Vesa->ResX()/2;
    ms[2].PosY = ms[1].PosY = ms[0].PosY = Vesa->ResY()/2;
    return true;
}

bool EnableMouse(u8 *base, u32 size) {
    IMouse_Enable();
    Device->Latch(IRQ_MOUSE, MouseIsr);
    return true;
}


static IMouse imouse = {
    IMouse_Enable,
    IMouse_Flush,
    IMouse_GetSpeed,
    IMouse_SetSpeed
};

export IMouse *Mouse = &imouse;