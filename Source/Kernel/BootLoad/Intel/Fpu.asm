
 ; compile with fasm.exe: (https://flatassembler.net/)
 ;     fasm Intel.asm Intel.obj
 ;
 ; this is a dumping ground for privileged opcodes
 ; which cannot be inlined with a msvc compiler

 format ms coff

 section '.text' code readable executable
 public    _finit
 _finit:   finit
           ret

 public    _fninit
 _fninit:  fninit
           ret

 public    _fnstsw
 _fnstsw:  mov     eax, [esp+0x04]
           fnstsw  word [eax]
           ret

 public    _fldcw
 _fldcw:   fldcw   word [esp+0x04]
           ret

 public    _fnsave
 _fnsave:  mov     eax, [esp+0x04]
           fnsave  [eax]
           ret

 public    _frstor
 _frstor:  mov     eax, [esp+0x04]
           frstor  [eax]
           ret

 public    _fxsave
 _fxsave:  mov     eax, [esp+0x04]
           fxsave  [eax]
           ret

 public    _fxrstor
 _fxrstor: mov     eax, [esp+0x04]
           fxrstor [eax]
           ret

 public    _ldmxcsr
 _ldmxcsr: mov     eax, [esp+0x04]
           ldmxcsr [eax]
           ret

 public    _stmxcsr
 _stmxcsr: mov     eax, [esp+0x04]
           ldmxcsr [eax]
           ret

           public    _clts
 _clts:    clts
           ret
