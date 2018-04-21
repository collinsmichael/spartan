; Listing generated by Microsoft (R) Optimizing Compiler Version 18.00.40629.0 

	TITLE	C:\Users\cex123\Desktop\FYP\develop\spartan\Source\Kernel\Thread\Process\Thread.c
	.686P
	.XMM
	include listing.inc
	.model	flat

INCLUDELIB OLDNAMES

PUBLIC	_Thread
PUBLIC	??_C@_0BB@CFKEFJPP@System?1Shell?4dll?$AA@	; `string'
PUBLIC	??_C@_0BMD@DEJPFPA@?5Thread?9?$DOName?5?5?5?5?5?5?5?$DN?5?$CFs?6?5Thread@ ; `string'
PUBLIC	_thread
;	COMDAT ??_C@_0BMD@DEJPFPA@?5Thread?9?$DOName?5?5?5?5?5?5?5?$DN?5?$CFs?6?5Thread@
CONST	SEGMENT
??_C@_0BMD@DEJPFPA@?5Thread?9?$DOName?5?5?5?5?5?5?5?$DN?5?$CFs?6?5Thread@ DB ' '
	DB	'Thread->Name       = %s', 0aH, ' Thread->Tss        = %X', 0aH
	DB	' Thread->Node       = %X', 0aH, ' Thread->Data       = %X', 0aH
	DB	' Thread->Gran       = %X', 0aH, ' Thread->Count      = %X', 0aH
	DB	' Thread->Next       = %X', 0aH, ' Thread->ReadyQ     = %X', 0aH
	DB	' Thread->BlockQ     = %X', 0aH, ' Thread->CtxList    = %X', 0aH
	DB	' Thread->PcbList    = %X', 0aH, ' Thread->PebList    = %X', 0aH
	DB	' Thread->ReadyHead  = %X', 0aH, ' Thread->ReadyTail  = %X', 0aH
	DB	' Thread->BlockHead  = %X', 0aH, ' Thread->BlockTail  = %X', 0aH
	DB	' Thread->ReadyQ[%d] = %X', 0aH, ' Thread->BlockQ[%d] = %X', 0aH
	DB	00H						; `string'
CONST	ENDS
;	COMDAT ??_C@_0BB@CFKEFJPP@System?1Shell?4dll?$AA@
CONST	SEGMENT
??_C@_0BB@CFKEFJPP@System?1Shell?4dll?$AA@ DB 'System/Shell.dll', 00H ; `string'
_Thread	DD	FLAT:_thread
_thread	DD	FLAT:_IThread_Debug
	DD	FLAT:_IThread_Status
	DD	FLAT:_IThread_GetPid
	DD	FLAT:_IThread_Fork
	DD	FLAT:_IThread_Spawn
	DD	FLAT:_IThread_Run
	DD	FLAT:_IThread_Yield
	DD	FLAT:_IThread_Exit
	DD	FLAT:_IThread_Kill
	DD	FLAT:_IThread_Block
	DD	FLAT:_IThread_Unblock
_DATA	ENDS
PUBLIC	_IThread_Status
PUBLIC	_IThread_Debug
PUBLIC	_IThread_Yield
PUBLIC	_IThread_Fork
PUBLIC	_IThread_Run
PUBLIC	_IThread_Unblock
PUBLIC	_IThread_Block
PUBLIC	_IThread_Kill
PUBLIC	_IThread_Exit
PUBLIC	_IThread_Killer
PUBLIC	_IThread_Spawn
PUBLIC	_IThread_GetPid
PUBLIC	_DoYield
PUBLIC	_DoKill
_Canvas	DD	01H DUP (?)
_cursor	DD	01H DUP (?)
_screen	DD	01H DUP (?)
_BSS	ENDS
; Function compile flags: /Ogspy
; File c:\users\cex123\desktop\fyp\develop\spartan\source\kernel\thread\process\thread.c
_TEXT	SEGMENT
_esp$1$ = -4						; size = 4
_DoKill	PROC
; _err$dead$ = ecx
; _esp$ = edx

