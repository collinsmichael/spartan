#include <Library.h>
#include <Kernel.h>
#include <Shell.h>

extern ICanvas *Canvas;
extern IBitmap *Bitmap;
extern IFont   *Font;

bool IGraphics_Poly2v(int x, int y1, int y2, int rgba) {
    CCanvas *canvas = Canvas->GetCanvas();
    if (!canvas) return false;

    if (x < 0 || x >= canvas->ResX) return false;
    int v1 = MIN(MAX(0, y1), canvas->ResY);
    int v2 = MIN(MAX(0, y2), canvas->ResY);

    for (int y = v1; y < v2; y++) {
        int *pixel = (int*)&canvas->Draw[y*canvas->ScanLine];
        pixel[x] = rgba;
    }
    return true;
}

bool IGraphics_Poly2h(int y, int x1, int x2, int rgba) {
    CCanvas *canvas = Canvas->GetCanvas();
    if (!canvas) return false;

    if (y < 0 || y >= canvas->ResY) return false;
    int u1 = MIN(MAX(0, x1), canvas->ResX);
    int u2 = MIN(MAX(0, x2), canvas->ResX);

    int *pixel = (int*)&canvas->Draw[y*canvas->ScanLine];
    stosd(&pixel[u1], rgba, u2 - u1);
    return true;
}

bool IGraphics_Poly2(int x1, int y1, int x2, int y2, int rgba) {
    CCanvas *canvas = Canvas->GetCanvas();
    if (!canvas) return false;
    return true;
}

bool IGraphics_Poly4(int x1, int y1, int x2, int y2, int rgba) {
    CCanvas *canvas = Canvas->GetCanvas();
    if (!canvas) return false;

    int u1 = MIN(MAX(0, x1  ), canvas->ResX  );
    int v1 = MIN(MAX(0, y1  ), canvas->ResY  );
    int u2 = MIN(MAX(0, x2-1), canvas->ResX-1);
    int v2 = MIN(MAX(0, y2-1), canvas->ResY-1);
    if (u2 <= u1 || v2 <= v1) return false;

    if (v1 == y1) {
        int *top = (int*)&canvas->Draw[v1*canvas->ScanLine];
        stosd(&top[u1], rgba, u2 - u1 + 1);
    }

    if (u1 == x1)  {
        for (int y = v1+1; y < v2; y++) {
            int *mid = (int*)&canvas->Draw[y*canvas->ScanLine];
            mid[u1] = rgba;
        }
    }

    if (u2 == x2-1) {
        for (int y = v1+1; y < v2; y++) {
            int *mid = (int*)&canvas->Draw[y*canvas->ScanLine];
            mid[u2] = rgba;
        }
    }

    if (v2 == y2-1) {
        int *btm = (int*)&canvas->Draw[v2*canvas->ScanLine];
        stosd(&btm[u1], rgba, u2 - u1 + 1);
    }
    return true;
}

bool IGraphics_Poly4f(int x1, int y1, int x2, int y2, int rgba) {
    CCanvas *canvas = Canvas->GetCanvas();
    if (!canvas) return false;

    int u1 = MIN(MAX(0, x1), canvas->ResX);
    int v1 = MIN(MAX(0, y1), canvas->ResY);
    int u2 = MIN(MAX(0, x2), canvas->ResX);
    int v2 = MIN(MAX(0, y2), canvas->ResY);
    if (u2 <= u1 || v2 <= v1) return false;
    for (int y = v1; y < v2; y++) {
        int *mid = (int*)&canvas->Draw[y*canvas->ScanLine];
        stosd(&mid[u1], rgba, u2 - u1);
    }
    return true;
}

bool IGraphics_Poly4gv(int x1, int y1, int x2, int y2, int rgba[]) {
    CCanvas *canvas = Canvas->GetCanvas();
    if (!canvas) return false;

    int u1 = MIN(MAX(0, x1), canvas->ResX);
    int v1 = MIN(MAX(0, y1), canvas->ResY);
    int u2 = MIN(MAX(0, x2), canvas->ResX);
    int v2 = MIN(MAX(0, y2), canvas->ResY);
    if (u2 <= u1 || v2 <= v1) return false;
    for (int y = v1; y < v2; y++) {
        int *image = (int*)&canvas->Draw[y*canvas->ScanLine];
        int c = rgba[0] + (y-y1)*(rgba[1]-rgba[0])/(y2-y1);
        stosd(&image[u1], c, u2 - u1);
    }
    return true;
}

