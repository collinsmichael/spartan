#include <Kernel.h>
#include <Shell.h>

void Ps2Command(char cmd, char val);

extern IDevice *Device;
extern IFileIO *FileIO;
extern IPipe   *Pipe;

export bool KEYMAP = 0x01;

static CPipeAsync *pipe;
static u8 led;
static u8 numlock;
static u8 caplock;
static u8 scrlock;
static u8 ctrl;
static u8 shift;
static u8 alt;
static u8 win;
static u8 prnt;
static u8 *filter; // preprocess scan code to produce 7-bit number
static u8 *lookup; // distinguished numeric/alphabet/grammar symbols
static u8 *qwerty; // maps scan codes to qwerty keyboard layout ascii
static u8 *dvorak; // maps scan codes to dvorak keyboard layout ascii
static u8 keybrd[256];

#define NUMBER  0x01
#define UPPER   0x02
#define LOWER   0x04
#define HEXNUM  0x08
#define GRAMMAR 0x10
#define PARENS  0x20
#define NEWLINE 0x40
#define SPACE   0x80

bool IKeyboard_SetLed(int toggle) {
    Ps2Command(0xED, led = toggle & 7);
    return true;
}

void IKeyboard_Flush(void) {
    if (~inb(PS2_STATUS) & PS2_DATA_FULL) return;
    char scan = inb(PS2_DATA);
    if (scan != 0xE0) return;
    for (int i = 0; i < 1*KHz; i++) {
        if (inb(PS2_DATA) != 0xE0) break;
    }
}

bool IKeyboard_KeyDown(int scancode) {
    return true;
}

int IKeyboard_GetLed(void) {
    return led;
}


char *IKeyboard_GetKeyMap(void) {
    if (KEYMAP == 0x01) return "Qwerty";
    if (KEYMAP == 0x02) return "Dvorak";
    return "Not Set";
}

bool IKeyboard_SetKeyMap(char *layout) {
    if (StrCmp(layout, "Qwerty") == 0) KEYMAP = 0x01;
    if (StrCmp(layout, "Dvorak") == 0) KEYMAP = 0x02;
    return true;
}

static u32 KeyboardIsr(u32 err, u32 esp) {
    static u8 buf[4];
    buf[3] = buf[2];
    buf[2] = buf[1];
    buf[1] = buf[0];
    buf[0] = inb(PS2_DATA);

    switch (buf[0]) {
    case 0xFA: outb(PS2_DATA, 0xFA);
    case 0xE0:
    case 0xE1: return esp;
    }
    if (buf[1] == 0xE1) return esp;

    u8 scan  = buf[0];
    u8 down  = (scan & 0x80) ? 0 : 1;
    u8 key   = (scan & 0x7F);
    if (buf[1] == 0xE0) key += 0x80;
    if (buf[2] == 0xE1) key  = 0xC0;

    u8 toggle = led;
    if (down) {
        if (key == 0x3A) toggle ^= PS2_CAPLOCK;
        if (key == 0x45) toggle ^= PS2_NUMLOCK;
        if (key == 0x46) toggle ^= PS2_SCRLOCK;
        if (led != toggle) IKeyboard_SetLed(led = toggle);
    }
    key = filter[key];
    keybrd[key] = down;

    keybrd[KEY_NUMLOCK]  = (toggle & PS2_NUMLOCK) ? 1 : 0;
    keybrd[KEY_CAPSLOCK] = (toggle & PS2_CAPLOCK) ? 1 : 0;
    keybrd[KEY_SCRLLOCK] = (toggle & PS2_SCRLOCK) ? 1 : 0;

    numlock = keybrd[KEY_NUMLOCK ];
    caplock = keybrd[KEY_CAPSLOCK];
    scrlock = keybrd[KEY_SCRLLOCK];
    ctrl    = keybrd[KEY_LCTRL ] | keybrd[KEY_RCTRL ];
    shift   = keybrd[KEY_LSHIFT] | keybrd[KEY_RSHIFT];
    alt     = keybrd[KEY_LALT  ] | keybrd[KEY_RALT  ];
    win     = keybrd[KEY_LWIN  ] | keybrd[KEY_RWIN  ];
    prnt    = keybrd[KEY_PRINT ];

    u8 *keymap = (KEYMAP == 0x01) ? qwerty : dvorak;
    char ascii = keymap[key + shift*128 + numlock*256];
    if (caplock && (lookup[ascii] & (UPPER|LOWER))) {
        ascii = keymap[key + (1-shift)*128 + numlock*256];
    }

    //Debug(" [KBD] S=%X K=%X A=%X (%c)\n", scan, key, ascii, ascii);
    u8 packet[8];
    packet[0] = scan;
    packet[1] = down;
    packet[2] = key;
    packet[3] = ascii;
    packet[4] = ctrl;
    packet[5] = shift;
    packet[6] = alt;
    packet[7] = led;
    Pipe->WriteAsync(pipe, packet, 8);
    return esp;
}

bool InstallKeyboard(u8 *base, u32 size) {
    pipe = (CPipeAsync*)base;
    Pipe->CreateAsync(pipe, null, null);

    stosd(keybrd, 0, 256/4);
    pipe->Capacity -= 256;
    pipe->Pointer += 256;

    CFile *file = 0;
    file = FileIO->Open("Config/KeyMaps/Filter.bin", "-r--");
    if (!file) {
        Logger(" [FAIL] FileIO->Open(Config/KeyMaps/Filter.bin, -r--);");
        return false;
    }
    filter = FileIO->Base(file);
    FileIO->Close(file);

    file = FileIO->Open("Config/KeyMaps/LookUp.bin", "-r--");
    if (!file) {
        Logger(" [FAIL] FileIO->Open(Config/KeyMaps/LookUp.bin, -r--);");
        return false;
    }
    lookup = FileIO->Base(file);
    FileIO->Close(file);

    file = FileIO->Open("Config/KeyMaps/Qwerty.bin", "-r--");
    if (!file) {
        Logger(" [FAIL] FileIO->Open(Config/KeyMaps/Qwerty.bin, -r--);");
        return false;
    }
    qwerty = FileIO->Base(file);
    FileIO->Close(file);

    file = FileIO->Open("Config/KeyMaps/Dvorak.bin", "-r--");
    if (!file) {
        Logger(" [FAIL] FileIO->Open(Config/KeyMaps/Dvorak.bin, -r--);");
        return false;
    }
    dvorak = FileIO->Base(file);
    FileIO->Close(file);
    return true;
}

bool EnableKeyboard(u8 *base, u32 size) {
    Device->Latch(IRQ_KBD, KeyboardIsr);
    return true;
}

static IKeyboard keyboard = {
    IKeyboard_Flush,
    IKeyboard_KeyDown,
    IKeyboard_GetLed,
    IKeyboard_SetLed,
    IKeyboard_GetKeyMap,
    IKeyboard_SetKeyMap

};

export IKeyboard *Keyboard = &keyboard;