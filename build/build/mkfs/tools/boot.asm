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
 org       0x500

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


 ; ************************************* ;
 ; set up vga text video mode 80x25 4bpp ;
 boot:  use16
        xor     ax, ax
        mov     ds, ax
        mov     es, ax
        mov     fs, ax
        mov     gs, ax
        mov     [mzhdr.e_csum], dx
        sti
        cld
        mov     ax, 0x0003
        int     0x10
        mov     ax, 0x0100
        mov     cx, 0x2000
        int     0x10
        jmp     0x0000:init


 ; ******************** ;
 ; enter protected mode ;
 init:  cli
        lgdt    [gdtr]                  
        mov     eax, cr0
        or      al, 0x01
        mov     cr0, eax
        jmp     SYSTEM_CODE:pm32
 pm32:  use32
        mov     ax, SYSTEM_DATA
        mov     ds, ax
        mov     es, ax
        mov     fs, ax
        mov     gs, ax
        mov     ss, ax
        KB = 1024
        MB = 1024*KB
        mov     esp, 64*MB - 16

        mov     esi, volume
        mov     edi, 1*MB
        mov     ecx, [volume.disk_space]
        shr     ecx, 2
        rep     movsd

        ; ******************************************** ;
        ; locate the kernel and call its main function ;
       ;KernelImageBase = 0x00010000
       ;mov     ebx, KernelImageBase
        mov     ebx, [volume.entry]
       ;mov     esi, [ebx + 0x3C]
       ;mov     edi, [ebx + esi + 0x28] ; EntryPoint
       ;add     ebx, edi
        xor     eax, eax
        xor     ecx, edx
        xor     edx, edx
        xor     ebp, ebp
        xor     esi, esi
        xor     edi, edi
        call    ebx
        jmp     $




 macro align ment*,dat {
     local a
     if ~ dat eq
         virtual
             align ment
             a = $-$$ 
         end virtual
         db a dup dat
     else
         align ment
     end if
 }

        align 64, 0
 gdt:   dq 0x0000000000000000
        dq 0x00CF9A000000FFFF
        dq 0x00CF92000000FFFF
        dq 0x00CFFA000000FFFF
        dq 0x00CFF2000000FFFF
        SYSTEM_CODE = 0x08
        SYSTEM_DATA = 0x10
        USER_CODE   = 0x18
        USER_DATA   = 0x20
 .fin:  dw 0
 gdtr:  dw gdt.fin-gdt-1
        dd gdt
        align 256, 'x'


 volume:
 .blocks      dd ?
 .inodes      dd ?
 .root        dd ?
 .entry       dd ?
 .disk_space  dd ?
 .disk_used   dd ?
 .disk_free   dd ?
 .max_blocks  dd ?
 .max_nodes   dd ?
 .node_count  dd ?
 .linker      dd ?
 .padding     db 20 dup ?
 .label       db 64 dup ?
 .author      db 64 dup ?
 .publisher   db 64 dup ?
 align 256, ?
