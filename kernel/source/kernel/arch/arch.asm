
 format ms coff

 section '.text' code readable executable

 public _memcpy
 _memcpy: mov     edi, [esp+0x04]
          mov     esi, [esp+0x08]
          mov     ecx, [esp+0x0C]
          rep     movsb
          ret

 public _memset
 _memset: mov     edi, [esp+0x04]
          mov     eax, [esp+0x08]
          mov     ecx, [esp+0x0C]
          rep     stosb
          ret

 public   _lgdt
 _lgdt:   mov     eax, [esp+0x04]
          mov     [gdtr+2], eax
          mov     eax, [esp+0x08]
          mov     [gdtr], ax
          lgdt    [gdtr]
          jmp     0x08:.pm
 .pm:     mov     eax, 0x10
          mov     ds, ax
          mov     es, ax
          mov     fs, ax
          mov     gs, ax
          mov     ss, ax
          ret

 public   _lidt
 _lidt:   mov     eax, [esp+0x04]
          mov     [idtr+2], eax
          mov     eax, [esp+0x08]
          mov     [idtr], ax
          lidt    [idtr]
          ret

 public   _ltr
 _ltr:    mov     eax, [esp+0x04]
          ltr     ax
          ret

 public   _sti
 _sti:    sti
          ret

 public   _cli
 _cli:    cli
          ret

 public   _inb
 _inb:    mov     edx, [esp+0x04]
          xor     eax, eax
          in      al, dx
          ret

 public   _inw
 _inw:    mov     edx, [esp+0x04]
          xor     eax, eax
          in      ax, dx
          ret

 public   _ind
 _ind:    mov     edx, [esp+0x04]
          in      eax, dx
          ret


 public   _outb
 _outb:   mov     edx, [esp+0x04]
          xor     eax, eax
          mov     al, [esp+0x08]
          out     dx, al
          ret

 public   _outw
 _outw:   mov     edx, [esp+0x04]
          xor     eax, eax
          mov     ax, [esp+0x08]
          out     dx, ax
          ret

 public   _outd
 _outd:   mov     edx, [esp+0x04]
          mov     eax, [esp+0x08]
          out     dx, eax
          ret


 section '.flat' data code readable writable executable
       dw ?
 gdtr: dw ?,?,?
       dw ?
 idtr: dw ?,?,?
