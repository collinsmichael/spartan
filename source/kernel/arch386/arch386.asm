
 format ms coff

 section '.text' code readable executable
 public _inb
 _inb:  pushd   edx
        xor     eax, eax
        mov     edx, [esp+0x08]
        in      al, dx
        popd    edx
        ret

 public _inw
 _inw:  pushd   edx
        xor     eax, eax
        mov     edx, [esp+0x08]
        in      ax, dx
        popd    edx
        ret

 public _ind
 _ind:  pushd   edx
        mov     edx, [esp+0x08]
        in      eax, dx
        popd    edx
        ret

 public _outb
 _outb: pushd   edx
        xor     eax, eax
        mov     edx, [esp+0x08]
        mov     al, [esp+0x0C]
        out     dx, al
        popd    edx
        ret

 public _outw
 _outw: pushd   edx
        xor     eax, eax
        mov     edx, [esp+0x08]
        mov     ax, [esp+0x0C]
        out     dx, ax
        popd    edx
        ret

 public _outd
 _outd: pushd   edx
        mov     edx, [esp+0x08]
        mov     eax, [esp+0x0C]
        out     dx, eax
        popd    edx
        ret
