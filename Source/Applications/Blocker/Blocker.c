#include <Kernel.h>
#include <Shell.h>

import IThread   *Thread;
import IProcess  *Process;
import IPool     *Pool;
import IWindow   *Window;
import IWidget   *Widget;
import IGraphics *Graphics;
import IBitmap   *Bitmap;
import ICanvas   *Canvas;
import IEvent    *Event;

CBitmap  light;
CBitmap  dark;
CBitmap  red;
CBitmap  blue;
CWindow *window;
CWindow *button[5*5];
int sx;
int sy;
int dx;
int dy;
int step;
char player;
char board [5*5*4];
char weight[5*5*4];
char other [5*5*4];

int OnDraw(CWindow *win, CEvent *evt) {
    if (!win->Visible) return true;
    for (int y = 0; y < 5; y++) {
    for (int x = 0; x < 5; x++) {
        CWindow *child = button[y*5 + x];
        if (child) {
            CBitmap *bitmap = ((x ^ y) & 1) ? &dark : &light;
            if (board[y*5 + x] == 'o') bitmap = &blue;
            if (board[y*5 + x] == 'x') bitmap = &red;
            child->Bitmap = bitmap;
        }
    }
    }
    return true;
}

void draw(char *board) {
    Window->Redraw(window);
    for (int y = 0; y < 5; y++)
    for (int x = 0; x < 5; x++) {
        Window->Redraw(button[y*5 + x]);
    }
}

// count the number of moves available
int moves(char *state, char p) {
    int val = 0;
    for (int y = 0; y < 5; y++)
    for (int x = 0; x < 5; x++) {
        if (state[y*5+x] == p) {
            if (x > 0 && y > 0) val += (state[(y-1)*5+(x-1)] == '+') ? 1 : 0;
            if (x > 0 && y < 4) val += (state[(y+1)*5+(x-1)] == '+') ? 1 : 0;
            if (x < 4 && y > 0) val += (state[(y-1)*5+(x+1)] == '+') ? 1 : 0;
            if (x < 4 && y < 4) val += (state[(y+1)*5+(x+1)] == '+') ? 1 : 0;
        }
    }
    return val;
}

// calculate the score of the position
int score(char *state, char p) {
    int val = 0;
    for (int y = 0; y < 5; y++)
    for (int x = 0; x < 5; x++) {
        if (state[y*5+x] == p) {
            if (p == 'x') val += (int)(weight[y*5+x]-'0');
            else val += (int)(other[y*5+x]-'0');
        }
    }
    return val;
}

bool valid(char *state, int x1, int y1, int x2, int y2, char p) {
    if (x1 < 0 || x1 >= 5) return false; // out of bounds
    if (x2 < 0 || x2 >= 5) return false; // out of bounds
    if (y1 < 0 || y1 >= 5) return false; // out of bounds
    if (y2 < 0 || y2 >= 5) return false; // out of bounds

    if (ABS(x2-x1) != 1) return false; // only move one square diagonally
    if (ABS(y2-y1) != 1) return false; // only move one square diagonally
    if (state[y1*5 + x1] !=  p ) return false; // only move your own pieces
    if (state[y2*5 + x2] != '+') return false; // only move to vacant squares
    return true; // valid move
}

int random(void) {
    u64 ts64 = rdtsc();
    u32 ts32 = (u32)(ts64 ^ (ts64 >> 32));
    u16 ts16 = (u16)(ts32 ^ (ts32 >> 16));
    u8  ts8  = (u8 )(ts16 ^ (ts16 >>  8));
    return ts8;
}

// computer move
void think() {
    char p = (player & 1) ? 'o' : 'x';
    char q = (player & 1) ? 'x' : 'o';
    if (score(board, p) == 9*4) return;

    int best   = -1024;
    int bestsx = -1;
    int bestsy = -1;
    int bestdx = -1;
    int bestdy = -1;
    for (int y = 0; y < 5; y++)
    for (int x = 0; x < 5; x++) {
        if (!((x ^ y) & 1)) continue;
        if (board[y*5+x] == p) {
            for (int v = y-1; v <= y+1; v += 2)
            for (int u = x-1; u <= x+1; u += 2) {
                if (v < 0 || v >= 5) continue;
                if (u < 0 || u >= 5) continue;
                char state[32];
                movsb(state, board, 5*5);
                if (state[v*5+u] != '+') continue;
                if (!valid(state, x,y,u,v,p)) continue;

                state[y*5+x] = '+';
                state[v*5+u] = p;
                int val = score(state, p);
                if (moves(state, q) == 0) {
                    best = 1024*1024;
                    bestsx = x; bestsy = y;
                    bestdx = u; bestdy = v;
                }
                if (val > best) {
                    best = val;
                    bestsx = x; bestsy = y;
                    bestdx = u; bestdy = v;
                }
                if (val == best && random() > 128) {
                    best = val;
                    bestsx = x; bestsy = y;
                    bestdx = u; bestdy = v;
                }
            }
        }
    }
    board[bestsy*5+bestsx] = '+';
    board[bestdy*5+bestdx] = p;
    draw(board);
    player++;
}