bool IGraphics_Poly4gh(int x1, int y1, int x2, int y2, int rgba[]) {
    CCanvas *canvas = Canvas->GetCanvas();
    if (!canvas) return false;

    int u1 = MIN(MAX(0, x1), canvas->ResX);
    int v1 = MIN(MAX(0, y1), canvas->ResY);
    int u2 = MIN(MAX(0, x2), canvas->ResX);
    int v2 = MIN(MAX(0, y2), canvas->ResY);
    if (u2 <= u1 || v2 <= v1) return false;
    for (int x = u1; x < u2; x++) {
        int c = rgba[0] + (x-x1)*(rgba[1]-rgba[0])/(x2-x1);
        int *image = (int*)&canvas->Draw[v1*canvas->ScanLine];
        for (int y = v1; y < v2; y++) {
            image[x] = c;
            image += canvas->ScanLine/4;
        }
    }
    return true;
}

bool IGraphics_Poly4t(int x1, int y1, int x2, int y2, CBitmap *bitmap) {
    CCanvas *canvas = Canvas->GetCanvas();
    if (!canvas) return false;

    int u1 = MIN(MAX(0, x1), canvas->ResX);
    int v1 = MIN(MAX(0, y1), canvas->ResY);
    int u2 = MIN(MAX(0, x2), canvas->ResX);
    int v2 = MIN(MAX(0, y2), canvas->ResY);
    if (u2 <= u1 || v2 <= v1) return false;
    for (int y = v1; y < v2; y++) {
        int v = y - y1;
        int *img = (int*)&bitmap->Image[v*bitmap->ScanLine];
        int *and = (int*)&bitmap->Mask[v*bitmap->ScanLine];
        int *pix = (int*)&canvas->Draw[y*canvas->ScanLine];
        for (int x = u1; x < u2; x++) {
            int u = x - x1;
            pix[x] = (pix[x] & and[u]) | img[u];
        }
    }
    return true;
}

bool IGraphics_Poly2vClip(CArea *area, int x, int y1, int y2, int rgba) {
    CCanvas *canvas = Canvas->GetCanvas();
    if (!area || !canvas) return false;

    int u = MIN(MAX(area->X1, x), area->X2);
    u = MIN(MAX(0, u), canvas->ResX);
    if (u != x) return true;

    int v1 = MIN(MAX(area->Y1, y1), area->Y2);
    int v2 = MIN(MAX(area->Y1, y2), area->Y2);
    v1 = MIN(MAX(0, v1), canvas->ResY);
    v2 = MIN(MAX(0, v2), canvas->ResY);
    if (v2 <= v1) return false;

    for (int y = v1; y < v2; y++) {
        int *pixel = (int*)&canvas->Draw[y*canvas->ScanLine];
        pixel[u] = rgba;
    }
    return true;
}

bool IGraphics_Poly2hClip(CArea *area, int y, int x1, int x2, int rgba) {
    CCanvas *canvas = Canvas->GetCanvas();
    if (!area || !canvas) return false;

    int v = MIN(MAX(area->Y1, y), area->Y2);
    v = MIN(MAX(0, v), canvas->ResY);
    if (v != y) return true;

    int u1 = MIN(MAX(area->X1, x1), area->X2);
    int u2 = MIN(MAX(area->X1, x2), area->X2);
    v = MIN(MAX(0, v), canvas->ResY);
    u1 = MIN(MAX(0, u1), canvas->ResX);
    u2 = MIN(MAX(0, u2), canvas->ResX);
    if (u2 <= u1) return false;

    int *pixel = (int*)&canvas->Draw[v*canvas->ScanLine];
    stosd(&pixel[u1], rgba, u2 - u1);
    return true;
}

