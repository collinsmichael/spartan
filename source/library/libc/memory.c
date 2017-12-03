_declspec(dllexport)
void *memcpy(void *dst, const void *src, unsigned long len) {
    _asm mov  edi, [dst]
    _asm mov  esi, [src]
    _asm mov  eax, [len]
    _asm mov  ecx, edi
    _asm and  ecx, 0x03
    _asm sub  eax, ecx
    _asm cld
    _asm rep  movsb
    _asm mov  ecx, eax
    _asm and  eax, 0x03
    _asm shr  ecx, 0x02
    _asm rep  movsd
    _asm xchg eax, ecx
    _asm rep  movsb
    return dst;
}

_declspec(dllexport)
void *memset(void *dst, int val, unsigned long len) {
    _asm mov  edi, [dst]
    _asm mov  esi, [len]
    _asm xor  eax, eax
    _asm mov  al, byte ptr [val]
    _asm mov  ecx, 0x01010101
    _asm imul ecx
    _asm mov  ecx, edi
    _asm and  ecx, 0x03
    _asm sub  esi, ecx
    _asm cld
    _asm rep  stosb
    _asm mov  ecx, esi
    _asm and  esi, 0x03
    _asm shr  ecx, 0x02
    _asm rep  stosd
    _asm xchg esi, ecx
    _asm rep  stosb
    return dst;
}