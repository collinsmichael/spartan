#include <Kernel.h>
#include <Shell.h>

extern ICanvas   *Canvas;
extern IGraphics *Graphics;
extern IFont     *Font;

static bool DrawHeader(CWindow *win, CListView *data) {
    CCanvas *canvas = win->Canvas;
    CFont *font = win->Font;
    if (!win || !data || !canvas || !font) return false;

    int *colsize = data->ColSize;
    char **headers = data->Headers;
    if (!colsize || !headers) return false;

    int x1 = win->PosX;
    int y1 = win->PosY;
    int x2 = win->PosX + win->Wide;
    int y2 = win->PosY + win->High;
    int hy = MIN(MAX(y1, y1 + data->HeadSize), y2);
    Graphics->Poly4f(x1, y1, x2, hy, win->BackGround[1]);
    Graphics->Poly2h(hy-1, x1, x2, win->ForeGround[1]);
    Graphics->Poly2h(y1,   x1, x2, win->ForeGround[1]);

    int x = x1 + colsize[0];
    for (int col = 1; col < data->NumCols; col++) {
        int rgb = win->ForeGround[1];
        Graphics->Poly2v(x, y1, hy, rgb);
        x += colsize[col];
    }

    int u = x1;
    for (int col = 0; col < data->NumCols; col++) {
        int fg = win->ForeGround[1];
        char *text = headers[col];
        for (int tx = font->FontW, i = 0; tx < colsize[col]; tx += font->FontW, i++) {
            if (!text[i]) break;
            int ty = (y1 + hy)/2 - font->FontH/2;
            Font->PutC(font, canvas, u + tx, ty, fg, text[i]);
        }
        u += colsize[col];
    }
    return true;
}

static bool DrawBackGround(CWindow *win, CListView *data) {
    if (!win || !data) return false;
    int *colsize = data->ColSize;
    if (!colsize) return false;

    int x1 = win->PosX;
    int y1 = win->PosY + data->HeadSize;
    int x2 = win->PosX + win->Wide;
    int y2 = win->PosY + win->High;
    y1 = MIN(MAX(win->PosY, y1), win->PosY + win->High);
    y2 = MIN(MAX(win->PosY, y2), win->PosY + win->High);
    Graphics->Poly4f(x1, y1, x2, y2, win->BackGround[0]);
    return true;
}

static bool DrawRow(CWindow *win, CListView *data, int row) {
    if (!win || !data) return false;
    if (row < 0 || row >= data->NumRows) return false;

    CCanvas *canvas = win->Canvas;
    CFont *font = win->Font;
    if (!canvas || !font) return false;

    int *colsize = data->ColSize;
    if (!colsize) return false;

    CArea area;
    area.X1 = win->PosX;
    area.Y1 = win->PosY + data->HeadSize;
    area.X2 = win->PosX + win->Wide;
    area.Y2 = win->PosY + win->High;

    int x1 = win->PosX;
    int y1 = win->PosY + data->HeadSize;
    int x2 = win->PosX + win->Wide;
    int y2 = win->PosY + data->HeadSize + data->RowSize;
    y1 += row*data->RowSize - data->Scroll;
    y2 += row*data->RowSize - data->Scroll;

    if (y2 < win->PosY) return 0;
    if (y1 >= win->PosY + win->High) return 0;
    int v1 = MIN(MAX(win->PosY, y1), win->PosY + win->High);
    int v2 = MIN(MAX(win->PosY, y2), win->PosY + win->High);

    int fg = win->ForeGround[0];
    int bg = win->BackGround[0];
    if (row == data->Hover) {
        bg = win->BackGround[2];
    }
    if (row == data->Selected) {
        bg = win->BackGround[3];
    }

    int u = x1;
    Graphics->Poly4f(x1, v1, x2, v2, bg);
    Graphics->Poly2h(v1, x1, x2, fg);
    for (int c = 0; c < data->NumCols; c++) {
        Graphics->Poly2v(u, v1, v2, fg);
        u += colsize[c];
    }
    Graphics->Poly2h(v2, x1, x2, fg);

    char **item = (char**)data->Items[row];
    if (!item) return true;

    CBitmap *bitmap = (CBitmap*)item[0];
    if (bitmap) {
        int w = MIN(MAX(x1, x1 + bitmap->ResX), x1 + colsize[0]);
        int h = MIN(MAX(y1, y1 + bitmap->ResY), y1 + data->RowSize);
        w = MIN(MAX(win->PosX, w), win->PosX + win->Wide);
        h = MIN(MAX(win->PosY, h), win->PosY + win->High);
        Graphics->Poly4tClip(&area, x1, y1, w, h, bitmap);
    }

    u = x1 + colsize[0];
    for (int c = 1; c < data->NumCols; c++) {
        int tx = font->FontW;
        int ty = (y1 + y2)/2 - font->FontH/2;
        int fg = win->ForeGround[1];
        char *text = item[c];
        if (!text) continue;
        for (int i = 0; text[i] && tx < colsize[c]; i++) {
            Font->PutCharClip(&area, font, canvas, u + tx, ty, fg, text[i]);
            tx += font->FontW;
        }
        u += colsize[c];
    }
    return true;
}

static bool OnDraw(CWindow *win, CEvent *evt) {
    CCanvas *canvas = win->Canvas;
    CListView *data = win->CustomData;
    CFont *font = win->Font;
    if (!canvas || !data || !font) return false;

    int total = data->NumRows*data->RowSize;
    total = MAX(0, total - data->RowSize);
    data->Scroll = MIN(MAX(0, data->Scroll), total);

    DrawBackGround(win, data);

    int y1 = data->Scroll/data->RowSize - 1;
    int y2 = y1 + win->High/data->RowSize + 2;
    y1 = MIN(MAX(0, y1), data->NumRows - 1);
    y2 = MIN(MAX(0, y2), data->NumRows);

    for (int y = y1; y < y2; y++) {
        DrawRow(win, data, y);
    }
    DrawHeader(win, data);
    return true;
}

bool ListViewOnDraw(CWindow *win, CEvent *evt) {
    if (!win->Visible) return true;
    CCanvas *canvas = win->Canvas;
    int x1 = win->PosX;
    int y1 = win->PosY;
    int x2 = win->PosX + win->Wide;
    int y2 = win->PosY + win->High;
    OnDraw(win, evt);
    Canvas->Blit(canvas, x1, y1, x2, y2);
    return true;
}