bool IGraphics_Poly4Clip(CArea *area, int x1, int y1, int x2, int y2, int rgba) {
    CCanvas *canvas = Canvas->GetCanvas();
    if (!canvas || !area) return false;

    int u1 = MIN(MAX(area->X1, x1  ), area->X2);
    int v1 = MIN(MAX(area->Y1, y1  ), area->Y2);
    int u2 = MIN(MAX(area->X1, x2-1), area->X2);
    int v2 = MIN(MAX(area->Y1, y2-1), area->Y2);
    u1 = MIN(MAX(0, u1), canvas->ResX);
    v1 = MIN(MAX(0, v1), canvas->ResY);
    u2 = MIN(MAX(0, u2), canvas->ResX);
    v2 = MIN(MAX(0, v2), canvas->ResY);
    if (u2 <= u1 || v2 <= v1) return false;

    if (v1 == y1) {
        int *top = (int*)&canvas->Draw[v1*canvas->ScanLine];
        stosd(&top[u1], rgba, u2 - u1 + 1);
    }

    if (u1 == x1)  {
        for (int y = v1+1; y < v2; y++) {
            int *mid = (int*)&canvas->Draw[y*canvas->ScanLine];
            mid[u1] = rgba;
        }
    }

    if (u2 == x2-1) {
        for (int y = v1+1; y < v2; y++) {
            int *mid = (int*)&canvas->Draw[y*canvas->ScanLine];
            mid[u2] = rgba;
        }
    }

    if (v2 == y2-1) {
        int *btm = (int*)&canvas->Draw[v2*canvas->ScanLine];
        stosd(&btm[u1], rgba, u2 - u1 + 1);
    }
    return true;
}

bool IGraphics_Poly4fClip(CArea *area, int x1, int y1, int x2, int y2, int rgba) {
    CCanvas *canvas = Canvas->GetCanvas();
    if (!canvas || !area) return false;

    int u1 = MIN(MAX(area->X1, x1), area->X2);
    int v1 = MIN(MAX(area->Y1, y1), area->Y2);
    int u2 = MIN(MAX(area->X1, x2), area->X2);
    int v2 = MIN(MAX(area->Y1, y2), area->Y2);
    u1 = MIN(MAX(0, u1), canvas->ResX);
    v1 = MIN(MAX(0, v1), canvas->ResY);
    u2 = MIN(MAX(0, u2), canvas->ResX);
    v2 = MIN(MAX(0, v2), canvas->ResY);
    if (u2 <= u1 || v2 <= v1) return false;

    for (int y = v1; y < v2; y++) {
        int *mid = (int*)&canvas->Draw[y*canvas->ScanLine];
        stosd(&mid[u1], rgba, u2 - u1);
    }
    return true;
}

bool IGraphics_Poly4tClip(CArea *area, int x1, int y1, int x2, int y2, CBitmap *bitmap) {
    CCanvas *canvas = Canvas->GetCanvas();
    if (!canvas || !area) return false;

    int u1 = MIN(MAX(area->X1, x1), area->X2);
    int v1 = MIN(MAX(area->Y1, y1), area->Y2);
    int u2 = MIN(MAX(area->X1, x2), area->X2);
    int v2 = MIN(MAX(area->Y1, y2), area->Y2);
    u1 = MIN(MAX(0, u1), canvas->ResX);
    v1 = MIN(MAX(0, v1), canvas->ResY);
    u2 = MIN(MAX(0, u2), canvas->ResX);
    v2 = MIN(MAX(0, v2), canvas->ResY);

    if (u2 <= u1 || v2 <= v1) return false;
    for (int y = v1; y < v2; y++) {
        int v = y - y1;
        int *img = (int*)&bitmap->Image[v*bitmap->ScanLine];
        int *and = (int*)&bitmap->Mask[v*bitmap->ScanLine];
        int *pix = (int*)&canvas->Draw[y*canvas->ScanLine];
        for (int x = u1; x < u2; x++) {
            int u = x - x1;
            pix[x] = (pix[x] & and[u]) | img[u];
        }
    }
    return true;
}

static IGraphics graphics = {
    IGraphics_Poly2v,
    IGraphics_Poly2h,
    IGraphics_Poly2,
    IGraphics_Poly4,
    IGraphics_Poly4f,
    IGraphics_Poly4gv,
    IGraphics_Poly4gh,
    IGraphics_Poly4t,
    IGraphics_Poly4Clip,
    IGraphics_Poly4fClip,
    IGraphics_Poly4tClip,
    IGraphics_Poly2vClip,
    IGraphics_Poly2hClip
};

export IGraphics *Graphics = &graphics;