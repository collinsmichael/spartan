/* ************************************************************************** */
/* author(s) : Michael Collins                                                */
/* file name : include/spartan/pc/cga.h                                       */
/* file date : 2017/12/03                                                     */
/* file info : Color Graphics Adapter Interface                               */
/* ************************************************************************** */
#ifndef CGA_H
#define CGA_H
#include <oop.h>
#pragma pack(push, 1)

#define CGA_REG_NUM   0x03D4 // 8bit wo (do an out 0x3D4, reg_no then in 0x03D5)
#define CGA_REG_VAL   0x03D5 // 8bit rw (do an out 0x3D4, reg_no then in/out 0x03D5)
#define CGA_MODE_SET  0x03D8 // 8bit wo (do an out, reg_no followed by a in 0x03D5)
#define CGA_STATUS    0x03DA // 8bit ro Status Register
#define CGA_PEN_RESET 0x03DB // 8bit rw Light Pen Latch Reset. Any OUT clears light pen latch.
#define CGA_PEN_SET   0x03DC // 8bit rw Light Pen Latch Set. Any OUT sets the light pen latch.

#define CGA_MODE_SET_WIDTH        0x01 // (text modes): 0=40 cols; 1=80 cols
#define CGA_MODE_SET_MODE         0x02 // 1=enable graphics modes; 0=text modes
#define CGA_MODE_SET_COLOR_BURST  0x04 // 1=disable color burst on composite monitor
#define CGA_MODE_SET_VIDEO_SIGNAL 0x08 // 1=enable video signal; 0=disable (all black)
#define CGA_MODE_SET_RESOLUTION   0x10 // dot resolutn: 0=320 2-bit dots; 1=640 1-bit dots
#define CGA_MODE_SET_BLINK_CURSOR 0x20 // 1=text attribute bit 7 blinks fgnd

#define CGA_RETRACE     0x01 // 1=display in vert or horiz retrace.
#define CGA_PEN_TRIGGER 0x02 // 1=light pen trigger is triggered; 0=armed
#define CGA_PEN_SWITCH  0x04 // 1=light pen switch is open; 0=closed
#define CGA_VSYNC       0x08 // 1=vertical sync pulse is occurring.

// ***************************************************************************
// Source of Information
//     http://www.techhelpmanual.com/901-color_graphics_adapter_i_o_ports.html
//  This describes the I/O control ports for the Color/Graphics Adapter.  The
//  CGA is built around the Motorolla 6845 CRT controller chip.
// 
//    ¦ The CGA decodes ports 3d0H through 3dfH (actually using 3d2H-3dcH)
// 
//     Note: These ports overlap the EGA I/O Ports for upward compatibility.
//           With some exceptions, the CGA information applies to the EGA.
// 
//  An early Tech Ref indicated that the CGA could force hardware interrupt
//  level 2 (IRQ 2) upon vertical retrace.  However, the CGA has never had
//  this ability (but EGA/VGA supports this feature).
// 
//     Note: BIOS stores "mirrors" of the current CRTC port values in the
//           BIOS Data Area.  To change a single bit, you should read the
//           appropriate variable, set the bit, perform the OUT, and store the
//           new value in the variable.
// 
// Port  Description
// ¯¯¯¯  ¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯
// 3d4H  Write: CRT Controller register select.  Use this port to select any of
//       the 18 CRTC registers.  Do an OUT 3d4H,reg_no followed by a read from
//       or write to port 3d5H.
// ---- -----------------------------------------------------------------------
// 3d5H  CRT Controller registers.  After selecting via port 3d4H, output or
//       input here to access the CRTC internal registers.
// 
//       Write: See Video Initialization Table for a summary of the CRTC
//              registers and a way to look up standard BIOS settings for the
//              display modes.
// 
//       Read:  Registers R12 through R17 can be read to determine the current
//              video page address, cursor address, and light pen information.
// ---- -----------------------------------------------------------------------
// 3d8H  Write: Mode Select Register (BIOS keeps current value at 40:0065)
//       +7-6-5-4-3-2-1-0+
//       ¦   ¦b¦r¦v¦c¦g¦w¦
//       +---------------+ bit
//            ¦ ¦ ¦ ¦ ¦ +-? 0: width (text modes): 0=40 clms; 1=80 clms
//            ¦ ¦ ¦ ¦ +---? 1: 1=enable graphics modes; 0=text modes
//            ¦ ¦ ¦ +-----? 2: 1=disable color burst on composite monitor
//            ¦ ¦ +-------? 3: 1=enable video signal; 0=disable (all black)
//            ¦ +---------? 4: dot resolutn: 0=320 2-bit dots; 1=640 1-bit dots
//            +-----------? 5: enable blink: 1=text attribute bit 7 blinks fgnd
//                                           0=attr bit 7 is bkgd intensity
// ---- -----------------------------------------------------------------------
// 3d8H  Write: Color Select Register
//       +7-6-5-4-3-2-1-0+
//       ¦   ¦b¦i¦I¦R¦G¦B¦
//       +---------------+ bit
//            ¦ ¦ +-------? 0-3: color. Text modes: IRGB of border
//            ¦ ¦                       Graphics modes: IRGB of background
//            ¦ +---------? 4: intensity. Text: bkgd intensity
//            ¦                           Mid-res graphics: pixel intensity
//            +-----------? 5: blue. Select grafx palette 0 or 1 (1 adds blue)
// ---- -----------------------------------------------------------------------
// 3daH  Read: Status Register
//       +7-6-5-4-3-2-1-0+
//       ¦       ¦ ¦ ¦ ¦ ¦
//       +---------------+ bit
//                ¦ ¦ ¦ +-? 0: retrace.  1=display in vert or horiz retrace.
//                ¦ ¦ +---? 1: 1=light pen trigger is triggered; 0=armed
//                ¦ +-----? 2: 1=light pen switch is open; 0=closed
//                +-------? 3: 1=vertical sync pulse is occurring.  Display is
//                               in vertical retrace--access won't cause "snow"
// ---- -----------------------------------------------------------------------
// 3dbH  Read/Write: Light Pen Latch Reset.  Any OUT clears light pen latch.
// 3dcH  Read/Write: Light Pen Latch Set.  Any OUT sets the light pen latch.
// ---- -----------------------------------------------------------------------

#pragma pack(pop)
#endif // CGA_H