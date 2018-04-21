#include <Kernel.h>
#include <Shell.h>

extern ICanvas   *Canvas;
extern IGraphics *Graphics;
extern IFont     *Font;

bool ICaret_Scroll(CWindow *window, int x1, int y1, int x2, int y2, int w, int h, int delta) {
    if (!window) return false;
    CCanvas *canvas = Canvas->GetCanvas();
    if (!canvas) return false;

    char *source = &canvas->Draw[y1*canvas->ScanLine + x1*4];
    char *target = &canvas->Draw[y2*canvas->ScanLine + x2*4];
    for (int y = 0; y < h; y++) {
        movsd(target, source, w);
        source += delta;
        target += delta;
    }
    return true;
}

static bool ICaret_Show(CWindow *window) {
    if (!window) return false;
    CCanvas *canvas = Canvas->GetCanvas();
    CFont *font = window->Font;
    if (!canvas || !font) return false;

    int x = window->PosX + window->CaretX*font->FontW;
    int y = window->PosY + window->CaretY*font->FontH;
    int w = font->FontW;
    int h = font->FontH;

    CArea area;
    area.X1 = window->PosX;
    area.Y1 = window->PosY;
    area.X2 = window->PosX + window->Wide;
    area.Y2 = window->PosY + window->High;

    Graphics->Poly4fClip(&area, x, y, x+w, y+h, window->BackGround[0]);
    Font->PutCharClip(&area, font, canvas, x, y, window->ForeGround[0], '\xB1');
    return true;
}

static bool ICaret_Hide(CWindow *window) {
    if (!window) return false;
    CCanvas *canvas = Canvas->GetCanvas();
    CFont *font = window->Font;
    if (!canvas || !font) return false;

    int x = window->PosX + window->CaretX*font->FontW;
    int y = window->PosY + window->CaretY*font->FontH;
    int w = font->FontW;
    int h = font->FontH;

    CArea area;
    area.X1 = window->PosX;
    area.Y1 = window->PosY;
    area.X2 = window->PosX + window->Wide;
    area.Y2 = window->PosY + window->High;
    Graphics->Poly4fClip(&area, x, y, x+w, y+h, window->BackGround[0]);
    //Font->PutC(font, canvas, x, y, window->BackGround[0], '\xB1');
    return true;
}

static bool ICaret_Advance(CWindow *window) {
    if (!window) return false;
    CCanvas *canvas = Canvas->GetCanvas();
    CFont *font = window->Font;

    window->CaretX++;
    if (window->CaretX >= window->Wide/font->FontW) {
        window->CaretX = 0;
        window->CaretY++;
        if (window->CaretY >= window->High/font->FontH) {
            window->CaretY--;

            int x1 = window->PosX;
            int y1 = window->PosY + font->FontH;
            int x2 = window->PosX;
            int y2 = window->PosY;
            int w  = window->Wide;
            int h  = window->High - font->FontH;
            int d  = canvas->ScanLine;
            ICaret_Scroll(window, x1, y1, x2, y2, w, h, d);

            char *target = &canvas->Draw[(y2+h)*canvas->ScanLine];
            target += x1*4;
            for (int y = 0; y < font->FontH; y++) {
                stosd(target, window->BackGround[0], w);
                target += canvas->ScanLine;
            }
        }
    }
    return true;
}

static bool ICaret_Retreat(CWindow *window) {
    if (!window) return false;
    CCanvas *canvas = Canvas->GetCanvas();
    CFont *font = window->Font;

    window->Caret = (window->Caret > 0) ? window->Caret-- : 0;
    window->CaretX--;
    if (window->CaretX >= window->Wide/font->FontW) {
        window->CaretX = 0;
        window->CaretY++;
        if (window->CaretY >= window->High/font->FontH) {
            window->CaretY--;

            int x1 = window->PosX;
            int y1 = window->PosY + font->FontH;
            int x2 = window->PosX;
            int y2 = window->PosY;
            int w  = window->Wide;
            int h  = window->High - font->FontH;
            int d  = canvas->ScanLine;
            ICaret_Scroll(window, x1, y1, x2, y2, w, h, d);

            char *target = &canvas->Draw[(y2+h)*canvas->ScanLine];
            target += x1*4;
            for (int y = 0; y < font->FontH; y++) {
                stosd(target, window->BackGround[0], w);
                target += canvas->ScanLine;
            }
        }
    }
    return true;
}

static bool PutC(CWindow *window, char c) {
    if (!window) return false;
    CCanvas *canvas = Canvas->GetCanvas();
    CFont *font = window->Font;
    if (!canvas || !font) return false;
    if (!font->FontW || !font->FontH) return false;

    ICaret_Hide(window);

    int x = window->PosX + window->CaretX*font->FontW;
    int y = window->PosY + window->CaretY*font->FontH;
    int w = font->FontW;
    int h = font->FontH;

    CArea area;
    area.X1 = window->PosX;
    area.Y1 = window->PosY;
    area.X2 = window->PosX + window->Wide;
    area.Y2 = window->PosY + window->High;
    if (c == '\b') {
        ICaret_Retreat(window);
        if (window->Text) window->Text[window->Caret] = 0;
        Graphics->Poly4fClip(&area, x, y, x+w, y+h, window->BackGround[0]);
        Font->PutCharClip(&area, font, canvas, x, y, window->ForeGround[0], ' ');
    } else {
        Graphics->Poly4fClip(&area, x, y, x+w, y+h, window->BackGround[0]);
        Font->PutCharClip(&area, font, canvas, x, y, window->ForeGround[0], c);
        if (window->Text) window->Text[window->Caret] = c;
        ICaret_Advance(window);
    }
    ICaret_Show(window);
    return true;
}

static bool ICaret_PutC(CWindow *window, char c) {
    if (!window) return false;

    switch (c) {
    case '\0': break;
    case '\r': window->CaretX = 0; break;
    case '\n':
        do {
            PutC(window, ' ');
        } while (window->CaretX != 0);
        break;
    case '\t':
        do {
            PutC(window, ' ');
        } while (window->CaretX % 4 != 0);
        break;
    default:
        PutC(window, c);
        break;
    }
    return true;
}

static bool ICaret_Print(CWindow *window, char *format, ...) {
    if (!window || !format) return false;

    char text[1024];
    va_list *list;
    va_start(list, format);
    VaFormat(text, format, list);
    va_end(list);

    for (int i = 0; i < elementsof(text) && text[i]; i++) {
        ICaret_PutC(window, text[i]);
    }
    return true;
}

static bool ICaret_TextOut(CWindow *window, int x, int y, char *format, ...) {
    if (!window || !format) return false;

    char text[1024];
    va_list *list;
    va_start(list, format);
    VaFormat(text, format, list);
    va_end(list);

    CCanvas *canvas = Canvas->GetCanvas();
    CFont *font = window->Font;

    CArea area;
    area.X1 = window->PosX;
    area.Y1 = window->PosY;
    area.X2 = window->PosX + window->Wide;
    area.Y2 = window->PosY + window->High;
    for (int i = 0; i < elementsof(text) && text[i]; i++) {
        Font->PutCharClip(&area, font, canvas, x, y, window->ForeGround[0], text[i]);
        x += font->FontW;
    }
    return true;
}

static ICaret caret = {
    ICaret_Scroll,
    ICaret_Show,
    ICaret_Hide,
    ICaret_Advance,
    ICaret_PutC,
    ICaret_Print,
    ICaret_TextOut
};

export ICaret *Caret = &caret;