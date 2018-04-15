#ifndef SHELL_WINDOW_WIDGET_H
#define SHELL_WINDOW_WIDGET_H
#include <oop.h>
#include <Shell\RunTime\Graphics\Bitmap.h>
#include <Shell\Window\Window.h>

typedef struct IWidget {
    CWindow *(*Panel   )(CWindow *parent);
    CWindow *(*Button  )(CWindow *parent);
    CWindow *(*Progress)(CWindow *parent);
    CWindow *(*Slider  )(CWindow *parent);
    CWindow *(*Scroll  )(CWindow *parent, int Orientation);
    CWindow *(*Console )(CWindow *parent);
    CWindow *(*TextBox )(CWindow *parent);
    CWindow *(*ListView)(CWindow *parent);
    CWindow *(*TabBar  )(CWindow *parent);
    CWindow *(*NoteBox )(CWindow *parent);
} IWidget;


// ========================================================================== //
// NoteBox Widget                                                             //
// ========================================================================== //

typedef struct CNoteBox {
    char *Text;
    int   Size;
    char *Format;
    int   Caret;
    bool  ReadOnly;
    bool  Numeric;
    bool  Password;
} CNoteBox;

typedef struct INoteBox {
    bool  (*ClearText)(CWindow *textbox);
    char *(*GetText)(CWindow *textbox);
    bool  (*SetText)(CWindow *textbox, char *text, int size);
    int   (*GetSize)(CWindow *textbox);
    bool  (*SetSize)(CWindow *textbox, int size);
    char *(*GetFormat)(CWindow *textbox);
    bool  (*SetFormat)(CWindow *textbox, char *text);
    bool  (*GetReadOnly)(CWindow *textbox);
    bool  (*SetReadOnly)(CWindow *textbox, bool value);
    bool  (*GetNumeric)(CWindow *textbox);
    bool  (*SetNumeric)(CWindow *textbox, bool value);
    bool  (*GetPassword)(CWindow *textbox);
    bool  (*SetPassword)(CWindow *textbox, bool value);
} INoteBox;

// ========================================================================== //
// ListView Widget                                                            //
// ========================================================================== //

typedef struct CListView {
    char **Headers;
    int    HeadSize;
    int    RowSize;
    int   *ColSize;
    int    MaxRows;
    int    NumRows;
    int    NumCols;

    int    Hover;
    int    Selected;
    int    Scroll;
    void  *Items[1];
} CListView;

typedef struct IListView {
    char **(*Retrieve )(CWindow *lv, int i);
    char **(*Selected )(CWindow *lv);
    bool   (*Insert   )(CWindow *lv, char **li);
    bool   (*Remove   )(CWindow *lv, int i);
    bool   (*RemoveAll)(CWindow *lv);
} IListView;

// ========================================================================== //
// TabBar Widget                                                              //
// ========================================================================== //
typedef struct CTabItem {
    int   TabId;
    char *Text;
    void *Icon;
    void *Button;
} CTabItem;

typedef struct CTabBar {
    int       Count;
    int       Selected;
    CTabItem *Items;
    CBitmap  *Icons;
    char     *Texts;
} CTabBar;

typedef struct ITabBar {
    bool      (*Remove   )(CWindow *tb, int i);
    bool      (*RemoveAll)(CWindow *tb);
    CTabItem *(*Insert   )(CWindow *tb, int count, char *text[], char *icons[]);
    CTabItem *(*Retrieve )(CWindow *tb, int i);
    CTabItem *(*Selected )(CWindow *tb);
} ITabBar;

// ========================================================================== //
// TextBox Widget                                                             //
// ========================================================================== //
typedef struct CTextBox {
    char  *Text;
    int    Size;
    int    MaxRows;
    int    MaxCols;
    int    NumRows;
    int    NumCols;
    int    CursorX;
    int    CursorY;
} CTextBox;

typedef struct ITextBox {
    bool (*GetText)(CWindow *textbox, char *text, int size);
    bool (*SetText)(CWindow *textbox, char *text, int size);
} ITextBox;

// ========================================================================== //
// Scroll Widget                                                              //
// ========================================================================== //
typedef struct CScroll {
    int   TrackMin;   // pixels
    int   TrackPos;   // pixels
    int   TrackMax;   // pixels
    int   TrackDelta; // pixels
    int   ThumbSize;  // pixels
    int   PageSize;   // pixels
    int   UnitCount;  // integer
    int   UnitSize;   // pixels
} CScroll;

typedef struct IScroll {
    bool (*SetTrackMin )(CWindow *scroll, int TrackMin );
    bool (*SetTrackPos )(CWindow *scroll, int TrackPos );
    bool (*SetTrackMax )(CWindow *scroll, int TrackMax );
    bool (*SetThumbSize)(CWindow *scroll, int ThumbSize);
    bool (*SetPageSize )(CWindow *scroll, int PageSize );
    bool (*SetUnitCount)(CWindow *scroll, int UnitCount);
    bool (*SetUnitSize )(CWindow *scroll, int UnitSize );

    bool (*Resize      )(CWindow *scroll, int x1, int y1, int x2, int y2);
    bool (*Goto        )(CWindow *scroll, int item);
    int  (*FirstVisible)(CWindow *scroll);
    int  (*LastVisible )(CWindow *scroll);
} IScroll;

#endif SHELL_WINDOW_WIDGET_H