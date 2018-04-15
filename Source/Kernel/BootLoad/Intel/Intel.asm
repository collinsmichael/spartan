
 ; compile with fasm.exe: (https://flatassembler.net/)
 ;     fasm Intel.asm Intel.obj
 ;
 ; this is a dumping ground for privileged opcodes
 ; which cannot be inlined with a msvc compiler

 format ms coff

 section '.text' code readable executable

  public    _cpuid
 _cpuid:   mov     eax, [esp+0x04]
           mov     edi, [esp+0x08]
           cpuid
           mov     [edi+0x00], eax
           mov     [edi+0x04], ebx
           mov     [edi+0x08], ecx
           mov     [edi+0x0C], edx
           ret

 public    _swint
 _swint:   pusha
           mov     eax, [esp+0x24]
           mov     [.xx], al
           db 0xCD
 .xx:      db 0x00
           popa
           ret

 public    _stosd
 _stosd:   pusha
           mov     edi, [esp+0x24]
           mov     eax, [esp+0x28]
           mov     ecx, [esp+0x2C]
           rep     stosd
           popa
           ret

 public    _stosw
 _stosw:   pusha
           mov     edi, [esp+0x24]
           mov     eax, [esp+0x28]
           mov     ecx, [esp+0x2C]
           rep     stosw
           popa
           ret

 public    _stosb
 _stosb:   pusha
           mov     edi, [esp+0x24]
           mov     eax, [esp+0x28]
           mov     ecx, [esp+0x2C]
           rep     stosb
           popa
           ret

 public    _movsd
 _movsd:   pusha
           mov     edi, [esp+0x24]
           mov     esi, [esp+0x28]
           mov     ecx, [esp+0x2C]
           rep     movsd
           popa
           ret

 public    _movsw
 _movsw:   pusha
           mov     edi, [esp+0x24]
           mov     esi, [esp+0x28]
           mov     ecx, [esp+0x2C]
           rep     movsw
           popa
           ret

 public    _movsb
 _movsb:   pusha
           mov     edi, [esp+0x24]
           mov     esi, [esp+0x28]
           mov     ecx, [esp+0x2C]
           rep     movsb
           popa
           ret

 public    _cmpsd
 _cmpsd:   pusha
           mov     edi, [esp+0x24]
           mov     esi, [esp+0x28]
           mov     ecx, [esp+0x2C]
           rep     cmpsd
           sub     edi, 0x04
           mov     [esp+0x1C], edi
           popa
           ret

 public    _cmpsw
 _cmpsw:   pusha
           mov     edi, [esp+0x24]
           mov     esi, [esp+0x28]
           mov     ecx, [esp+0x2C]
           rep     cmpsw
           sub     edi, 0x02
           mov     [esp+0x1C], edi
           popa
           ret

 public    _cmpsb
 _cmpsb:   pusha
           mov     edi, [esp+0x24]
           mov     esi, [esp+0x28]
           mov     ecx, [esp+0x2C]
           rep     cmpsb
           dec     edi
           mov     [esp+0x1C], edi
           popa
           ret

 public    _scasd
 _scasd:   pusha
           mov     edi, [esp+0x24]
           mov     eax, [esp+0x28]
           mov     ecx, [esp+0x2C]
           rep     scasd
           sub     edi, 0x04
           mov     [esp+0x1C], edi
           popa
           ret

 public    _scasw
 _scasw:   pusha
           mov     edi, [esp+0x24]
           mov     eax, [esp+0x28]
           mov     ecx, [esp+0x2C]
           rep     scasw
           sub     edi, 0x02
           mov     [esp+0x1C], edi
           popa
           ret

 public    _scasb
 _scasb:   pusha
           mov     edi, [esp+0x24]
           mov     eax, [esp+0x28]
           mov     ecx, [esp+0x2C]
           rep     scasb
           dec     edi
           mov     [esp+0x1C], edi
           popa
           ret

 public    _rol
 _rol:     pusha
           mov     eax, [esp+0x24]
           mov     ecx, [esp+0x28]
           rol     eax, cl
           mov     [esp+0x1C], eax
           popa
           ret

 public    _ror
 _ror:     pusha
           mov     eax, [esp+0x24]
           mov     ecx, [esp+0x28]
           ror     eax, cl
           mov     [esp+0x1C], eax
           popa
           ret

 GDT_NULL       = (0x00+0)
 GDT_RING0_CODE = (0x08+0)
 GDT_RING0_DATA = (0x10+0)
 GDT_RING3_CODE = (0x18+3)
 GDT_RING3_DATA = (0x20+3)
 GDT_RM16_CODE  = (0x28+3)
 GDT_RM16_DATA  = (0x30+3)
 gdtr = 0x0F42
 idtr = 0x0F4A

 public    _lgdt
 _lgdt:    mov     eax, [esp+0x04]
           lgdt    [eax]
           jmp     GDT_RING0_CODE:.pm
 .pm:      mov     eax, GDT_RING0_DATA
           mov     ds, ax
           mov     es, ax
           mov     fs, ax
           mov     gs, ax
           mov     ss, ax
           ret

 public    _lidt
 _lidt:    mov     eax, [esp+0x04]
           lidt    [eax]
           ret

 public    _ltr
 _ltr:     mov     eax, [esp+0x04]
           ltr     ax
           ret

 public    _sti
 _sti:     sti
           ret

 public    _cli
 _cli:     cli
           ret

 public    _inb
 _inb:     mov     edx, [esp+0x04]
           xor     eax, eax
           in      al, dx
           ret

 public    _inw
 _inw:     mov     edx, [esp+0x04]
           xor     eax, eax
           in      ax, dx
           ret

 public    _ind
 _ind:     mov     edx, [esp+0x04]
           in      eax, dx
           ret


 public    _outb
 _outb:    mov     edx, [esp+0x04]
           xor     eax, eax
           mov     al, [esp+0x08]
           out     dx, al
           ret

 public    _outw
 _outw:    mov     edx, [esp+0x04]
           xor     eax, eax
           mov     ax, [esp+0x08]
           out     dx, ax
           ret

 public    _outd
 _outd:    mov     edx, [esp+0x04]
           mov     eax, [esp+0x08]
           out     dx, eax
           ret

 public    _get_cr0
 _get_cr0: mov     eax, cr0
           ret

 public    _set_cr0
 _set_cr0: mov     eax, [esp+0x04]
           mov     cr0, eax
           ret

 public    _get_cr2
 _get_cr2: mov     eax, cr2
           ret

 public    _set_cr2
 _set_cr2: mov     eax, [esp+0x04]
           mov     cr2, eax
           ret

 public    _set_cr3
 _set_cr3: mov     eax, [esp+0x04]
           mov     cr3, eax
           ret

 public    _get_cr3
 _get_cr3: mov     eax, cr3
           ret

 public    _get_cr4
 _get_cr4: mov     eax, cr4
           ret

 public    _set_cr4
 _set_cr4: mov     eax, [esp+0x04]
           mov     cr4, eax
           ret

 public    _rdtsc
 _rdtsc:   rdtsc
           ret

 public    _ring3
 _ring3:   mov     esp, [esp+0x04]  ; context
           pop     eax              ; cr3 (page directory)
           mov     cr3, eax         ;
           pop     eax              ; ring3 data segment
           mov     ds, ax
           mov     es, ax
           mov     fs, ax
           mov     gs, ax
           popa
           iretd

 public    _int386
 _int386:  mov     eax, [esp+0x04]
           mov     [INTA], al
           mov     eax, [esp+0x08]
           pushad
           int     0x80
           INTA    =$-1
           popad
           mov     eax, [eax]
           ret

 public    _int32
 _int32:   mov     eax, [esp+0x04]
           mov     [INTB], al
           pushad
           int     0x80
           INTB    =$-1
           popad
           ret