; 63   : u32 DoKill(u32 err, u32 esp) {

	push	ecx

; 64   :     CTask *TaskPool = (CTask*)MMAP_TASKPOOL;
; 65   :     while (TaskPool->KillHead != TaskPool->KillTail) {

	mov	eax, DWORD PTR ds:1732
	push	edi
	mov	edi, edx
	mov	DWORD PTR _esp$1$[esp+8], edi
	cmp	eax, DWORD PTR ds:1736
	je	$LN54@DoKill

; 67   :         TaskPool->KillQ[TaskPool->KillHead] = 0;

	push	ebx
	push	ebp
	push	esi
	mov	esi, 1712				; 000006b0H
	xor	ebx, ebx
	lea	ebp, DWORD PTR [esi+20]
	lea	edx, DWORD PTR [esi+24]
$LL55@DoKill:

; 66   :         int pid = TaskPool->KillQ[TaskPool->KillHead];

	mov	eax, DWORD PTR [esi]
	mov	ecx, DWORD PTR [ebp]
	mov	esi, DWORD PTR [eax+ecx*4]

; 67   :         TaskPool->KillQ[TaskPool->KillHead] = 0;

	mov	DWORD PTR [eax+ecx*4], ebx

; 68   :         TaskPool->KillHead = ++TaskPool->KillHead % (4*KB);

	mov	eax, DWORD PTR [ebp]
	inc	eax
	and	eax, -2147479553			; 80000fffH
	jns	SHORT $LN526@DoKill
	dec	eax
	or	eax, -4096				; fffff000H
	inc	eax
$LN526@DoKill:
	mov	DWORD PTR [ebp], eax

; 69   :         if (pid <= 0) continue;

	test	esi, esi
	jle	$LN461@DoKill

; 70   : 
; 71   :         IThread_Debug();
; 72   : 
; 73   :         Logger("DoKill (Blocking Pid %d)\n", pid);
; 74   :         IThread_Block(pid);

	push	esi
	call	_IThread_Block

; 75   :         IThread_Debug();
; 76   : 
; 77   :         Logger("Flushing Blocked Q (%d)\n", pid);
; 78   :         for (int i = 0; i < TaskPool->KillTail; i++) {

	mov	eax, 1736				; 000006c8H
	mov	edx, ebx
	pop	ecx
	cmp	DWORD PTR [eax], ebx
	jle	SHORT $LN382@DoKill

; 82   :                 TaskPool->BlockQ[TaskPool->BlockTail] = 0;

	lea	ebp, DWORD PTR [eax-8]
	lea	edi, DWORD PTR [eax-28]
$LL46@DoKill:

; 79   :             if (TaskPool->BlockQ[i] == pid) {

	mov	ecx, DWORD PTR [edi]
	cmp	DWORD PTR [ecx+edx*4], esi
	jne	SHORT $LN45@DoKill

; 80   :                 int tail = TaskPool->BlockQ[--TaskPool->BlockTail];

	dec	DWORD PTR [ebp]
	mov	eax, DWORD PTR [ebp]

; 81   :                 TaskPool->BlockQ[i] = tail;

	mov	eax, DWORD PTR [ecx+eax*4]
	mov	DWORD PTR [ecx+edx*4], eax

; 82   :                 TaskPool->BlockQ[TaskPool->BlockTail] = 0;

	mov	eax, DWORD PTR [edi]
	mov	ecx, DWORD PTR [ebp]
	mov	DWORD PTR [eax+ecx*4], ebx
	mov	eax, 1736				; 000006c8H

; 83   :                 TaskPool->BlockHead = 0;

	mov	DWORD PTR ds:1724, ebx
$LN45@DoKill:

; 75   :         IThread_Debug();
; 76   : 
; 77   :         Logger("Flushing Blocked Q (%d)\n", pid);
; 78   :         for (int i = 0; i < TaskPool->KillTail; i++) {

	inc	edx
	cmp	edx, DWORD PTR [eax]
	jl	SHORT $LL46@DoKill
	mov	ebp, 1732				; 000006c4H

; 84   :             }
; 85   :         }
; 86   :         IThread_Debug();

$LN382@DoKill:

; 87   : 
; 88   :         for (int i = 0; i < 32; i++) Logger("%c", TaskPool->Node[i]); Logger("\n");
; 89   :         Pool->Release((CPool*)TaskPool, pid);

	mov	eax, DWORD PTR _Pool
	push	esi
	push	1664					; 00000680H
	call	DWORD PTR [eax+12]

; 90   :         for (int i = 0; i < 32; i++) Logger("%c", TaskPool->Node[i]); Logger("\n");
; 91   : 
; 92   :         CPcb *pcb = (CPcb*)TaskPool->PcbList[pid];

	mov	eax, DWORD PTR ds:1696
	pop	ecx
	pop	ecx
	mov	edx, DWORD PTR [eax+esi*4]

; 93   :         CPeb *peb = (CPeb*)TaskPool->PebList[pid];

	mov	eax, DWORD PTR ds:1700
	mov	eax, DWORD PTR [eax+esi*4]

; 94   :         u32  *esp = (u32 *)TaskPool->CtxList[pid];
; 95   :     
; 96   :         Logger(" [info] Process %d pcb=%X peb=%X\n", pid, pcb, peb);
; 97   :         if (peb) {

	test	eax, eax
	je	SHORT $LN8@DoKill

; 98   :             CCanvas *canvas = peb->Canvas;

	mov	eax, DWORD PTR [eax+16]

; 99   :             Logger(" [info] Process %d canvas=%X\n", pid, canvas);
; 100  :             if (canvas) {

	test	eax, eax
	je	SHORT $LN8@DoKill

; 101  :                 CWindow *frame = canvas->TopMost;

	mov	eax, DWORD PTR [eax+8]

; 102  :                 Logger(" [info] Process %d frame=%X\n", pid, frame);
; 103  :                 if (frame) {

	test	eax, eax
	je	SHORT $LN8@DoKill

; 104  :                     CWindow *prev = frame->Prev;

	mov	ecx, DWORD PTR [eax]

; 105  :                     CWindow *next = frame->Next;

	mov	eax, DWORD PTR [eax+4]

; 106  :                     Logger(" [info] Process %d prev=%X next=%X\n", pid, prev, next);
; 107  :                     if (prev) prev->Next = next;

	test	ecx, ecx
	je	SHORT $LN9@DoKill
	mov	DWORD PTR [ecx+4], eax
$LN9@DoKill:

; 108  :                     if (next) next->Prev = prev;

	test	eax, eax
	je	SHORT $LN8@DoKill
	mov	DWORD PTR [eax], ecx
$LN8@DoKill:

; 109  :                 }
; 110  :             }
; 111  :         }
; 112  : 
; 113  :         if (screen) {

	mov	eax, DWORD PTR _screen
	test	eax, eax
	je	SHORT $LN4@DoKill

; 114  :             for (CWindow *list = screen->TopMost; list; list = list->Next) {

	mov	eax, DWORD PTR [eax+8]
	jmp	SHORT $LN527@DoKill
$LL6@DoKill:

; 115  :                 list->Dirty = 2;

	mov	DWORD PTR [eax+60], 2
	mov	eax, DWORD PTR [eax+4]
$LN527@DoKill:

; 114  :             for (CWindow *list = screen->TopMost; list; list = list->Next) {

	test	eax, eax
	jne	SHORT $LL6@DoKill
$LN4@DoKill:

; 116  :             }
; 117  :         }
; 118  : 
; 119  :         SysMem->Free((void*)pcb->Binary);

	mov	eax, DWORD PTR _SysMem
	push	DWORD PTR [edx+8]
	call	DWORD PTR [eax+4]

; 120  :         TaskPool->PcbList[pid] = 0;

	mov	eax, DWORD PTR ds:1696
	mov	edx, 1736				; 000006c8H
	pop	ecx
	mov	DWORD PTR [eax+esi*4], ebx

; 121  :         TaskPool->PebList[pid] = 0;

	mov	eax, DWORD PTR ds:1700
	mov	DWORD PTR [eax+esi*4], ebx

; 122  :         TaskPool->CtxList[pid] = 0;

	mov	eax, DWORD PTR ds:1692
	mov	DWORD PTR [eax+esi*4], ebx
$LN461@DoKill:

; 64   :     CTask *TaskPool = (CTask*)MMAP_TASKPOOL;
; 65   :     while (TaskPool->KillHead != TaskPool->KillTail) {

	mov	eax, DWORD PTR [ebp]
	mov	esi, 1712				; 000006b0H
	cmp	eax, DWORD PTR [edx]
	jne	$LL55@DoKill
	mov	edi, DWORD PTR _esp$1$[esp+20]
	pop	esi
	pop	ebp
	pop	ebx
$LN54@DoKill:

; 123  :         Logger(" [info] Process %d terminated\n", pid);
; 124  :     }
; 125  :     return esp;

	mov	eax, edi
	pop	edi

; 126  : }

	pop	ecx
	ret	0
_DoKill	ENDP
_TEXT	ENDS
; Function compile flags: /Ogspy
; File c:\users\cex123\desktop\fyp\develop\spartan\source\kernel\thread\process\thread.c
_TEXT	SEGMENT
_DoYield PROC
; _err$dead$ = ecx
; _esp$ = edx

; 214  :     return RoundRobin(err, esp);

	jmp	_RoundRobin
_DoYield ENDP
_TEXT	ENDS
; Function compile flags: /Ogspy
; File c:\users\cex123\desktop\fyp\develop\spartan\source\kernel\thread\process\thread.c
_TEXT	SEGMENT
_IThread_GetPid PROC

; 23   :     CTask *TaskPool = (CTask*)MMAP_TASKPOOL;
; 24   :     return TaskPool->ReadyQ[TaskPool->ReadyHead];

	mov	ecx, DWORD PTR ds:1716
	mov	eax, DWORD PTR ds:1704
	mov	eax, DWORD PTR [eax+ecx*4]

; 25   : }

	ret	0
_IThread_GetPid ENDP
_TEXT	ENDS
; Function compile flags: /Ogspy
; File c:\users\cex123\desktop\fyp\develop\spartan\source\kernel\thread\process\thread.c
_TEXT	SEGMENT
_cr3$ = 8						; size = 4
_eip$ = 12						; size = 4
_esp0$ = 16						; size = 4
_esp3$ = 20						; size = 4
_IThread_Spawn PROC

; 28   :     CTask *TaskPool = (CTask*)MMAP_TASKPOOL;
; 29   :     int pid = Pool->Alloc((CPool*)TaskPool);

	mov	eax, DWORD PTR _Pool
	push	esi
	push	1664					; 00000680H
	call	DWORD PTR [eax+8]
	mov	esi, eax
	pop	ecx

; 30   :     if (pid < 0 || pid >= TaskPool->Count) return 0;

	test	esi, esi
	js	$LN3@IThread_Sp
	cmp	esi, DWORD PTR ds:1684
	jge	$LN3@IThread_Sp

; 31   : 
; 32   :     CContext *ctx = (CContext*)(esp0 - sizeof(CContext));
; 33   :     TaskPool->CtxList[pid] = (u32)&ctx->cr3;

	mov	eax, DWORD PTR ds:1692
	push	ebx
	mov	ebx, DWORD PTR _esp0$[esp+4]
	push	ebp
	push	edi

; 34   : 
; 35   :     int shell = (int)Loader->GetEntry("System/Shell.dll");

	push	OFFSET ??_C@_0BB@CFKEFJPP@System?1Shell?4dll?$AA@
	lea	edi, DWORD PTR [ebx-64]
	mov	DWORD PTR [eax+esi*4], edi
	mov	eax, DWORD PTR _Loader
	call	DWORD PTR [eax+8]

; 36   :     stosd(ctx, 0, sizeof(CContext)/4);

	push	16					; 00000010H
	push	0
	push	edi
	mov	ebp, eax
	call	_stosd

; 37   :     ctx->cr3    = cr3;

	mov	ecx, DWORD PTR _cr3$[esp+28]
	add	esp, 16					; 00000010H
	mov	DWORD PTR [edi], ecx

; 38   :     ctx->cs     = RING3_CODE;

	mov	DWORD PTR [edi+44], 27			; 0000001bH

; 39   :     ctx->ds     = RING3_DATA;
; 40   :     ctx->ss     = RING3_DATA;
; 41   :     ctx->eflags = EFLAGS_IOPL + EFLAGS_IF;

	mov	DWORD PTR [edi+48], 12800		; 00003200H
	push	35					; 00000023H
	pop	eax
	mov	DWORD PTR [edi+4], eax
	mov	DWORD PTR [edi+56], eax

; 42   :     ctx->esp0   = esp3;

	mov	eax, DWORD PTR _esp3$[esp+12]
	mov	DWORD PTR [edi+52], eax

; 43   :     ctx->esp    = (int)&ctx->eip;

	lea	eax, DWORD PTR [edi+40]
	mov	DWORD PTR [edi+20], eax

; 44   :     ctx->eip    = (shell) ? shell : eip;

	test	ebp, ebp
	jne	SHORT $LN8@IThread_Sp
	mov	ebp, DWORD PTR _eip$[esp+12]
$LN8@IThread_Sp:

; 45   :     ctx->pid    = pid;
; 46   : 
; 47   :     if (fpu) {

	cmp	DWORD PTR _fpu, 0
	mov	DWORD PTR [eax], ebp
	mov	DWORD PTR [edi+60], esi
	je	SHORT $LN2@IThread_Sp

; 48   :         extern char x87[128];
; 49   :         u32 *ctx_fpu = (u32*)(esp0+0x100);
; 50   :         movsd(ctx_fpu, x87, sizeof(x87)/4);

	push	32					; 00000020H
	lea	eax, DWORD PTR [ebx+256]
	push	OFFSET _x87
	push	eax
	call	_movsd
	add	esp, 12					; 0000000cH
$LN2@IThread_Sp:

; 51   :     }
; 52   : 
; 53   :     if (sse) {

	cmp	DWORD PTR _sse, 0
	je	SHORT $LN1@IThread_Sp

; 54   :         extern char simd[320];
; 55   :         u32 *ctx_fpu = (u32*)(esp0+0x200);
; 56   :         movsd(ctx_fpu, simd, sizeof(simd)/4);

	push	80					; 00000050H
	lea	ecx, DWORD PTR [ebx+512]
	push	OFFSET _simd
	push	ecx
	call	_movsd
	add	esp, 12					; 0000000cH
$LN1@IThread_Sp:

; 57   :     }
; 58   : 
; 59   :     TaskPool->BlockQ[TaskPool->BlockTail++] = pid;

	mov	edx, DWORD PTR ds:1728

; 60   :     return ctx;

	mov	eax, edi
	mov	ecx, DWORD PTR ds:1708
	pop	edi
	pop	ebp
	pop	ebx
	mov	DWORD PTR [ecx+edx*4], esi
	inc	DWORD PTR ds:1728
	pop	esi

; 61   : }

	ret	0
$LN3@IThread_Sp:

; 30   :     if (pid < 0 || pid >= TaskPool->Count) return 0;

	xor	eax, eax
	pop	esi

; 61   : }

	ret	0
_IThread_Spawn ENDP
_TEXT	ENDS
; Function compile flags: /Ogspy
; File c:\users\cex123\desktop\fyp\develop\spartan\source\kernel\thread\process\thread.c
_TEXT	SEGMENT
_a$ = 8							; size = 4
_b$ = 12						; size = 4
_c$ = 16						; size = 4
_IThread_Killer PROC

; 128  : export bool IThread_Killer(void *a, void *b, void *c) {

	push	ebp
	mov	ebp, esp

; 129  :     if (Canvas == nullptr) Canvas = a;

	mov	eax, DWORD PTR _Canvas
	test	eax, eax
	jne	SHORT $LN7@IThread_Ki
	mov	eax, DWORD PTR _a$[ebp]
	mov	DWORD PTR _Canvas, eax
$LN7@IThread_Ki:

; 130  :     if (screen == nullptr) screen = b;

	mov	ecx, DWORD PTR _screen
	test	ecx, ecx
	jne	SHORT $LN6@IThread_Ki
	mov	ecx, DWORD PTR _b$[ebp]
	mov	DWORD PTR _screen, ecx
$LN6@IThread_Ki:

; 131  :     if (cursor == nullptr) cursor = c;

	mov	edx, DWORD PTR _cursor
	test	edx, edx
	jne	SHORT $LN5@IThread_Ki
	mov	edx, DWORD PTR _c$[ebp]
	mov	DWORD PTR _cursor, edx
$LN5@IThread_Ki:

; 132  :     if (Canvas != a) return false;

	cmp	eax, DWORD PTR _a$[ebp]
	je	SHORT $LN4@IThread_Ki
$LN10@IThread_Ki:
	xor	eax, eax

; 141  : }

	pop	ebp
	ret	0
$LN4@IThread_Ki:

; 133  :     if (screen != b) return false;

	cmp	ecx, DWORD PTR _b$[ebp]
	jne	SHORT $LN10@IThread_Ki

; 134  :     if (cursor != c) return false;

	cmp	edx, DWORD PTR _c$[ebp]
	jne	SHORT $LN10@IThread_Ki

; 135  : 
; 136  :     CTask *TaskPool = (CTask*)MMAP_TASKPOOL;
; 137  :     if (TaskPool->KillHead != TaskPool->KillTail) {

	mov	eax, DWORD PTR ds:1732
	cmp	eax, DWORD PTR ds:1736
	je	SHORT $LN1@IThread_Ki

; 138  :         SysCall(SYS_KILL,0,0,0,0,0,0);

	sub	esp, 20					; 00000014H
	xor	edx, edx
	mov	ecx, 32769				; 00008001H
	call	_SysCall
	add	esp, 20					; 00000014H
$LN1@IThread_Ki:

; 139  :     }
; 140  :     return true;

	xor	eax, eax
	inc	eax

; 141  : }

	pop	ebp
	ret	0
_IThread_Killer ENDP
_TEXT	ENDS
; Function compile flags: /Ogspy
; File c:\users\cex123\desktop\fyp\develop\spartan\source\kernel\thread\process\thread.c
_TEXT	SEGMENT
_code$ = 8						; size = 4
_IThread_Exit PROC

; 143  : u32 IThread_Exit(int code) {

	push	esi

; 144  :     CTask *TaskPool = (CTask*)MMAP_TASKPOOL;
; 145  :     IThread_Yield();

	call	_IThread_Yield

; 146  :     Logger("IThread_Exit\n");
; 147  :     IThread_Debug();
; 148  :     TaskPool->KillQ[TaskPool->KillTail] = IThread_GetPid();

	mov	esi, DWORD PTR ds:1716
	mov	eax, DWORD PTR ds:1704
	mov	edx, DWORD PTR ds:1736
	mov	ecx, DWORD PTR ds:1712
	mov	eax, DWORD PTR [eax+esi*4]
	mov	DWORD PTR [ecx+edx*4], eax

; 149  :     TaskPool->KillTail = ++TaskPool->KillTail % (4*KB);

	mov	eax, DWORD PTR ds:1736
	inc	eax
	mov	DWORD PTR ds:1736, eax
	and	eax, -2147479553			; 80000fffH
	jns	SHORT $LN162@IThread_Ex
	dec	eax
	or	eax, -4096				; fffff000H
	inc	eax
$LN162@IThread_Ex:
	mov	DWORD PTR ds:1736, eax
$LL2@IThread_Ex:

; 150  :     for (;;);

	jmp	SHORT $LL2@IThread_Ex
_IThread_Exit ENDP
_TEXT	ENDS
; Function compile flags: /Ogspy
; File c:\users\cex123\desktop\fyp\develop\spartan\source\kernel\thread\process\thread.c
_TEXT	SEGMENT
_pid$ = 8						; size = 4
_IThread_Kill PROC

; 155  :     CTask *TaskPool = (CTask*)MMAP_TASKPOOL;
; 156  :     if (pid == IThread_GetPid()) IThread_Exit(0);

	mov	ecx, DWORD PTR ds:1716
	mov	eax, DWORD PTR ds:1704
	push	esi
	mov	esi, DWORD PTR _pid$[esp]
	cmp	esi, DWORD PTR [eax+ecx*4]
	je	SHORT $LN160@IThread_Ki

; 157  :     IThread_Debug();
; 158  :     IThread_Yield();

	call	_IThread_Yield

; 159  :     Logger("IThread_Kill\n");
; 160  :     TaskPool->KillQ[TaskPool->KillTail] = pid;

	mov	ecx, DWORD PTR ds:1736
	mov	eax, DWORD PTR ds:1712
	mov	DWORD PTR [eax+ecx*4], esi

; 161  :     TaskPool->KillTail = ++TaskPool->KillTail % (4*KB);

	mov	eax, DWORD PTR ds:1736
	inc	eax
	mov	DWORD PTR ds:1736, eax
	and	eax, -2147479553			; 80000fffH
	jns	SHORT $LN158@IThread_Ki
	dec	eax
	or	eax, -4096				; fffff000H
	inc	eax
$LN158@IThread_Ki:
	mov	DWORD PTR ds:1736, eax

; 162  :     return true;

	xor	eax, eax
	inc	eax
	pop	esi

; 163  : }

	ret	0
$LN160@IThread_Ki:

; 155  :     CTask *TaskPool = (CTask*)MMAP_TASKPOOL;
; 156  :     if (pid == IThread_GetPid()) IThread_Exit(0);

	push	0
	call	_IThread_Exit
$LN162@IThread_Ki:
$LN159@IThread_Ki:
	int	3
_IThread_Kill ENDP
_TEXT	ENDS
; Function compile flags: /Ogspy
; File c:\users\cex123\desktop\fyp\develop\spartan\source\kernel\thread\process\thread.c
_TEXT	SEGMENT
_pid$ = 8						; size = 4
_IThread_Block PROC

; 166  :     CTask *TaskPool = (CTask*)MMAP_TASKPOOL;
; 167  :     if (pid < 0 || pid >= TaskPool->Count) return false;

	mov	edx, DWORD PTR _pid$[esp-4]
	push	esi
	push	edi
	test	edx, edx
	js	SHORT $LN5@IThread_Bl
	cmp	edx, DWORD PTR ds:1684
	jge	SHORT $LN5@IThread_Bl

; 168  :     for (int i = 0; i < TaskPool->ReadyTail; i++) {

	mov	eax, DWORD PTR ds:1720
	xor	ecx, ecx
	test	eax, eax
	jle	SHORT $LN5@IThread_Bl

; 169  :         if (TaskPool->ReadyQ[i] == pid) {

	mov	edi, DWORD PTR ds:1704
	mov	esi, edi
$LL4@IThread_Bl:
	cmp	DWORD PTR [esi], edx
	je	SHORT $LN12@IThread_Bl

; 168  :     for (int i = 0; i < TaskPool->ReadyTail; i++) {

	inc	ecx
	add	esi, 4
	cmp	ecx, eax
	jl	SHORT $LL4@IThread_Bl
$LN5@IThread_Bl:

; 166  :     CTask *TaskPool = (CTask*)MMAP_TASKPOOL;
; 167  :     if (pid < 0 || pid >= TaskPool->Count) return false;

	xor	eax, eax
$LN7@IThread_Bl:
	pop	edi
	pop	esi

; 176  :         }
; 177  :     }
; 178  :     return false;
; 179  : }

	ret	0
$LN12@IThread_Bl:

; 170  :             TaskPool->ReadyTail--;

	dec	eax
	mov	DWORD PTR ds:1720, eax

; 171  :             TaskPool->ReadyTail = MAX(0, TaskPool->ReadyTail);

	jns	SHORT $LN9@IThread_Bl
	xor	eax, eax
$LN9@IThread_Bl:
	mov	DWORD PTR ds:1720, eax

; 172  :             TaskPool->ReadyQ[i] = TaskPool->ReadyQ[TaskPool->ReadyTail];

	mov	eax, DWORD PTR [edi+eax*4]
	mov	DWORD PTR [edi+ecx*4], eax

; 173  :             TaskPool->ReadyQ[TaskPool->ReadyTail] = 0;

	mov	ecx, DWORD PTR ds:1720
	mov	eax, DWORD PTR ds:1704
	and	DWORD PTR [eax+ecx*4], 0

; 174  :             TaskPool->BlockQ[TaskPool->BlockTail++] = pid;

	mov	ecx, DWORD PTR ds:1728
	mov	eax, DWORD PTR ds:1708
	mov	DWORD PTR [eax+ecx*4], edx
	xor	eax, eax
	inc	eax
	add	DWORD PTR ds:1728, eax

; 175  :             return true;

	jmp	SHORT $LN7@IThread_Bl
_IThread_Block ENDP
_TEXT	ENDS
; Function compile flags: /Ogspy
; File c:\users\cex123\desktop\fyp\develop\spartan\source\kernel\thread\process\thread.c
_TEXT	SEGMENT
_pid$ = 8						; size = 4
_IThread_Unblock PROC

; 182  :     CTask *TaskPool = (CTask*)MMAP_TASKPOOL;
; 183  :     if (pid < 0 || pid >= TaskPool->Count) return false;

	mov	edx, DWORD PTR _pid$[esp-4]
	push	esi
	push	edi
	test	edx, edx
	js	SHORT $LN5@IThread_Un
	mov	eax, DWORD PTR ds:1684
	cmp	edx, eax
	jge	SHORT $LN5@IThread_Un

; 184  :     for (int i = 0; i < TaskPool->Count; i++) {

	xor	ecx, ecx
	test	eax, eax
	jle	SHORT $LN5@IThread_Un

; 185  :         if (TaskPool->BlockQ[i] == pid) {

	mov	edi, DWORD PTR ds:1708
	mov	esi, edi
$LL4@IThread_Un:
	cmp	DWORD PTR [esi], edx
	je	SHORT $LN12@IThread_Un

; 184  :     for (int i = 0; i < TaskPool->Count; i++) {

	inc	ecx
	add	esi, 4
	cmp	ecx, eax
	jl	SHORT $LL4@IThread_Un
$LN5@IThread_Un:

; 182  :     CTask *TaskPool = (CTask*)MMAP_TASKPOOL;
; 183  :     if (pid < 0 || pid >= TaskPool->Count) return false;

	xor	eax, eax
$LN7@IThread_Un:
	pop	edi
	pop	esi

; 192  :         }
; 193  :     }
; 194  :     return false;
; 195  : }

	ret	0
$LN12@IThread_Un:

; 186  :             TaskPool->BlockTail--;

	mov	eax, DWORD PTR ds:1728
	dec	eax
	mov	DWORD PTR ds:1728, eax

; 187  :             TaskPool->BlockTail = MAX(0, TaskPool->BlockTail);

	jns	SHORT $LN9@IThread_Un
	xor	eax, eax
$LN9@IThread_Un:
	mov	DWORD PTR ds:1728, eax

; 188  :             TaskPool->BlockQ[i] = TaskPool->BlockQ[TaskPool->BlockTail];

	mov	eax, DWORD PTR [edi+eax*4]
	mov	DWORD PTR [edi+ecx*4], eax

; 189  :             TaskPool->BlockQ[TaskPool->BlockTail] = 0;

	mov	ecx, DWORD PTR ds:1728
	mov	eax, DWORD PTR ds:1708
	and	DWORD PTR [eax+ecx*4], 0

; 190  :             TaskPool->ReadyQ[TaskPool->ReadyTail++] = pid;

	mov	ecx, DWORD PTR ds:1720
	mov	eax, DWORD PTR ds:1704
	mov	DWORD PTR [eax+ecx*4], edx
	xor	eax, eax
	inc	eax
	add	DWORD PTR ds:1720, eax

; 191  :             return true;

	jmp	SHORT $LN7@IThread_Un
_IThread_Unblock ENDP
_TEXT	ENDS
; Function compile flags: /Ogspy
; File c:\users\cex123\desktop\fyp\develop\spartan\source\kernel\thread\process\thread.c
_TEXT	SEGMENT
_pid$ = 8						; size = 4
_IThread_Run PROC

; 198  :     return IThread_Unblock(pid);

	jmp	_IThread_Unblock
_IThread_Run ENDP
_TEXT	ENDS
; Function compile flags: /Ogspy
; File c:\users\cex123\desktop\fyp\develop\spartan\source\kernel\thread\process\thread.c
_TEXT	SEGMENT
_cr3$ = 8						; size = 4
_eip$ = 12						; size = 4
_esp0$ = 16						; size = 4
_esp3$ = 20						; size = 4
_IThread_Fork PROC

; 201  : CContext *IThread_Fork(u32 cr3, u32 eip, u32 esp0, u32 esp3) {

	push	esi

; 202  :     CContext *context = IThread_Spawn(cr3, eip, esp0, esp3);

	push	DWORD PTR _esp3$[esp]
	push	DWORD PTR _esp0$[esp+4]
	push	DWORD PTR _eip$[esp+8]
	push	DWORD PTR _cr3$[esp+12]
	call	_IThread_Spawn
	mov	esi, eax
	add	esp, 16					; 00000010H

; 203  :     if (!context) return false;

	test	esi, esi
	jne	SHORT $LN2@IThread_Fo
	pop	esi

; 205  :     return context;
; 206  : }

	ret	0
$LN2@IThread_Fo:

; 204  :     if (!IThread_Run(context->pid)) return 0;

	push	DWORD PTR [esi+60]
	call	_IThread_Unblock
	neg	eax
	pop	ecx
	sbb	eax, eax
	and	eax, esi
	pop	esi

; 205  :     return context;
; 206  : }

	ret	0
_IThread_Fork ENDP
_TEXT	ENDS
; Function compile flags: /Ogspy
; File c:\users\cex123\desktop\fyp\develop\spartan\source\kernel\thread\process\thread.c
_TEXT	SEGMENT
_IThread_Yield PROC

; 209  :     SysCall(SYS_YIELD,0,0,0,0,0,0);

	sub	esp, 20					; 00000014H
	xor	edx, edx
	mov	ecx, 32768				; 00008000H
	call	_SysCall

; 210  :     return true;

	xor	eax, eax
	add	esp, 20					; 00000014H
	inc	eax

; 211  : }

	ret	0
_IThread_Yield ENDP
_TEXT	ENDS
; Function compile flags: /Ogspy
; File c:\users\cex123\desktop\fyp\develop\spartan\source\kernel\thread\process\thread.c
_TEXT	SEGMENT
_IThread_Debug PROC

; 218  :     CTask *TaskPool = (CTask*)MMAP_TASKPOOL;
; 219  :     Logger("\n");
; 220  :     int  RH = TaskPool->ReadyHead;
; 221  :     int  RT = TaskPool->ReadyTail;
; 222  :     int *RQ = TaskPool->ReadyQ;
; 223  :     Logger(" RDYQ"); for (int i = 0; i <= RT+2; i++) Logger((i == RH) ? " H" : " -"); Logger("\n");
; 224  :     Logger(" RDYQ"); for (int i = 0; i <= RT+2; i++) Logger(" %d", RQ[i]);            Logger("\n");
; 225  :     Logger(" RDYQ"); for (int i = 0; i <= RT+2; i++) Logger((i == RT) ? " T" : " -"); Logger("\n");
; 226  : 
; 227  :     int  BH = TaskPool->BlockHead;
; 228  :     int  BT = TaskPool->BlockTail;
; 229  :     int *BQ = TaskPool->BlockQ;
; 230  :     Logger(" BLKQ"); for (int i = 0; i <= BT+2; i++) Logger((i == BH) ? " H" : " -"); Logger("\n");
; 231  :     Logger(" BLKQ"); for (int i = 0; i <= BT+2; i++) Logger(" %d", BQ[i]);            Logger("\n");
; 232  :     Logger(" BLKQ"); for (int i = 0; i <= BT+2; i++) Logger((i == BT) ? " T" : " -"); Logger("\n");
; 233  : 
; 234  :     int  KH = TaskPool->KillHead;
; 235  :     int  KT = TaskPool->KillTail;
; 236  :     int *KQ = TaskPool->KillQ;
; 237  :     Logger(" KILL"); for (int i = 0; i <= KT+2; i++) Logger((i == KH) ? " H" : " -"); Logger("\n");
; 238  :     Logger(" KILL"); for (int i = 0; i <= KT+2; i++) Logger(" %d", KQ[i]);            Logger("\n");
; 239  :     Logger(" KILL"); for (int i = 0; i <= KT+2; i++) Logger((i == KT) ? " T" : " -"); Logger("\n");
; 240  :     return true;

	xor	eax, eax
	inc	eax

; 241  : }

	ret	0
_IThread_Debug ENDP
_TEXT	ENDS
; Function compile flags: /Ogspy
; File c:\users\cex123\desktop\fyp\develop\spartan\source\kernel\thread\process\thread.c
_TEXT	SEGMENT
_text$ = 8						; size = 4
_IThread_Status PROC

; 244  :     CTask *TaskPool = (CTask*)MMAP_TASKPOOL;
; 245  :     Format(text,
; 246  :     " Thread->Name       = %s\n" " Thread->Tss        = %X\n"
; 247  :     " Thread->Node       = %X\n" " Thread->Data       = %X\n"
; 248  :     " Thread->Gran       = %X\n"
; 249  : 
; 250  :     " Thread->Count      = %X\n" " Thread->Next       = %X\n"
; 251  :     " Thread->ReadyQ     = %X\n" " Thread->BlockQ     = %X\n"
; 252  : 
; 253  :     " Thread->CtxList    = %X\n"
; 254  :     " Thread->PcbList    = %X\n" " Thread->PebList    = %X\n"
; 255  :     " Thread->ReadyHead  = %X\n" " Thread->ReadyTail  = %X\n"
; 256  :     " Thread->BlockHead  = %X\n" " Thread->BlockTail  = %X\n"
; 257  :     " Thread->ReadyQ[%d] = %X\n" " Thread->BlockQ[%d] = %X\n",
; 258  :     TaskPool->Name,      TaskPool->Tss,
; 259  :     TaskPool->Node,      TaskPool->Data,
; 260  :     TaskPool->Gran,
; 261  :     TaskPool->Count,     TaskPool->Next,
; 262  :     TaskPool->CtxList,
; 263  :     TaskPool->PcbList,   TaskPool->PebList,
; 264  :     TaskPool->ReadyQ,    TaskPool->BlockQ,
; 265  :     TaskPool->ReadyHead, TaskPool->ReadyTail,
; 266  :     TaskPool->BlockHead, TaskPool->BlockTail,
; 267  :     TaskPool->ReadyHead, TaskPool->ReadyQ[TaskPool->ReadyHead],
; 268  :     TaskPool->BlockHead, TaskPool->BlockQ[TaskPool->BlockHead]);

	mov	eax, DWORD PTR ds:1724
	mov	ecx, DWORD PTR ds:1716
	mov	edx, DWORD PTR ds:1704
	push	esi
	mov	esi, DWORD PTR ds:1708
	push	DWORD PTR [esi+eax*4]
	push	eax
	push	DWORD PTR [edx+ecx*4]
	push	ecx
	push	DWORD PTR ds:1728
	push	eax
	push	DWORD PTR ds:1720
	push	ecx
	push	esi
	push	edx
	push	DWORD PTR ds:1700
	push	DWORD PTR ds:1696
	push	DWORD PTR ds:1692
	push	DWORD PTR ds:1688
	push	DWORD PTR ds:1684
	push	DWORD PTR ds:1680
	push	DWORD PTR ds:1676
	push	DWORD PTR ds:1672
	push	DWORD PTR ds:1668
	push	1664					; 00000680H
	push	OFFSET ??_C@_0BMD@DEJPFPA@?5Thread?9?$DOName?5?5?5?5?5?5?5?$DN?5?$CFs?6?5Thread@
	push	DWORD PTR _text$[esp+84]
	call	DWORD PTR __imp__Format

; 269  :     return text;

	mov	eax, DWORD PTR _text$[esp+88]
	add	esp, 88					; 00000058H
	pop	esi

; 270  : }

	ret	0
_IThread_Status ENDP
_TEXT	ENDS
END