
 ; compile with fasm.exe: (https://flatassembler.net/)
 ;     fasm boot.asm boot.bin
 ;
 ; grub multiboot wrapper for msvc based systems
 ; this approach requires rebasing the second stage to 0x00101000
 ; which can be achieved with the supplied tool rebase.exe using
 ;     rebase.exe boot.exe 0x00101000

 format binary as 'bin'
 org    0x00100000
 use32

 MAGIC  =  0x1BADB002
 FLAGS  =  0x00010007
 CHECK  = -(MAGIC+FLAGS)
 KMAIN  = 0x00101040

 mbhdr:
 .magic:     dd MAGIC      ; magic
 .flags:     dd FLAGS      ; flags
 .check:     dd CHECK      ; checksum
 .header:    dd mbhdr      ; header_addr
 .text_base: dd 0x00100000 ; load_addr
 .text_end:  dd 0x00100800 ; load_end_addr
 .bss_end:   dd 0x00101000 ; bss_end_addr
 .entry:     dd boot       ; entry_addr

 ; 80x25 text mode
 ;.vesa_type: dd 0x00000001 ; mode_type (linear video mode)
 ;.vesa_resx: dd 0x00000000 ; width
 ;.vesa_resy: dd 0x00000000 ; height
 ;.vesa_bits: dd 0x00000000 ; depth

 ; 800x600
 .vesa_type: dd 0x00000000 ; mode_type (linear video mode)
 .vesa_resx: dd 0x00000320 ; width
 .vesa_resy: dd 0x00000258 ; height
 .vesa_bits: dd 0x00000020 ; depth

 ; 1024x768
 ;.vesa_type: dd 0x00000000 ; mode_type (linear video mode)
 ;.vesa_resx: dd 0x00000400 ; width
 ;.vesa_resy: dd 0x00000300 ; height
 ;.vesa_bits: dd 0x00000020 ; depth

 head:
 .magic:     dd 0x00000000 ; boot magic
 .info:      dd 0x00000000 ; boot info
 .mzhdr:     dd mzhdr      ; base of second stage
 .initrd:    dd 0x00000000 ; base of ramdisk image

 MMAP_XBDA    = 0x00090000

 boot:       mov     esp, MMAP_XBDA-16   ;
             push    mbhdr               ; mulitboot
             push    ebx                 ; info
             push    eax                 ; magic
             mov     [head.magic], eax   ; save boot magic
             mov     [head.info], ebx    ; save info pointer

            ;mov     ebx, [head.mzhdr]   ; mzhdr
            ;mov     esi, [ebx+0x3C]     ; pehdr
            ;mov     edi, [ebx+esi+0x28] ; entry point
            ;add     ebx, edi
            ;call    ebx
             call    dword [KMAIN]
             jmp     $


      db 0x00100100-$ dup 'x'
      file 'bios.bin'
 pad: db 0x00101000-$ dup 'x'
 mzhdr: