; Listing generated by Microsoft (R) Optimizing Compiler Version 18.00.40629.0 

	TITLE	C:\Users\cex123\Desktop\FYP\develop\spartan\source\Kernel\Monitor\Monitor.c
	.686P
	.XMM
	include listing.inc
	.model	flat

INCLUDELIB OLDNAMES

PUBLIC	_Monitor
PUBLIC	??_C@_03NOLLCAOD@?$AA?$AA?$AA?$AA@		; `string'
PUBLIC	??_C@_03GIFLFGDG@?0?$DLZi@			; `string'
PUBLIC	_monitor
EXTRN	_rol:PROC
;	COMDAT ??_C@_03GIFLFGDG@?0?$DLZi@
CONST	SEGMENT
??_C@_03GIFLFGDG@?0?$DLZi@ DB ',;Zi'			; `string'
CONST	ENDS
;	COMDAT ??_C@_03NOLLCAOD@?$AA?$AA?$AA?$AA@
CONST	SEGMENT
??_C@_03NOLLCAOD@?$AA?$AA?$AA?$AA@ DB 00H, 00H, 00H, 00H ; `string'
_Monitor DD	FLAT:_monitor
_map	DD	049565420H
	DD	00H
	DD	01000H
	ORG $+8
	DD	042494f53H
	DD	01000H
	DD	01000H
	ORG $+8
	DD	050414745H
	DD	02000H
	DD	02000H
	ORG $+8
	DD	047445420H
	DD	04000H
	DD	01000H
	ORG $+8
	DD	054535320H
	DD	05000H
	DD	01000H
	ORG $+8
	DD	049445420H
	DD	06000H
	DD	01000H
	ORG $+8
	DD	049525120H
	DD	07000H
	DD	01000H
	ORG $+8
	DD	049535220H
	DD	08000H
	DD	04000H
	ORG $+8
	DD	05441534bH
	DD	0f000H
	DD	01000H
	ORG $+8
	DD	052445951H
	DD	010000H
	DD	04000H
	ORG $+8
	DD	0424c4b51H
	DD	014000H
	DD	04000H
	ORG $+8
	DD	04b494c4cH
	DD	018000H
	DD	04000H
	ORG $+8
	DD	043545820H
	DD	01c000H
	DD	04000H
	ORG $+8
	DD	050434220H
	DD	020000H
	DD	04000H
	ORG $+8
	DD	050454220H
	DD	024000H
	DD	04000H
	ORG $+8
	DD	054424c33H
	DD	028000H
	DD	04000H
	ORG $+8
	DD	0434f4d31H
	DD	030000H
	DD	02000H
	ORG $+8
	DD	0434f4d32H
	DD	032000H
	DD	02000H
	ORG $+8
	DD	04b444220H
	DD	034000H
	DD	01000H
	ORG $+8
	DD	04d4f5553H
	DD	035000H
	DD	01000H
	ORG $+8
	DD	052544320H
	DD	036000H
	DD	01000H
	ORG $+8
	DD	056455341H
	DD	037000H
	DD	01000H
	ORG $+8
	DD	0534e4420H
	DD	038000H
	DD	08000H
	ORG $+8
	DD	054424c30H
	DD	0800000H
	DD	0400000H
	ORG $+8
	DD	048454150H
	DD	0c00000H
	DD	0100000H
	ORG $+8
	DD	04449534bH
	DD	0100000H
	DD	0d00000H
	ORG $+8
	DD	05354414bH
	DD	08e000H
	DD	01000H
	ORG $+8
	DD	0544f5053H
	DD	08f000H
	DD	01000H
	ORG $+8
_monitor DD	FLAT:_IMonitor_EnableErrorRecovery
	DD	FLAT:_IMonitor_ClearMemory
	DD	FLAT:_IMonitor_CalculateCrc
	DD	FLAT:_IMonitor_Recover
	DD	FLAT:_IMonitor_Verify
	DD	FLAT:_IMonitor_Protect
	DD	FLAT:_IMonitor_GetBase
	DD	FLAT:_IMonitor_GetSize
	DD	FLAT:_IMonitor_GetCrc
	DD	FLAT:_IMonitor_ProtectAll
	DD	FLAT:_IMonitor_RecoverAll
PUBLIC	_IMonitor_RecoverAll
PUBLIC	_IMonitor_ProtectAll
PUBLIC	_IMonitor_ClearMemory
PUBLIC	_IMonitor_EnableErrorRecovery
PUBLIC	_IMonitor_GetCrc
PUBLIC	_IMonitor_GetSize
PUBLIC	_IMonitor_GetCopy
PUBLIC	_IMonitor_GetBase
PUBLIC	_IMonitor_Protect
PUBLIC	_IMonitor_Verify
PUBLIC	_IMonitor_Recover
PUBLIC	_IMonitor_CalculateCrc
PUBLIC	_IMonitor_GetId
; Function compile flags: /Ogspy
; File c:\users\cex123\desktop\fyp\develop\spartan\source\kernel\monitor\monitor.c
_TEXT	SEGMENT
_IMonitor_GetId PROC
; _owner$ = ecx

; 42   :     for (int id = 0; id < elementsof(map); id++) {

	xor	edx, edx
	mov	eax, edx
$LL4@IMonitor_G:

; 43   :         if (owner == map[id][NAME]) return id;

	cmp	ecx, DWORD PTR _map[eax]
	je	SHORT $LN8@IMonitor_G

; 42   :     for (int id = 0; id < elementsof(map); id++) {

	add	eax, 20					; 00000014H
	inc	edx
	cmp	eax, 560				; 00000230H
	jb	SHORT $LL4@IMonitor_G

; 44   :     }
; 45   :     return 0;

	xor	eax, eax

; 46   : }

	ret	0
$LN8@IMonitor_G:

; 43   :         if (owner == map[id][NAME]) return id;

	mov	eax, edx

; 46   : }

	ret	0
_IMonitor_GetId ENDP
_TEXT	ENDS
; Function compile flags: /Ogspy
; File c:\users\cex123\desktop\fyp\develop\spartan\source\kernel\monitor\monitor.c
_TEXT	SEGMENT
_h$ = -4						; size = 4
_base$ = 8						; size = 4
tv437 = 12						; size = 4
_size$ = 12						; size = 4
_IMonitor_CalculateCrc PROC

; 48   : u32 IMonitor_CalculateCrc(u8 *base, u32 size) {

	push	ecx
	push	esi
	push	edi

; 49   :     char h[4] = "\x00\x00\x00\x00";
; 50   :     char m[4] = "\x2C\x3B\x5A\x69";
; 51   :     u32 *crc = (int*)h;
; 52   :     for (char *t = base; size > 0; size -= 4, t += 4) {

	mov	edi, DWORD PTR _size$[esp+8]
	xor	esi, esi
	mov	DWORD PTR _h$[esp+12], esi
	test	edi, edi
	je	$LN1@IMonitor_C

; 53   :         // 2x2 matrix multiplication spreads the bits
; 54   :         h[0] ^= t[0]*m[0] + t[1]*m[2];

	mov	eax, DWORD PTR _base$[esp+8]
	dec	edi
	push	ebx
	push	ebp
	shr	edi, 2
	push	44					; 0000002cH
	lea	ebp, DWORD PTR [eax+3]
	mov	DWORD PTR tv437[esp+20], 90		; 0000005aH
	inc	edi
	pop	ecx
$LL3@IMonitor_C:
	mov	al, BYTE PTR [ebp-3]
	imul	cl

; 55   :         h[1] ^= t[0]*m[1] + t[1]*m[3];

	push	59					; 0000003bH
	mov	cl, al
	mov	al, BYTE PTR [ebp-2]
	imul	BYTE PTR tv437[esp+20]
	add	cl, al
	mov	al, BYTE PTR [ebp-3]
	xor	BYTE PTR _h$[esp+24], cl
	pop	ecx
	imul	cl
	push	105					; 00000069H
	mov	cl, al
	mov	al, BYTE PTR [ebp-2]
	pop	edx
	imul	dl

; 56   :         h[2] ^= t[2]*m[0] + t[3]*m[2];

	push	44					; 0000002cH
	add	cl, al
	mov	al, BYTE PTR [ebp-1]
	xor	BYTE PTR _h$[esp+25], cl
	pop	ecx
	imul	cl

; 57   :         h[3] ^= t[2]*m[1] + t[3]*m[3];

	push	59					; 0000003bH
	mov	cl, al
	mov	al, BYTE PTR [ebp]
	imul	BYTE PTR tv437[esp+20]
	add	cl, al
	mov	al, BYTE PTR [ebp-1]
	xor	BYTE PTR _h$[esp+26], cl
	pop	ecx
	imul	cl

; 58   :         // rotate left further spreads the bits
; 59   :         crc[0] += rol(crc[0], 13);

	push	13					; 0000000dH
	mov	cl, al
	mov	al, BYTE PTR [ebp]
	imul	dl
	add	cl, al
	xor	BYTE PTR _h$[esp+27], cl
	mov	esi, DWORD PTR _h$[esp+24]
	push	esi
	call	_rol
	pop	ecx
	pop	ecx
	add	esi, eax
	lea	ebp, DWORD PTR [ebp+4]
	mov	DWORD PTR _h$[esp+20], esi
	push	44					; 0000002cH
	pop	ecx
	dec	edi
	jne	SHORT $LL3@IMonitor_C
	pop	ebp
	pop	ebx
$LN1@IMonitor_C:
	pop	edi

; 60   :     }
; 61   :     return crc[0];

	mov	eax, esi
	pop	esi

; 62   : }

	pop	ecx
	ret	0
_IMonitor_CalculateCrc ENDP
_TEXT	ENDS
; Function compile flags: /Ogspy
; File c:\users\cex123\desktop\fyp\develop\spartan\source\kernel\monitor\monitor.c
_TEXT	SEGMENT
_owner$ = 8						; size = 4
_IMonitor_Recover PROC

; 65   :     int id = IMonitor_GetId(owner);

	mov	ecx, DWORD PTR _owner$[esp-4]
	push	ebx
	push	ebp
	push	esi
	push	edi
	call	_IMonitor_GetId

; 66   :     if (id < 0 || id >= elementsof(map)) return false;

	test	eax, eax
	js	SHORT $LN3@IMonitor_R
	cmp	eax, 28					; 0000001cH
	jae	SHORT $LN3@IMonitor_R

; 67   : 
; 68   :     u8 *base = (u8*)map[id][BASE];

	imul	edi, eax, 20

; 69   :     u8 *copy = (u8*)map[id][COPY];
; 70   :     u32 size = (u32)map[id][SIZE];

	mov	esi, DWORD PTR _map[edi+8]
	mov	ebp, DWORD PTR _map[edi+4]
	mov	ebx, DWORD PTR _map[edi+12]

; 71   : 
; 72   :     u32 crc1 = IMonitor_CalculateCrc(base, size);

	push	esi
	push	ebp
	call	_IMonitor_CalculateCrc
	pop	ecx
	pop	ecx

; 73   :     if (crc1 == map[id][CRC]) {

	cmp	eax, DWORD PTR _map[edi+16]
	jne	SHORT $LN2@IMonitor_R

; 74   :         movsd(copy, base, size/4);

	shr	esi, 2
	push	esi
	push	ebp
	push	ebx
$LN7@IMonitor_R:
	call	_movsd

; 75   :         return true;

	xor	eax, eax
	add	esp, 12					; 0000000cH
	inc	eax
	jmp	SHORT $LN5@IMonitor_R
$LN2@IMonitor_R:

; 76   :     }
; 77   : 
; 78   :     u32 crc2 = IMonitor_CalculateCrc(copy, size);

	push	esi
	push	ebx
	call	_IMonitor_CalculateCrc
	pop	ecx
	pop	ecx

; 79   :     if (crc2 == map[id][CRC]) {

	cmp	eax, DWORD PTR _map[edi+16]
	jne	SHORT $LN3@IMonitor_R

; 80   :         movsd(base, copy, size/4);

	shr	esi, 2
	push	esi
	push	ebx
	push	ebp

; 81   :         return true;

	jmp	SHORT $LN7@IMonitor_R
$LN3@IMonitor_R:

; 66   :     if (id < 0 || id >= elementsof(map)) return false;

	xor	eax, eax
$LN5@IMonitor_R:
	pop	edi
	pop	esi
	pop	ebp
	pop	ebx

; 82   :     }
; 83   :     return false;
; 84   : }

	ret	0
_IMonitor_Recover ENDP
_TEXT	ENDS
; Function compile flags: /Ogspy
; File c:\users\cex123\desktop\fyp\develop\spartan\source\kernel\monitor\monitor.c
_TEXT	SEGMENT
_owner$ = 8						; size = 4
_IMonitor_Verify PROC

; 87   :     int id = IMonitor_GetId(owner);

	mov	ecx, DWORD PTR _owner$[esp-4]
	call	_IMonitor_GetId

; 88   :     if (id < 0 || id >= elementsof(map)) return false;

	test	eax, eax
	js	SHORT $LN1@IMonitor_V
	cmp	eax, 28					; 0000001cH
	jae	SHORT $LN1@IMonitor_V

; 89   : 
; 90   :     u8 *base = (u8*)map[id][BASE];

	push	esi
	imul	esi, eax, 20

; 91   :     u32 size = map[id][SIZE];
; 92   : 
; 93   :     u32 crc = IMonitor_CalculateCrc(base, size);

	push	DWORD PTR _map[esi+8]
	push	DWORD PTR _map[esi+4]
	call	_IMonitor_CalculateCrc
	pop	ecx
	pop	ecx

; 94   :     bool status = (crc == map[id][CRC]);

	xor	ecx, ecx
	cmp	eax, DWORD PTR _map[esi+16]
	pop	esi
	sete	cl

; 95   : 
; 96   :     #ifndef OPTIMIZED
; 97   :     u64 totext = (u64)owner;
; 98   :     char *text = (status) ? "OK" : "Failed Cycle Redundency Check";
; 99   :     Logger(" Verifying  %s ... %s (CRC = %X)\n", &totext, text, crc);
; 100  :     #endif
; 101  :     return status;

	mov	eax, ecx

; 102  : }

	ret	0
$LN1@IMonitor_V:

; 88   :     if (id < 0 || id >= elementsof(map)) return false;

	xor	eax, eax

; 102  : }

	ret	0
_IMonitor_Verify ENDP
_TEXT	ENDS
; Function compile flags: /Ogspy
; File c:\users\cex123\desktop\fyp\develop\spartan\source\kernel\monitor\monitor.c
_TEXT	SEGMENT
_owner$ = 8						; size = 4
_IMonitor_Protect PROC

; 105  :     int id = IMonitor_GetId(owner);

	mov	ecx, DWORD PTR _owner$[esp-4]
	call	_IMonitor_GetId

; 106  :     if (id < 0 || id >= elementsof(map)) return false;

	test	eax, eax
	js	SHORT $LN1@IMonitor_P
	cmp	eax, 28					; 0000001cH
	jae	SHORT $LN1@IMonitor_P

; 107  :     u8 *base = (u8*)map[id][BASE];

	push	ebx
	push	ebp
	push	esi
	imul	esi, eax, 20
	push	edi

; 108  :     u8 *copy = (u8*)map[id][COPY];
; 109  :     u32 size = (u32)map[id][SIZE];

	mov	edi, DWORD PTR _map[esi+8]
	mov	ebx, DWORD PTR _map[esi+4]
	mov	ebp, DWORD PTR _map[esi+12]

; 110  : 
; 111  :     u32 crc = IMonitor_CalculateCrc(base, size);

	push	edi
	push	ebx
	call	_IMonitor_CalculateCrc

; 112  :     map[id][CRC] = crc;
; 113  :     movsd(copy, base, size/4);

	shr	edi, 2
	push	edi
	push	ebx
	push	ebp
	mov	DWORD PTR _map[esi+16], eax
	call	_movsd
	add	esp, 20					; 00000014H

; 114  : 
; 115  :     #ifndef OPTIMIZED
; 116  :     u64 totext = (u64)owner;
; 117  :     Logger(" Updating   %s ... OK (CRC = %X)\n", &totext, crc);
; 118  :     #endif
; 119  :     return true;

	xor	eax, eax
	inc	eax
	pop	edi
	pop	esi
	pop	ebp
	pop	ebx

; 120  : }

	ret	0
$LN1@IMonitor_P:

; 106  :     if (id < 0 || id >= elementsof(map)) return false;

	xor	eax, eax

; 120  : }

	ret	0
_IMonitor_Protect ENDP
_TEXT	ENDS
; Function compile flags: /Ogspy
; File c:\users\cex123\desktop\fyp\develop\spartan\source\kernel\monitor\monitor.c
_TEXT	SEGMENT
_owner$ = 8						; size = 4
_IMonitor_GetBase PROC

; 123  :     int id = IMonitor_GetId(owner);

	mov	ecx, DWORD PTR _owner$[esp-4]
	call	_IMonitor_GetId

; 124  :     return (id < 0 || id >= elementsof(map)) ? 0 : (char*)map[id][BASE];

	test	eax, eax
	js	SHORT $LN3@IMonitor_G
	cmp	eax, 28					; 0000001cH
	jae	SHORT $LN3@IMonitor_G
	imul	eax, eax, 20
	mov	eax, DWORD PTR _map[eax+4]

; 125  : }

	ret	0
$LN3@IMonitor_G:

; 124  :     return (id < 0 || id >= elementsof(map)) ? 0 : (char*)map[id][BASE];

	xor	eax, eax

; 125  : }

	ret	0
_IMonitor_GetBase ENDP
_TEXT	ENDS
; Function compile flags: /Ogspy
; File c:\users\cex123\desktop\fyp\develop\spartan\source\kernel\monitor\monitor.c
_TEXT	SEGMENT
_IMonitor_GetCopy PROC
; _owner$dead$ = ecx

; 128  :     int id = IMonitor_GetId(owner);

	mov	ecx, 1447383873				; 56455341H
	call	_IMonitor_GetId

; 129  :     return (id < 0 || id >= elementsof(map)) ? 0 : (char*)map[id][COPY];

	test	eax, eax
	js	SHORT $LN3@IMonitor_G
	cmp	eax, 28					; 0000001cH
	jae	SHORT $LN3@IMonitor_G
	imul	eax, eax, 20
	mov	eax, DWORD PTR _map[eax+12]

; 130  : }

	ret	0
$LN3@IMonitor_G:

; 129  :     return (id < 0 || id >= elementsof(map)) ? 0 : (char*)map[id][COPY];

	xor	eax, eax

; 130  : }

	ret	0
_IMonitor_GetCopy ENDP
_TEXT	ENDS
; Function compile flags: /Ogspy
; File c:\users\cex123\desktop\fyp\develop\spartan\source\kernel\monitor\monitor.c
_TEXT	SEGMENT
_owner$ = 8						; size = 4
_IMonitor_GetSize PROC

; 133  :     int id = IMonitor_GetId(owner);

	mov	ecx, DWORD PTR _owner$[esp-4]
	call	_IMonitor_GetId

; 134  :     return (id < 0 || id >= elementsof(map)) ? 0 : map[id][SIZE];

	test	eax, eax
	js	SHORT $LN3@IMonitor_G
	cmp	eax, 28					; 0000001cH
	jae	SHORT $LN3@IMonitor_G
	imul	eax, eax, 20
	mov	eax, DWORD PTR _map[eax+8]

; 135  : }

	ret	0
$LN3@IMonitor_G:

; 134  :     return (id < 0 || id >= elementsof(map)) ? 0 : map[id][SIZE];

	xor	eax, eax

; 135  : }

	ret	0
_IMonitor_GetSize ENDP
_TEXT	ENDS
; Function compile flags: /Ogspy
; File c:\users\cex123\desktop\fyp\develop\spartan\source\kernel\monitor\monitor.c
_TEXT	SEGMENT
_owner$ = 8						; size = 4
_IMonitor_GetCrc PROC

; 137  : u32 IMonitor_GetCrc(int owner) {

	push	esi

; 138  :     char *base = IMonitor_GetBase(owner);

	push	DWORD PTR _owner$[esp]
	call	_IMonitor_GetBase

; 139  :     int size = IMonitor_GetSize(owner);

	push	DWORD PTR _owner$[esp+4]
	mov	esi, eax
	call	_IMonitor_GetSize

; 140  :     return IMonitor_CalculateCrc(base, size);

	push	eax
	push	esi
	call	_IMonitor_CalculateCrc
	add	esp, 16					; 00000010H
	pop	esi

; 141  : }

	ret	0
_IMonitor_GetCrc ENDP
_TEXT	ENDS
; Function compile flags: /Ogspy
; File c:\users\cex123\desktop\fyp\develop\spartan\source\kernel\monitor\monitor.c
_TEXT	SEGMENT
_IMonitor_EnableErrorRecovery PROC

; 144  :     for (int m = 0; m < elementsof(map); m++) {

	xor	ecx, ecx
$LL3@IMonitor_E:

; 145  :         map[m][COPY] = map[m][BASE] + 256*KB;

	mov	eax, DWORD PTR _map[ecx+4]
	add	eax, 262144				; 00040000H
	mov	DWORD PTR _map[ecx+12], eax
	add	ecx, 20					; 00000014H
	cmp	ecx, 560				; 00000230H
	jb	SHORT $LL3@IMonitor_E

; 146  :     }
; 147  :     return true;

	xor	eax, eax
	inc	eax

; 148  : }

	ret	0
_IMonitor_EnableErrorRecovery ENDP
_TEXT	ENDS
; Function compile flags: /Ogspy
; File c:\users\cex123\desktop\fyp\develop\spartan\source\kernel\monitor\monitor.c
_TEXT	SEGMENT
_IMonitor_ClearMemory PROC

; 150  : bool IMonitor_ClearMemory(void) {

	push	ebx
	push	ebp
	push	esi
	push	edi

; 151  :     char *base = IMonitor_GetBase('VESA');

	mov	esi, 1447383873				; 56455341H
	push	esi
	call	_IMonitor_GetBase
	mov	edi, eax

; 152  :     char *copy = IMonitor_GetCopy('VESA');

	call	_IMonitor_GetCopy

; 153  :     int size = IMonitor_GetSize('VESA');

	push	esi
	mov	ebx, eax
	call	_IMonitor_GetSize

; 154  :     movsd(MMAP_CLEARFINISH, base, size/4);

	cdq
	and	edx, 3
	lea	esi, DWORD PTR [edx+eax]
	sar	esi, 2
	push	esi
	push	edi
	push	524288					; 00080000H
	call	_movsd

; 155  :     stosd(MMAP_CLEARSTART, 'xxxx', (MMAP_CLEARFINISH-MMAP_CLEARSTART)/4);

	push	130640					; 0001fe50H
	push	2021161080				; 78787878H
	mov	ebp, 1728				; 000006c0H
	push	ebp
	call	_stosd

; 156  :     movsd(base, MMAP_CLEARFINISH, size/4);

	push	esi
	push	524288					; 00080000H
	push	edi
	call	_movsd

; 157  :     movsd(copy, MMAP_CLEARFINISH, size/4);

	push	esi
	push	524288					; 00080000H
	push	ebx
	call	_movsd

; 158  :     stosd(MMAP_CLEARSTART, 'xxxx', size/4);

	push	esi
	push	2021161080				; 78787878H
	push	ebp
	call	_stosd
	add	esp, 68					; 00000044H

; 159  :     return true;

	xor	eax, eax
	inc	eax
	pop	edi
	pop	esi
	pop	ebp
	pop	ebx

; 160  : }

	ret	0
