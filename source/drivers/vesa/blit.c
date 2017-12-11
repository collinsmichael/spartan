#include <arch.h>
#include <driver.h>
#include <memory.h>
#include <pipe.h>
#include <vesa.h>

#include <pc/cga.h>

_declspec(dllexport)
int flush(PIPE *pipe) {
    // to avoid tearing we only draw to the framebuffer during vertical retrace
    // this requires polling the CGA status register...
    //
    // Source of Information
    // http://www.techhelpmanual.com/901-color_graphics_adapter_i_o_ports.html
    // 03DA 8bit ro: CGA Status Register
    // ╓7┬6┬5┬4┬3┬2┬1┬0╖
    // ║       │ │ │ │ ║
    // ╙─┴─┴─┴─┴╥┴╥┴╥┴╥╜ bit
    //          ║ ║ ║ ╚═► 0: retrace.  1=display in vert or horiz retrace.
    //          ║ ║ ╚═══► 1: 1=light pen trigger is triggered; 0=armed
    //          ║ ╚═════► 2: 1=light pen switch is open; 0=closed
    //          ╚═══════► 3: 1=vertical sync pulse is occurring.  Display is
    //                       in vertical retrace--access won't cause "snow"
    register MODE_INFO *mi = GetModeInfo();
    register uint8_t *screen = (uint8_t*)mi->PhysBasePtr;
    register uint16_t width = mi->ResX*mi->Bits/8;
    while ((inb(CGA_STATUS) & CGA_VSYNC) == 0);

    for (uint16_t y = 0; y < mi->ResY; y++) {
        register char *source = &pipe->ring[y*width];
        register char *target = &screen[y*mi->BytesPerScanline];
        memcpy(target, source, width);
    }
    return 1;
}