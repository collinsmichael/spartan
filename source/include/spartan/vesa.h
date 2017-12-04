/* ************************************************************************** */
/* author(s) : Michael Collins                                                */
/* file name : include/spartan/vesa.h                                         */
/* file date : 2017/12/03                                                     */
/* file info : vesa video mode definitions                                    */
/* ************************************************************************** */
#ifndef VESA_H
#define VESA_H
#include <stdint.h>
#pragma pack(push, 1)

typedef struct vesainfo_t {
    uint8_t  magic[4];
    uint16_t version;
    uint32_t oem_string;
    uint32_t capabilities;
    uint32_t video_modes;
    uint16_t total_memory;
    uint16_t oem_software_rev;
    uint32_t oem_vendor_name;
    uint32_t oem_product_name;
    uint32_t oem_product_rev;
    uint8_t  reserved[222];
    uint8_t  oem_data[256];
} vesainfo_t;

#define DAC_SUPPORT_8BIT   0x00000001 // 0      DAC can be switched into 8-bit mode
#define NON_VGA_CONTROLLER 0x00000002 // 1      non-VGA controller
#define DAC_BLANK_BIT      0x00000004 // 2      programmed DAC with blank bit (i.e. only during blanking interval)
#define STERIOSCOPIC       0x00000008 // 3      (VBE v3.0) controller supports hardware stereoscopic signalling
#define VBE_AF_SUPPORT     0x00000008 // 3      controller supports VBE/AF v1.0P extensions
#define STERIO_SIGNALLING  0x00000010 // 4      (VBE v3.0) if bit 3 set:
                                      //        =0 stereo signalling via external VESA stereo connector
                                      //        =1 stereo signalling via VESA EVC connector
#define DIRECT_FRAMEBUFFER 0x00000010 // 4      (VBE/AF v1.0P) must call EnableDirectAccess to access framebuffer
#define HARDWARE_MOUSE     0x00000020 // 5      (VBE/AF v1.0P) controller supports hardware mouse cursor
#define HARDWARE_CLIPPING  0x00000040 // 6      (VBE/AF v1.0P) controller supports hardware clipping
#define TRANSPARENT_BLIT   0x00000080 // 7      (VBE/AF v1.0P) controller supports transparent BitBLT

typedef struct modeinfo_t {
    uint16_t mode_attributes;       // 00
    uint8_t  winA_attributes;       // 02
    uint8_t  winB_attributes;       // 03
    uint16_t win_granularity;       // 04
    uint16_t win_size;              // 06
    uint16_t winA_segment;          // 08
    uint16_t winB_segment;          // 0A
    uint32_t win_func_ptr;          // 0C
    uint16_t bytes_scanline;        // 10
    uint16_t resx;                  // 12
    uint16_t resy;                  // 14
    uint8_t  char_w;                // 16
    uint8_t  char_h;                // 17
    uint8_t  numplanes;             // 18
    uint8_t  bits;                  // 19
    uint8_t  numbanks;              // 1A
    uint8_t  memory_model;          // 1B
    uint8_t  bank_size;             // 1C
    uint8_t  num_image_pages;       // 1D
    uint8_t  reserved_page;         // 1E
    uint8_t  red_mask_size;         // 1F
    uint8_t  red_mask_pos;          // 20
    uint8_t  green_mask_size;       // 21
    uint8_t  green_mask_pos;        // 22
    uint8_t  blue_mask_size;        // 23
    uint8_t  blue_mask_pos;         // 24
    uint8_t  alpha_mask_size;       // 25
    uint8_t  alpha_mask_pos;        // 26
    uint8_t  direct_color_modeinfo; // 27
    uint32_t physbaseptr;           // 28
    uint32_t offscreen_mem_offset;  // 2C
    uint16_t offscreen_mem_size;    // 30
    uint8_t  reserved[206];         // 32
} modeinfo_t;

#define VESA_MODE_SUPPORTED     0x0001 // (VBE1.0) mode supported by present hardware configuration
#define VESA_OPTIONAL_INFO      0x0002 // (VBE1.0) optional information available (must be =1 for VBE v1.2+)
#define VESA_BIOS_OUTPUT        0x0004 // (VBE1.0) BIOS output supported
#define VESA_COLOR              0x0008 // (VBE1.0) set if color, clear if monochrome
#define VESA_GRAPHICS           0x0010 // (VBE1.0) set if graphics mode, clear if text mode
#define VESA_NON_VGA_MODE       0x0020 // (VBE2.0) mode is not VGA-compatible
#define VESA_NON_BANKED         0x0040 // (VBE2.0) bank-switched mode not supported
#define VESA_LINEAR             0x0080 // (VBE2.0) linear framebuffer mode supported
#define VESA_DOUBLE_SCAN        0x0100 // (VBE2.0) double-scan mode available (e.g. 320x200 and 320x240)
#define VESA_INTERLACED         0x0200 // (VBE3.0) interlaced mode available
#define VESA_TRIPLE_BUFFER      0x0400 // (VBE3.0) hardware supports triple buffering
#define VESA_STERIOSCOPIC       0x0800 // (VBE3.0) hardware supports stereoscopic display
#define VESA_DUALDISPLAY_ADDR   0x1000 // (VBE3.0) dual display start address support
#define VESA_MUST_ENABLE_ACCESS 0x0200 // (VBE/AF) application must call EnableDirectAccess before calling bank-switching

typedef struct crtcinfo_t {
    uint16_t resx;
    uint16_t horz_sync_start;
    uint16_t horz_sync_end;
    uint16_t resy;
    uint16_t vert_sync_start;
    uint16_t vert_sync_end;
    uint8_t  flags;
    uint32_t pixel_clock_hz;   // pixel clock, in Hz
    uint16_t refresh_hz; // refresh rate, in 0.01 Hz units
    uint8_t  reserved[40];
} crtcinfo_t;

#define CRTC_DOUBLE_SCAN    0x01 // 0 enable double scanning
#define CRTC_INTERLACED     0x02 // 1 enable interlacing
#define CRTC_HSYNC_POLARITY 0x03 // 2 (0 positive, 1 negative)
#define CRTC_VSYNC_POLARITY 0x04 // 3 (0 positive, 1 negative)

typedef struct vesamode_t {
    uint16_t resx;
    uint16_t resy;
    uint16_t bits;
    uint16_t mode;
} vesamode_t;

#pragma pack(pop)
#endif // VESA_H