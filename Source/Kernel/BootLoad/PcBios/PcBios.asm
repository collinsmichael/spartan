
; compile with fasm.exe: (https://flatassembler.net/)
;     fasm PcBios.asm PcBios.bin
;
; Protected Mode BIOS Interface
; Switches between protected mode and real mode
; in order to execute a BIOS service call


; ********************************************************* ;
; moved permenantly to <Kernel/BootLoad/MultiBoot/bios.asm> ;
; ********************************************************* ;
 include '../MultiBoot/bios.asm'
