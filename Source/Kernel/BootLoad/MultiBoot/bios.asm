
 ; compile with fasm.exe: (https://flatassembler.net/)
 ;     fasm bios.asm bios.bin
 ;
 ; I'm not explaining anything in depth, it would take forever.
 ; Our kernel runs in Protected Mode. To execute a PC BIOS service
 ; it is required to first return to 16-bit Real Mode, then once
 ; the BIOS service has been executed the system must transition
 ; back into Protected Mode. This is basically what this file does.
 ; A tiny bit of self modifying code is performed to achieve this.
 ; Not all BIOS services are available using this method.
 ; Notably any service which uses DMA is broken
 ;

 format binary  as 'bin'
 org    0x1000
 use32


 GDT_NULL       = (0x00+0)
 GDT_RING0_CODE = (0x08+0)
 GDT_RING0_DATA = (0x10+0)
 GDT_RING3_CODE = (0x18+3)
 GDT_RING3_DATA = (0x20+3)
 GDT_RM16_CODE  = (0x28+0)
 GDT_RM16_DATA  = (0x30+0)
 IDTR = 0x0F4A
 IVTR = 0x0F52

 bios:  cli
        lidt   [IVTR]
        mov    eax, [esp+0x04]
        mov    [_int_], al
        pushad
        pushd  ds
        pushd  es
        pushd  fs
        pushd  gs

        mov    ax, GDT_RING0_DATA
        mov    ds, ax
        mov    es, ax
        mov    fs, ax
        mov    gs, ax

        mov    esi, [esp+0x38]
        mov    edi, reg32
        mov    ecx, length
        rep    movsb

        call   int86

        mov    edi, [esp+0x38]
        mov    esi, reg32
        mov    ecx, length
        rep    movsb

        popd   gs
        popd   fs
        popd   es
        popd   ds
        popad
        lidt   [IDTR]
        sti
        ret

 int86: mov    [_esp_], esp
        jmp    GDT_RM16_CODE:rm16

 ret86: mov    ax, GDT_RING0_DATA
        mov    ds, ax
        mov    es, ax
        mov    fs, ax
        mov    gs, ax
        mov    ss, ax
        mov    eax, cr0
        or     eax, 0x80000001
        mov    cr0, eax
        mov    esp, [_esp_]
        ret

 use16
 rm16:  mov    eax, cr0
        and    eax, 0x7FFFFFFE
        mov    cr0, eax
        jmp    0x0000:rmx

 rmx:   mov    ax, GDT_RM16_DATA
        mov    ds, ax
        mov    es, ax
        mov    fs, ax
        mov    gs, ax
        mov    ss, ax
        mov    esp, 0x0000FB00
        xor    ax, ax
        mov    ds, ax
        mov    es, ax
        mov    fs, ax
        mov    gs, ax

        mov    ax, 0x9000
        mov    ss, ax

        mov    eax, [_eax_]
        mov    ecx, [_ecx_]
        mov    edx, [_edx_]
        mov    ebx, [_ebx_]
        mov    ebp, [_ebp_]
        mov    esi, [_esi_]
        mov    edi, [_edi_]

        sti
        int    0x00
        _int_ = $-1
        cli

        mov    [_eax_], eax
        mov    [_ecx_], ecx
        mov    [_edx_], edx
        mov    [_ebx_], ebx
        mov    [_ebp_], ebp
        mov    [_esi_], esi
        mov    [_edi_], edi
        pushf
        pop    word [eflag]

        mov    eax, cr0
        or     eax, 0x00000001
        mov    cr0, eax
        jmp    GDT_RING0_CODE:ret86

 align 4

 reg32:
 _eax_: dd 0 ; eax
 _ecx_: dd 0 ; ecx
 _edx_: dd 0 ; edx
 _ebx_: dd 0 ; ebx
 _esp_: dd 0 ; esp
 _ebp_: dd 0 ; ebp
 _esi_: dd 0 ; esi
 _edi_: dd 0 ; edi
 eflag: dd 0 ; eflags

 length = $-reg32
