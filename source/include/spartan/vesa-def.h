/* ************************************************************************** */
/* author(s) : Michael Collins                                                */
/* file name : include/spartan/vesa-def.h                                     */
/* file date : 2017/12/03                                                     */
/* file info : vesa video mode definitions                                    */
/* ************************************************************************** */
#ifndef VESADEF_H
#define VESADEF_H
#include <stdint.h>
#pragma pack(push, 1)

typedef struct VESA_INFO {
    uint8_t  Magic[4];
    uint16_t Version;
    uint32_t OemString;
    uint32_t Capabilities;
    uint32_t VideoModes;
    uint16_t TotalMemory;
    uint16_t OemSoftwareRev;
    uint32_t OemVendorName;
    uint32_t OemProductName;
    uint32_t OemProductRev;
    uint8_t  Reserved[222];
    uint8_t  OemData[256];
} VESA_INFO;

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

typedef struct MODE_INFO {
    uint16_t modeAttributes;      // 00
    uint8_t  WinAAttributes;      // 02
    uint8_t  WinBAttributes;      // 03
    uint16_t WinGranularity;      // 04
    uint16_t WinSize;             // 06
    uint16_t WinASegment;         // 08
    uint16_t WinBSegment;         // 0A
    uint32_t WinFuncPtr;          // 0C
    uint16_t BytesPerScanline;    // 10
    uint16_t ResX;                // 12
    uint16_t ResY;                // 14
    uint8_t  CharW;               // 16
    uint8_t  CharH;               // 17
    uint8_t  NumPlanes;           // 18
    uint8_t  Bits;                // 19
    uint8_t  NumBanks;            // 1A
    uint8_t  MemoryModel;         // 1B
    uint8_t  BankSize;            // 1C
    uint8_t  NumImagePages;       // 1D
    uint8_t  ReservedPage;        // 1E
    uint8_t  RedMaskSize;         // 1F
    uint8_t  RedMaskPos;          // 20
    uint8_t  GreenMaskSize;       // 21
    uint8_t  GreenMaskPos;        // 22
    uint8_t  BlueMaskSize;        // 23
    uint8_t  BlueMaskPos;         // 24
    uint8_t  AlphaMaskSize;       // 25
    uint8_t  AlphaMaskPos;        // 26
    uint8_t  DirectColorModeInfo; // 27
    uint32_t PhysBasePtr;         // 28
    uint32_t OffscreenMemOffset;  // 2C
    uint16_t OffscreenMemSize;    // 30
    uint8_t  Reserved[206];       // 32
} MODE_INFO;

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

typedef struct CRTC_INFO {
    uint16_t ResX;          //
    uint16_t HorzSyncStart; //
    uint16_t HorzSyncEnd;   //
    uint16_t ResY;          //
    uint16_t VertSyncStart; //
    uint16_t VertSyncEnd;   //
    uint8_t  Flags;         //
    uint32_t PixelClockHz;  // pixel clock, in Hz
    uint16_t RefreshHz;     // refresh rate, in 0.01 Hz units
    uint8_t  Reserved[40];  //
} CRTC_INFO;

#define CRTC_DOUBLE_SCAN    0x01 // 0 enable double scanning
#define CRTC_INTERLACED     0x02 // 1 enable interlacing
#define CRTC_HSYNC_POLARITY 0x03 // 2 (0 positive, 1 negative)
#define CRTC_VSYNC_POLARITY 0x04 // 3 (0 positive, 1 negative)

typedef struct VESA_MODE {
    uint16_t ResX;
    uint16_t ResY;
    uint16_t Bits;
    uint16_t Mode;
} VESA_MODE;

#pragma pack(pop)
#endif // VESADEF_H