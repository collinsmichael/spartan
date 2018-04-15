#include <Kernel.h>
#include <Shell.h>

import IMemory   *Memory;
import IThread   *Thread;
import IProcess  *Process;
import INetwork  *Network;
import IBitmap   *Bitmap;
import IFont     *Font;
import IGraphics *Graphics;
import IEvent    *Event;
import IWindow   *Window;
import IWidget   *Widget;
import IListView *ListView;
import INoteBox  *NoteBox;

static int count;
static char **item;
static char  *text;
static char nick[256] = "nick";
static char body[256] = "message";

static int avatar;
static CBitmap icons[5];
static CBitmap tinys[5];
static CWindow *port;
static CWindow *name;
static CWindow *chat;
static CWindow *icon;
static CWindow *note;
static CWindow *send;

static char *headers[4] = { "", "Name", "Comment" };
static int colsize[4];
static int hook;

#define ID_NAME 0x101
#define ID_PORT 0x102
#define ID_CHAT 0x103
#define ID_ICON 0x104
#define ID_NOTE 0x105
#define ID_SEND 0x106

#define NICK_SIZE (8)
#define NOTE_SIZE (62-NICK_SIZE)

bool Insert(int avatar, char *nick, char *body) {
    char **msg = &item[count*4];
    msg[0] = (char*)&tinys[avatar];
    msg[1] = (char*)&text[count*64 + 0 ];
    msg[2] = (char*)&text[count*64 + 16];
    movsd(&text[64*count +  0], nick, 16/4);
    movsd(&text[64*count + 16], body, 48/4);
    ListView->Insert(chat, msg);
    count = (++count) % 256;

    static CEvent draw;
    draw.Target = chat;
    draw.Code = EVT_ONDRAW;
    Event->Forward(chat, &draw);
    return true;
}

bool Recv(u8 *packet, int size) {
    int pos = 0;
    int avatar = packet[pos++];
    char *nick = &packet[pos];
    char *body = nullptr;

    for (int i = 0; i < size; i++)  {
        if (packet[pos] == 0) {
            if (!body) body = &packet[pos+1];
        }
        if (packet[pos] == 0xFF) {
            break;
        }
        pos++;
    }
    if (body == nullptr || packet[pos] != 0xFF) return false;
    Insert(avatar, nick, body);
    return true;
}


static bool ICursor_Hook(CPipe *pipe) {
    static int tail;
    static u8 recv[256];
    for (u32 i = 0; i < pipe->Consumed; i++) {
        u8 c = recv[tail++] = pipe->Pointer[(pipe->Head + i) % pipe->Capacity];
        if (c == 0xFF) {
            Recv(recv, tail);
            tail = 0;
        }
    }
    pipe->Head = pipe->Tail;
    pipe->Consumed = 0;
    return true;
}

bool Send(int port, int avatar, char *nick, char *body) {
    if (port <= 0 || port > 255) return false;
    if (!nick || !body) return false;

    static char packet[256];
    int pos = 0;
    packet[pos++] = avatar;
    for (int i = 0; i < NICK_SIZE; i++) {
        if (!nick[i]) break;
        packet[pos++] = nick[i];
    }
    packet[pos++] = 0;

    for (int i = 0; pos < 63; i++) {
        if (!body[i]) break;
        packet[pos++] = body[i];
    }
    packet[pos++] = 0;
    packet[pos++] = 0xFF;

    if (hook != port) {
        Network->Unhook(hook, ICursor_Hook);
        hook = port;
        Network->Hook(hook, ICursor_Hook);
    }

    if (!Network->Send(port, packet, pos)) return false;
    return true;
}

bool ChatOnLoad(CWindow *win, CEvent *evt) {
    CWindow *parent = win->Parent;
    parent->Title = "Chat";
    Window->SetTheme(win, "Light");
    win->ForeGround[0] = 0x001F1F1F;

    if (!Bitmap->LoadSprite(&icons[0], "Apps/Chat/Chat.bmp")) {
        return false;
    }
    for (int i = 1; i < elementsof(icons); i++) {
        static char path[64];
        Format(path, "Apps/Chat/User%d.bmp", i);
        if (!Bitmap->LoadSprite(&icons[i], path)) {
            return false;
        }
        Format(path, "Apps/Chat/Tiny%d.bmp", i);
        if (!Bitmap->LoadSprite(&tinys[i], path)) {
            return false;
        }
    }

    name = Widget->NoteBox(win);
    if (!name) return false;
    name->WindowID = ID_NAME;
    NoteBox->SetSize(name, NICK_SIZE);

    port = Widget->NoteBox(win);
    if (!port) return false;
    port->WindowID = ID_PORT;
    NoteBox->SetNumeric(port, true);
    NoteBox->SetSize(port, 3);
    port->MinValue = 0;
    port->MaxValue = 255;

    chat = Widget->ListView(win);
    if (!chat) return false;
    chat->WindowID = ID_CHAT;

    icon = Widget->Button(win);
    if (!icon) return false;
    icon->WindowID = ID_ICON;
    icon->Bitmap = &icons[avatar = 1];

    note = Widget->NoteBox(win);
    if (!note) return false;
    note->WindowID = ID_NOTE;
    NoteBox->SetSize(note, NOTE_SIZE);
    
    send = Widget->Button(win);
    if (!send) return false;
    send->WindowID = ID_SEND;
    send->Bitmap = &icons[0];

    CListView *list = chat->CustomData;
    if (!list) return false;

    list->NumCols  = elementsof(headers);
    list->Headers  = headers;
    list->ColSize  = colsize;
    list->RowSize  = 24;
    list->HeadSize = 24;

    colsize[0] = 24;
    colsize[1] = 96;
    colsize[2] = win->Wide - (24+96+32);
    return true;
}

bool ChatOnDraw(CWindow *win, CEvent *evt) {
    CArea area;
    int x1 = area.X1 = win->PosX;
    int y1 = area.Y1 = win->PosY;
    int x2 = area.X2 = win->PosX + win->Wide;
    int y2 = area.Y2 = win->PosY + 24;

    CFont *font = win->Font;
    int lut = win->Lut;
    int bg = win->Clut[lut*8 + COLOUR_BORDER];
    int fg = win->Clut[lut*8 + COLOUR_FOREGROUND];
    int v1 = y1 - font->FontH/2 + 24/2;

    int u1 = x1;
    int u2 = u1 + font->FontW/2;
    int u3 = u1 + 64;
    Graphics->Poly4fClip(&area, u1,   y1,   u3,   y2,  bg);
    Graphics->Poly4fClip(&area, u1+1, y1+1, u3-1, y2-1, fg);
    Window->TextOut(win, u2, v1, "%s", "nick");

    int u4 = x1 + win->Wide/2;
    int u5 = u4 + font->FontW/2;
    int u6 = u4 + 64;
    Graphics->Poly4fClip(&area, u4,   y1,   u6,   y2,  bg);
    Graphics->Poly4fClip(&area, u4+1, y1+1, u6-1, y2-1, fg);
    Window->TextOut(win, u5, v1, "%s", "port");
    return true;
}

bool ChatOnSize(CWindow *win, CEvent *evt) {
    CWindow *parent = win->Parent;
    win->PosX = 7;
    win->PosY = 32;
    win->Wide = parent->Wide - 14;
    win->High = parent->High - 38;

    colsize[0] = 24;
    colsize[1] = 96;
    colsize[2] = win->Wide - (24+96+32);

    Window->Resize(name, win->PosX + 64, win->PosY, win->Wide/2 - 64, 24);
    Window->Resize(port, win->PosX + win->Wide/2 + 64, win->PosY, win->Wide/2 - 64, 24);
    Window->Resize(chat, win->PosX, win->PosY + 25, win->Wide, win->High - 72);
    Window->Resize(icon, win->PosX, win->PosY + win->High - 47, 48, 48);
    Window->Resize(note, win->PosX + 48, win->PosY + win->High - 47, win->Wide - 96, 48);
    Window->Resize(send, win->PosX + win->Wide - 48, win->PosY + win->High - 47, 48, 48);
    return true;
}

bool ChatOnAction(CWindow *win, CEvent *evt) {
    CWindow *child = evt->Source;
    if (!child) return false;

    if (child->WindowID == ID_ICON) {
        avatar = (++avatar) % elementsof(icons);
        if (avatar == 0) avatar++;
        child->Bitmap = &icons[avatar];

        CEvent draw;
        draw.Source = win;
        draw.Target = child;
        draw.Code = EVT_ONDRAW;
        Event->Forward(child, &draw);
    }

    if (child->WindowID == ID_SEND || child->WindowID == ID_NOTE) {
        char *data = NoteBox->GetText(name);
        if (data) movsb(nick, data, 256);

        char *text = NoteBox->GetText(note);
        if (text) movsb(body, text, 256);
        
        Send(port->CurValue, avatar, nick, body);
        NoteBox->ClearText(note);
        Insert(avatar, nick, body);

        static CEvent draw;
        draw.Target = note;
        draw.Code = EVT_ONDRAW;
        Event->Forward(note, &draw);
    }
    return true;
}

bool ChatOnFocus(CWindow *win, CEvent *evt) {
    return true;
}

bool ChatProc(CWindow *win, CEvent *evt) {
    switch (evt->Code) {
    case EVT_ONLOAD:   return ChatOnLoad(win, evt);
    case EVT_ONDRAW:   return ChatOnDraw(win, evt);
    case EVT_ONSIZE:   return ChatOnSize(win, evt);
    case EVT_ONACTION: return ChatOnAction(win, evt);
    case EVT_ONFOCUS:  return ChatOnFocus(win, evt);
    }
    return true;
}

int main(void) {
    item = (char**)Memory->Alloc(256*4);
    text = (char*)Memory->Alloc(256*64);
    CWindow *frame = Window->Create(nullptr, ChatProc);
    if (!frame) return -1;

    for (CEvent evt;;) {
        if (Event->GetEvent(&evt)) {
            Event->HandleEvent(&evt);
        }
    }
    return 0;
}

void start(void) {
    Thread->Exit(main());
}