_IMonitor_ClearMemory ENDP
_TEXT	ENDS
; Function compile flags: /Ogspy
; File c:\users\cex123\desktop\fyp\develop\spartan\source\kernel\monitor\monitor.c
_TEXT	SEGMENT
_IMonitor_ProtectAll PROC

; 162  : bool IMonitor_ProtectAll(void) {

	push	esi

; 163  :     Logger(" [info] Creating Restore Point\n");
; 164  :     for (int id = 0; id < elementsof(map); id++) {

	xor	esi, esi
$LL7@IMonitor_P:

; 165  :         if (map[id][NAME] == 'DISK') break;

	cmp	DWORD PTR _map[esi], 1145656139		; 4449534bH
	je	SHORT $LN3@IMonitor_P

; 166  : 
; 167  :         int   size = map[id][SIZE];
; 168  :         char *base = (char*)map[id][BASE];
; 169  :         char *save = (char*)(base + 16*MB);
; 170  :         movsd(save, base, size/4);

	mov	eax, DWORD PTR _map[esi+8]
	mov	ecx, DWORD PTR _map[esi+4]
	cdq
	and	edx, 3
	add	eax, edx
	sar	eax, 2
	push	eax
	push	ecx
	lea	eax, DWORD PTR [ecx+16777216]
	push	eax
	call	_movsd
	add	esi, 20					; 00000014H
	add	esp, 12					; 0000000cH
	cmp	esi, 560				; 00000230H
	jb	SHORT $LL7@IMonitor_P
$LN3@IMonitor_P:

; 171  :       //map[id][CRC] = IMonitor_CalculateCrc(base, size);
; 172  :     }
; 173  :     Logger(" [info] Restore Point Created\n");
; 174  :     return true;

	xor	eax, eax
	inc	eax
	pop	esi

; 175  : }

	ret	0
_IMonitor_ProtectAll ENDP
_TEXT	ENDS
; Function compile flags: /Ogspy
; File c:\users\cex123\desktop\fyp\develop\spartan\source\kernel\monitor\monitor.c
_TEXT	SEGMENT
_IMonitor_RecoverAll PROC

; 177  : bool IMonitor_RecoverAll(void) {

	push	esi

; 178  :     Logger(" [info] Restoring Restore Point\n");
; 179  :     for (int id = 0; id < elementsof(map); id++) {

	xor	esi, esi
$LL7@IMonitor_R:

; 180  :         if (map[id][NAME] == 'DISK') break;

	cmp	DWORD PTR _map[esi], 1145656139		; 4449534bH
	je	SHORT $LN3@IMonitor_R

; 181  : 
; 182  :         int   size = map[id][SIZE];
; 183  :         char *base = (char*)map[id][BASE];
; 184  :         char *save = (char*)(base + 16*MB);
; 185  :         movsd(base, save, size/4);

	mov	eax, DWORD PTR _map[esi+8]
	mov	ecx, DWORD PTR _map[esi+4]
	cdq
	and	edx, 3
	add	eax, edx
	sar	eax, 2
	push	eax
	lea	eax, DWORD PTR [ecx+16777216]
	push	eax
	push	ecx
	call	_movsd
	add	esi, 20					; 00000014H
	add	esp, 12					; 0000000cH
	cmp	esi, 560				; 00000230H
	jb	SHORT $LL7@IMonitor_R
$LN3@IMonitor_R:

; 186  :     }
; 187  :     Logger(" [info] Restore Point Restored\n");
; 188  :     return true;

	xor	eax, eax
	inc	eax
	pop	esi

; 189  : }

	ret	0
_IMonitor_RecoverAll ENDP
_TEXT	ENDS
END
