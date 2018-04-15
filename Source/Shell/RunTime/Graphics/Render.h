#ifndef SHELL_GRAPHICS_CANVAS_RENDER_H
#define SHELL_GRAPHICS_CANVAS_RENDER_H
#include <oop.h>
#pragma pack(push, 1)

typedef struct IRender {
    bool (*Dispatch)(void *canvas, u32 op, ...);
    bool (*Decoder )(void *canvas);
} IRender;

// virtual machine opcodes
#define RENDER_CMD_NOP     0x00 // Render->Nop()
#define RENDER_NUM_NOP     0x00 // Render->Nop()

#define RENDER_CMD_PUSH    0x01 // Render->Push(window)
#define RENDER_NUM_PUSH    0x01 // Render->Push(window)

#define RENDER_CMD_POP     0x02 // Render->Pop(window)
#define RENDER_NUM_POP     0x01 // Render->Pop(window)

#define RENDER_CMD_SHOW    0x03 // Render->Show(window)
#define RENDER_NUM_SHOW    0x01 // Render->Show(window)

#define RENDER_CMD_HIDE    0x04 // Render->Hide(window)
#define RENDER_NUM_HIDE    0x01 // Render->Hide(window)

#define RENDER_CMD_MOVE    0x05 // Render->Move(window,x,y)
#define RENDER_NUM_MOVE    0x03 // Render->Move(window,x,y)

#define RENDER_CMD_SIZE    0x06 // Render->Size(window,w,h)
#define RENDER_NUM_SIZE    0x03 // Render->Size(window,w,h)

#define RENDER_CMD_DRAW    0x07 // Render->Draw(window)
#define RENDER_NUM_DRAW    0x01 // Render->Draw(window)

#define RENDER_CMD_REVEAL  0x08 // Render->Reveal(x1,y1,x2,y2)
#define RENDER_NUM_REVEAL  0x04 // Render->Reveal(x1,y1,x2,y2)

#define RENDER_CMD_REFRESH 0x09 // Render->Refresh()
#define RENDER_NUM_REFRESH 0x00 // Render->Refresh()

#pragma pack(pop)
#endif SHELL_GRAPHICS_CANVAS_RENDER_H