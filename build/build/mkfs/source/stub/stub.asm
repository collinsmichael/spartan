 ;----------------;--------------------------------;--------------------------------------;
 ; memory map     ;  first   -  final   -  length  ;  comments                            ;
 ;----------------;--------------------------------;--------------------------------------;
 ; ivt:           ; 00000000 - 000003FF - 00000400 ; interrupt vector table               ;
 ; bda:           ; 00000400 - 000004FF - 00000100 ; bios data area                       ;
 ; boot loader    ; 00000500 - 000007FF - 00000300 ; boot loader                          ;
 ; ram disk       ; 00000800 - 0007FFFF - 0007FA00 ; free for use                         ;
 ;                ; 00080000 - 0009FBFF - 00080000 ; free for use, if it exists           ;
 ; xbda:          ; 0009FC00 - 0009FFFF - 00000400 ; extended bios data area              ;
 ; vram:          ; 000A0000 - 000FFFFF - 00060000 ; video memory                         ;
 ;-------------------------------------------------;--------------------------------------;
 ; user:          ; 00100000 - ???????? - ???????? ; user memory area                     ;
 ;-------------------------------------------------;--------------------------------------;

 format    binary as 'exe'
 org       0x1000

 mzhdr:
 .e_magic:    dw 0x5A4D
 .e_cblp:     dw 0x0040
 .e_cp:       dw 0x0009
 .e_crlc:     dw 0x0000
 .e_cparhdr:  dw (boot-mzhdr)/16
 .e_minalloc: dw 0x0010
 .e_maxalloc: dw 0xFFFF
 .e_ss:       dw 0x0102
 .e_sp:       dw 0xFFF0
 .e_csum:     dw 0x0001
 .e_ip:       dw 0x0000
 .e_cs:       dw 0x0000
 .e_lsarlc:   dw 0x0002
 .e_ovno:     dw 0x001C
 .e_res:      dw 0x0000, 0x0000, 0x0000, 0x0000
 .e_oemid:    dw 0x0000
 .e_oeminfo:  dw 0x0000
 .e_res2:     dw 0x0000, 0x0000, 0x0000, 0x0000
              dw 0x0000, 0x0000, 0x0000, 0x0000
              dw 0x0000, 0x0000
 .e_lfanew:   dd 0x00000000
 boot:  use16
