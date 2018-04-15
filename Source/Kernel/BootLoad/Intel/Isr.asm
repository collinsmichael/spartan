
 ; compile with fasm.exe: (https://flatassembler.net/)
 ;     fasm isr.asm isr.obj
 ;

 format binary as 'bin'
 use32

 RING0 = 0x0010
 ERROR = 0x002C
 PAGES = 0x2000
 master:  cli                     ; disable interrupts
          nop                     ; this nop makes debugging easier
          pushad                  ; these pushes save the context to the stack
          pushd   ds              ; save data segment
          mov     edx, cr3        ; save page directory
          pushd   edx             ; save it to the stack
          pushd   RING0           ; this push activates ring0 (kernel mode) data read/write access
          pop     ds              ;
          mov     edx, PAGES      ;
          mov     cr3, edx        ;
          pushd   esp             ; callback argument (stack pointer)
          pushd   'x'             ; some exception push an error code so we inject one if needed
          call    dword ['xxxx']  ; execute callback (this is how we hook an interrupt handler)
          xchg    eax, esp        ; this is a stack switch and facilitates multi-tasking
          mov     al, 0x20        ; tell PIC that we have serviced this IRQ
          out     0x20, al        ; all IRQs pass through the master
          popd    eax             ; these pops restore the context from the stack
          mov     cr3, eax        ; activate page directory
          popd    ds              ; ring 2 (user mode) data read write access
          popad                   ; load registers
          iret                    ; return from interrupt

          db 'xxxxxxxxxxxxxxxx'   ; align for cache friendliness
          db 'xxxxxxxx'

 slave:   cli                     ; disable interrupts
          nop                     ; this nop makes debugging easier
          pushad                  ; these pushes save the context to the stack
          pushd   ds              ; save data segment
          mov     edx, cr3        ; save page directory
          pushd   edx             ; save it to the stack
          pushd   RING0           ; this push activates ring0 (kernel mode) data read/write access
          pop     ds              ;
          mov     edx, PAGES      ;
          mov     cr3, edx        ;
          pushd   esp             ; callback argument (stack pointer)
          pushd   'x'             ; some exception push an error code so we inject one if needed
          call    dword ['xxxx']  ; execute callback (this is how we hook an interrupt handler)
          xchg    eax, esp        ; this is a stack switch and facilitates multi-tasking
          mov     al, 0x20        ; tell PIC that we have serviced this IRQ
          out     0xA0, al        ; some IRQs come from the slave
          out     0x20, al        ; all IRQs pass through the master
          popd    eax             ; these pops restore the context from the stack
          mov     cr3, eax        ; activate page directory
          popd    ds              ; ring 2 (user mode) data read write access
          popad                   ; load registers
          iret                    ; return from interrupt

          db 'xxxxxxxxxxxxxxxx'   ; align for cache friendliness
          db 'xxxxxx'

 except:  cli                     ; disable interrupts
          nop                     ; this nop makes debugging easier
          pushad                  ; these pushes save the context to the stack
          pushd   ds              ; save data segment
          mov     edx, cr3        ; save page directory
          pushd   edx             ; save it to the stack
          pushd   RING0           ; this push activates ring0 (kernel mode) data read/write access
          pop     ds              ;
          mov     edx, PAGES      ;
          mov     cr3, edx        ;
          pushd   esp             ; callback argument (stack pointer)
          pushd   [esp+ERROR]     ; callback argument (error code)
          call    dword ['xxxx']  ; execute callback (this is how we hook an interrupt handler)
          xchg    eax, esp        ; this is a stack switch and facilitates multi-tasking
          mov     al, 0x20        ; tell PIC that we have serviced this IRQ
          out     0x20, al        ; all IRQs pass through the master
          popd    eax             ; these pops restore the context from the stack
          mov     cr3, eax        ; activate page directory
          popd    ds              ; ring 2 (user mode) data read write access
          popad                   ; load registers
          popd    [esp]           ; discard error code
          iret                    ; return from interrupt

          db 'xxxxxxxxxxxxxxxx'   ; align for cache friendliness
          db 'xxx'
