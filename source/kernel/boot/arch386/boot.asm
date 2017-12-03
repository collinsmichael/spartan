
 ;----------------------------------------------------------------;
 ; Michael Collins (2017)                                         ;
 ;                                                                ;
 ; grub multiboot wrapper for msvc based systems                  ;
 ; this approach requires rebasing the second stage to 0x00100800 ;
 ; which can be achieved with the supplied tool rebase.exe using  ;
 ;     rebase.exe boot.exe 0x00100800                             ;
 ;----------------------------------------------------------------;

 format binary as 'bin'
 org    0x00100000
 use32



 MAGIC  =  0x1BADB002
 FLAGS  =  0x00010007
 CHECK  = -(MAGIC+FLAGS)



 mbhdr:
 .magic:     dd MAGIC      ; magic
 .flags:     dd FLAGS      ; flags
 .check:     dd CHECK      ; checksum
 .header:    dd mbhdr      ; header_addr
 .text_base: dd 0x00100000 ; load_addr
 .text_end:  dd 0x00100100 ; load_end_addr
 .bss_end:   dd 0x00100200 ; bss_end_addr
 .entry:     dd boot       ; entry_addr
 .vesa_type: dd 0x00000000 ; mode_type (linear video mode)
 .vesa_resx: dd 0x00000400 ; width
 .vesa_resy: dd 0x00000300 ; height
 .vesa_bits: dd 0x00000020 ; depth



 head:
 .magic:     dd 0x00000000 ; boot magic
 .info:      dd 0x00000000 ; boot info
 .mzhdr:     dd mzhdr      ; base of second stage
 .initrd:    dd 0x00000000 ; base of ramdisk image



 boot:       push    mbhdr               ; mulitboot
             push    ebx                 ; info
             push    eax                 ; magic
             mov     [head.magic], eax   ; save boot magic
             mov     [head.info], ebx    ; save info pointer
             mov     ebx, [head.mzhdr]   ; mzhdr
             mov     esi, [ebx+0x3C]     ; pehdr
             mov     edi, [ebx+esi+0x28] ; entry point
             add     ebx, edi
             call    ebx
             jmp     $



 pad: db 0x00100800-$ dup 'x'
 mzhdr:
