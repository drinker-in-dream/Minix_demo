
; ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
;                              klib.asm
; ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
;                                                       Toby Du, 2014/7/12
; ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
%include "sconst.inc"
extern	disp_pos

[SECTION .data]

lockvar	   DD	 0		; place	to store flags for lock()/restore()

[SECTION .text]

; 导出函数
global	disp_str
global	disp_color_str
global	out_byte
global	in_byte
global	memcpy
global	memset
global disable_irq
global enable_irq
global _lock
global _unlock
global _restore
global cls
global cp_mess


;===========================================================================
;				lock
;===========================================================================
; Disable CPU interrupts.
_lock:
	pushfd			; save flags on	stack
	cli			; disable interrupts
	pop dword [lockvar]		; save flags for possible restoration later
	ret			; return to caller


;===========================================================================
;				unlock
;===========================================================================
; Enable CPU interrupts.
_unlock:
	sti			; enable interrupts
	ret			; return to caller

;===========================================================================
;				restore
;===========================================================================
; Restore enable/disable bit to	the value it had before	last lock.
_restore:
	push dword [lockvar]		; push flags as	they were before previous lock
	popfd			; restore flags
	ret			; return to caller

; ========================================================================
;                  void disable_irq(int irq);
; ========================================================================
; Disable an interrupt request line by setting an 8259 bit.
; Equivalent code for irq < 8:
;       out_byte(INT_CTLMASK, in_byte(INT_CTLMASK) | (1 << irq));
; Returns true iff the interrupt was not already disabled.
;
disable_irq:
	mov	ecx, [esp + 4]		; irq
	pushf
	cli
	mov	ah, 1
	rol	ah, cl			; ah = (1 << (irq % 8))
	cmp	cl, 8
	jae	disable_8		; disable irq >= 8 at the slave 8259
disable_0:
	in	al, INT_M_CTLMASK
	test	al, ah
	jnz	dis_already		; already disabled?
	or	al, ah
	out	INT_M_CTLMASK, al	; set bit at master 8259
	popf
	mov	eax, 1			; disabled by this function
	ret
disable_8:
	in	al, INT_S_CTLMASK
	test	al, ah
	jnz	dis_already		; already disabled?
	or	al, ah
	out	INT_S_CTLMASK, al	; set bit at slave 8259
	popf
	mov	eax, 1			; disabled by this function
	ret
dis_already:
	popf
	xor	eax, eax		; already disabled
	ret

; ========================================================================
;                  void enable_irq(int irq);
; ========================================================================
; Enable an interrupt request line by clearing an 8259 bit.
; Equivalent code:
;	if(irq < 8){
;		out_byte(INT_M_CTLMASK, in_byte(INT_M_CTLMASK) & ~(1 << irq));
;	}
;	else{
;		out_byte(INT_S_CTLMASK, in_byte(INT_S_CTLMASK) & ~(1 << irq));
;	}
;
enable_irq:
        mov	ecx, [esp + 4]		; irq
        pushf
        cli
        mov	ah, ~1
        rol	ah, cl			; ah = ~(1 << (irq % 8))
        cmp	cl, 8
        jae	enable_8		; enable irq >= 8 at the slave 8259
enable_0:
        in	al, INT_M_CTLMASK
        and	al, ah
        out	INT_M_CTLMASK, al	; clear bit at master 8259
        popf
        ret
enable_8:
        in	al, INT_S_CTLMASK
        and	al, ah
        out	INT_S_CTLMASK, al	; clear bit at slave 8259
        popf
        ret



;===========================================================================
;				cp_mess
;===========================================================================
; This routine is makes	a fast copy of a message from anywhere in the address
; space	to anywhere else.  It also copies the source address provided as a
; parameter to the call	into the first word of the destination message.
; It is	called by:
;    cp_mess[src, src_clicks, src_offset, dst_clicks, dst_offset]
; where	all 5 parameters are unsignd ints [32-bits].
;
; Note that the	message	size, 'Msize' is in WORDS [not bytes] and must be set
; correctly.  Changing the definition of message the type file and not changing
; it here will lead to total disaster.
; This routine destroys	ax.  It	preserves the other registers.
Msize		equ	9
cp_mess:
	push	ebp
	mov	ebp, esp
	push es
	push ds
	pushf
	cli
	push ecx
	push esi
	push edi
	push ebx
	xor ecx,ecx

	mov eax,[ebp + 8]		; ax = process number of sender
	mov edi,[ebp + 24]		; di = offset of destination buffer
	mov ecx,[ebp + 20]		; 
	;mov es,cx			; es = clicks of destination
	mov esi,[ebp + 16]		; si = offset of source	message
	mov ecx,[ebp + 12]		;
	;mov ds,cx			; ds = clicks of source	message
	mov [edi],eax		; copy sender's	process	number to dest message
	mov ecx,Msize-1			; remember, first word doesn't count