// player move
void move(int x, int y) {
    if ((x ^ y) & 1) {
        char p = (player & 1) ? 'o' : 'x';
        Debug(" move(%d %d %c %c)\n", x, y, p, board[y*5 + x]);
        if (board[y*5 + x] == p) {
            sx = x;
            sy = y;
            step++;
        }
        if (step == 1 && board[y*5 + x] == '+') {
            dx = x;
            dy = y;
            step++;
        }
        if (step == 2) {
            if (valid(board, sx, sy, dx, dy, p)) {
                board[sy*5+sx] = '+';
                board[dy*5+dx] = p;
                player++;
                if (score(board, p) != 9*4) {
                    think();
                }
            }
            step = 0;
        }
    }
    draw(board);
}


int OnAction(CWindow *win, CEvent *evt) {
    CWindow *child = evt->Source;
    if (!child) return true;
    int id = child->WindowID - 0x7000;
    if (id < 0 || id >= 25) return true;

    int y = id / 5;
    int x = id % 5;
    move(x, y);
    return true;
}

int OnReset(CWindow *win, CEvent *evt) {
    Window->Redraw(window->Parent);
    movsb(board,  "+o+o+" "o+++o" "+++++" "x+++x" "+x+x+", 5*5);
    movsb(weight, "09090" "90709" "05050" "10301" "01010", 5*5);
    movsb(other,  "01010" "10301" "05050" "90709" "09090", 5*5);
    player = 0;
    step   = 0;
    player = 0;
    step   = 0;
    return true;
}

int OnLoad(CWindow *win, CEvent *evt) {
    CCanvas *canvas = win->Canvas;
    if (!canvas) return false;
    CWindow *frame = canvas->BtmMost;
    if (!frame) return false;
    frame->Title = "Calc";
    window = win;

    if (!Bitmap->LoadSprite(&light, "Apps/Blocker/Light.bmp")) return false;
    if (!Bitmap->LoadSprite(&dark,  "Apps/Blocker/Dark.bmp"))  return false;
    if (!Bitmap->LoadSprite(&red,   "Apps/Blocker/Red.bmp"))   return false;
    if (!Bitmap->LoadSprite(&blue,  "Apps/Blocker/Blue.bmp"))  return false;

    int w = light.ResX*5 + 16 + 4;
    int h = light.ResY*5 + 40 + 4;
    int x = (canvas->ResX)/2 - w/2;
    int y = (canvas->ResY)/2 - h/2;
    Window->Resize(frame, frame->PosX, frame->PosY, w, h);

    for (int y = 0; y < 5; y++) {
    for (int x = 0; x < 5; x++) {
        CWindow *child = Widget->Button(win);
        if (!child) return false;
        button[y*5 + x] = child;
        child->WindowID = 0x7000 + y*5 + x;
        child->Bitmap = ((x ^ y) & 1) ? &dark : &light;
        child->Wide = child->Bitmap->ResX;
        child->High = child->Bitmap->ResY;
        child->PosX = win->PosX + x*child->Bitmap->ResX + 2;
        child->PosY = win->PosY + y*child->Bitmap->ResY + 2;
    }
    }

    OnReset(win, evt);
    return true;
}

int BlockerProc(CWindow *win, CEvent *evt) {
    switch (evt->Code) {
    case EVT_ONLOAD:   return OnLoad(win, evt);
    case EVT_ONDRAW:   return OnDraw(win, evt);
    case EVT_ONACTION: return OnAction(win, evt);
    }
    return true;
}

int main(void) {
    CWindow *frame = Window->Create(nullptr, BlockerProc);
    if (!frame) return -1;

    static CEvent evt;
    for (;;) {
        if (Event->GetEvent(&evt)) {
            //Event->DebugEvent(&evt);
            Event->HandleEvent(&evt);
        }
    }
    return 0;
}

void start(void) {
    Process->Exit(main());
}