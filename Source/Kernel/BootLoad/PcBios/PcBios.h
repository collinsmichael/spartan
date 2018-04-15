#ifndef KERNEL_BOOTLOAD_PCBIOS_H
#define KERNEL_BOOTLOAD_PCBIOS_H
#include <oop.h>
#include <Kernel/BootLoad/PcBios/Cga.h>
#include <Kernel/BootLoad/PcBios/Com.h>
#include <Kernel/BootLoad/PcBios/PcBios.h>
#include <Kernel/BootLoad/PcBios/Pit.h>
#include <Kernel/BootLoad/PcBios/Ps2.h>
#include <Kernel/BootLoad/PcBios/Rtc.h>

#pragma pack(push, 1)
typedef struct BIOS {
    u32 eax;
    u32 ecx;
    u32 edx;
    u32 ebx;
    u32 esp;
    u32 ebp;
    u32 esi;
    u32 edi;
    u32 eflags;
} BIOS;

typedef struct E820 {
    u32 Next;
    u64 Base;
    u64 Size;
    u32 Type;
} E820;
#define E820_AVAILABLE 1
#define E820_RESERVED  2
#define E820_ACPI      3
#define E820_NVS       4
#define E820_BADRAM    5

typedef struct VESA_INFO {
    u32 Magic;
    u16 Version;
    u32 OemString;
    u32 Capabilities;
    u32 VideoModes;
    u16 TotalMemory;
    u16 OemSoftwareRev;
    u32 OemVendorName;
    u32 OemProductName;
    u32 OemProductRev;
    u8  Reserved[222];
    u8  OemData[256];
} VESA_INFO;

#define DAC_SUPPORT_8BIT   0x00000001 // 0      DAC can be switched into 8-bit mode
#define NON_VGA_CONTROLLER 0x00000002 // 1      non-VGA controller
#define DAC_BLANK_BIT      0x00000004 // 2      programmed DAC with blank bit (i.e. only during blanking interval)
#define STERIOSCOPIC       0x00000008 // 3      (VBE v3.0) controller supports hardware stereoscopic signalling
#define VBE_AF_SUPPORT     0x00000008 // 3      controller supports VBE/AF v1.0P extensions
#define STERIO_SIGNALLING  0x00000010 // 4      (VBE v3.0) if bit 3 set:
#define DIRECT_FRAMEBUFFER 0x00000010 // 4      (VBE/AF v1.0P) must call EnableDirectAccess to access framebuffer
#define HARDWARE_MOUSE     0x00000020 // 5      (VBE/AF v1.0P) controller supports hardware mouse cursor
#define HARDWARE_CLIPPING  0x00000040 // 6      (VBE/AF v1.0P) controller supports hardware clipping
#define TRANSPARENT_BLIT   0x00000080 // 7      (VBE/AF v1.0P) controller supports transparent BitBLT

typedef struct MODE_INFO {
    u16 modeAttributes;      // 00
    u8  WinAAttributes;      // 02
    u8  WinBAttributes;      // 03
    u16 WinGranularity;      // 04
    u16 WinSize;             // 06
    u16 WinASegment;         // 08
    u16 WinBSegment;         // 0A
    u32 WinFuncPtr;          // 0C
    u16 BytesPerScanline;    // 10
    u16 ResX;                // 12
    u16 ResY;                // 14
    u8  CharW;               // 16
    u8  CharH;               // 17
    u8  NumPlanes;           // 18
    u8  Bits;                // 19
    u8  NumBanks;            // 1A
    u8  MemoryModel;         // 1B
    u8  BankSize;            // 1C
    u8  NumImagePages;       // 1D
    u8  ReservedPage;        // 1E
    u8  RedMaskSize;         // 1F
    u8  RedMaskPos;          // 20
    u8  GreenMaskSize;       // 21
    u8  GreenMaskPos;        // 22
    u8  BlueMaskSize;        // 23
    u8  BlueMaskPos;         // 24
    u8  AlphaMaskSize;       // 25
    u8  AlphaMaskPos;        // 26
    u8  DirectColorModeInfo; // 27
    u8 *PhysBasePtr;         // 28
    u32 OffscreenMemOffset;  // 2C
    u16 OffscreenMemSize;    // 30
    u8  Reserved[206];       // 32
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
    u16 ResX;          //
    u16 HorzSyncStart; //
    u16 HorzSyncEnd;   //
    u16 ResY;          //
    u16 VertSyncStart; //
    u16 VertSyncEnd;   //
    u8  Flags;         //
    u32 PixelClockHz;  // pixel clock, in Hz
    u16 RefreshHz;     // refresh rate, in 0.01 Hz units
    u8  Reserved[40];  //
} CRTC_INFO;

#define CRTC_DOUBLE_SCAN    0x01 // 0 enable double scanning
#define CRTC_INTERLACED     0x02 // 1 enable interlacing
#define CRTC_HSYNC_POLARITY 0x03 // 2 (0 positive, 1 negative)
#define CRTC_VSYNC_POLARITY 0x04 // 3 (0 positive, 1 negative)

typedef struct VESA_MODE {
    u16 ResX;
    u16 ResY;
    u16 Bits;
    u16 Mode;
} VESA_MODE;

typedef struct RECT {
    int PosX;
    int PosY;
    int Wide;
    int High;
} RECT;

typedef struct BMP {
    u16 Magic;
    u32 FileSize;
    u32 Reserved;
    u32 Offset;
    u32 SizeHeader;
    s32 Width;
    s32 Height;
    u16 Planes;
    u16 BitCount;
    u32 Compression;
    u32 SizeImage;
    u32 HorzPelsPerMeter;
    u32 VertPelsPerMeter;
    u32 ColorsUsed;
    u32 ImportantColors;
} BMP;

typedef struct WAV {
  u32 Riff;
  u32 FileSize;
  u32 WaveSig;
  u32 FormatSig;
  u32 FormatSize;
  u16 AudioFormat;
  u16 Channels;
  u32 SampleRate;
  u32 ByteRate;
  u16 BlockAlign;
  u16 Bits;
  u32 DataSig;
  u32 DataSize;
} WAV;

typedef struct VESA {
    u32   ResX;
    u32   ResY;
    u32   Bits;
    u32   BytesPerLine;
    u32   HorzStart;
    u32   HorzFinish;
    u32   VertStart;
    u32   VertFinish;
    u32   PosX;
    u32   PosY;
    u32   Wide;
    u32   High;
    u8   *Pixels;
    void *Pcb;
    BMP  *Frame;
    BMP  *Font;
    u32   BackGround;
    u32   ForeGround;
    u32   FontW;
    u32   FontH;
    u32   CursorX;
    u32   CursorY;
    u32   Cols;
    u32   Rows;
} VESA;
#pragma pack(pop)

#ifndef IMPL_PCBIOS
void PcBios(int intno, BIOS *reg);
#endif IMPL_PCBIOS

#endif KERNEL_BOOTLOAD_PCBIOS_H