.s:
	add si,4			; don't	copy first word
	add di,4			; don't	copy first word
	mov ebx,[esi]
	mov [edi],ebx
	loop .s
	
	
	pop ebx
	pop edi
	pop esi
	pop ecx
	popf
	pop ds
	pop es
	pop ebp
	ret

;;-------------------------------FOR DEBUG

; ========================================================================
;                  void disp_str(char * pszInfo);
; ========================================================================
disp_str:
	push	ebp
	mov	ebp, esp

	mov	esi, [ebp + 8]	; pszInfo
	mov	edi, [disp_pos]
	mov	ah, 0Ch
.1:
	lodsb
	test	al, al
	jz	.2
	cmp	al, 0Ah	; is that enter?
	jnz	.3
	push	eax
	mov	eax, edi
	mov	bl, 160
	div	bl
	and	eax, 0FFh
	inc	eax
	mov	bl, 160
	mul	bl
	mov	edi, eax
	pop	eax
	jmp	.1
.3:
	mov	[gs:edi], ax
	add	edi, 2
	jmp	.1

.2:
	mov	[disp_pos], edi

	pop	ebp
	ret


; ========================================================================
;                  void cls();
; ========================================================================
cls:
		pushad

		xor bx,bx
		mov ax,0720H
		mov cx,2000
.scl:
		mov [gs:bx],ax
		inc bx
		inc bx
		loop .scl

		mov dword [disp_pos],0
		
		popad
		ret

; ========================================================================
;                  void disp_color_str(char * info, int color);
; ========================================================================
disp_color_str:
	push	ebp
	mov	ebp, esp

	mov	esi, [ebp + 8]	; pszInfo
	mov	edi, [disp_pos]
	mov	ah, [ebp + 12]	; color
.1:
	lodsb
	test	al, al
	jz	.2
	cmp	al, 0Ah	; is that enter?
	jnz	.3
	push	eax
	mov	eax, edi
	mov	bl, 160
	div	bl
	and	eax, 0FFh
	inc	eax
	mov	bl, 160
	mul	bl
	mov	edi, eax
	pop	eax
	jmp	.1
.3:
	mov	[gs:edi], ax
	add	edi, 2
	jmp	.1

.2:
	mov	[disp_pos], edi

	pop	ebp
	ret

; ========================================================================
;                  void out_byte(t_port port, t_8 value);
; ========================================================================
out_byte:
	mov	edx, [esp + 4]		; port
	mov	al, [esp + 4 + 4]	; value
	out	dx, al
	nop	; a little delay
	nop
	ret

; ========================================================================
;                  t_8 in_byte(t_port port);
; ========================================================================
in_byte:
	mov	edx, [esp + 4]		; port
	xor	eax, eax
	in	al, dx
	nop	; a little delay
	nop
	ret

; ------------------------------------------------------------------------
; void* memcpy(void* es:pDest, void* ds:pSrc, int iSize);
; ------------------------------------------------------------------------
memcpy:
	push	ebp
	mov	ebp, esp

	push	esi
	push	edi
	push	ecx

	mov	edi, [ebp + 8]	; Destination
	mov	esi, [ebp + 12]	; Source
	mov	ecx, [ebp + 16]	; Counter
.1:
	cmp	ecx, 0		
	jz	.2		; 

	mov	al, [ds:esi]		; ┓
	inc	esi			; ┃
					; ┣ byte by byte
	mov	byte [es:edi], al	; ┃
	inc	edi			; ┛

	dec	ecx		; 
	jmp	.1		; 
.2:
	mov	eax, [ebp + 8]	; return value

	pop	ecx
	pop	edi
	pop	esi
	mov	esp, ebp
	pop	ebp

	ret			;
; memcpy end-------------------------------------------------------------

; ------------------------------------------------------------------------
; void memset(void* p_dst, char ch, int size);
; ------------------------------------------------------------------------
memset:
	push	ebp
	mov	ebp, esp

	push	esi
	push	edi
	push	ecx

	mov	edi, [ebp + 8]	; Destination
	mov	edx, [ebp + 12]	; Char to be putted
	mov	ecx, [ebp + 16]	; Counter
.1:
	cmp	ecx, 0		; 判断计数器
	jz	.2		; 计数器为零时跳出

	mov	byte [edi], dl		; ┓
	inc	edi			; ┛

	dec	ecx		; 计数器减一
	jmp	.1		; 循环
.2:

	pop	ecx
	pop	edi
	pop	esi
	mov	esp, ebp
	pop	ebp

	ret			; 函数结束，返回
; ------------------------------------------------------------------------