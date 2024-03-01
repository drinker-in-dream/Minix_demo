
; ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
;                              sendrec.asm
; ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
;                                                       Toby Du, 2014/7/21
; ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
%include "../H/sconst.inc"

[SECTION .text]
global	send
global	receive
global	sendrec
global	send_rec

;========================================================================
;                           send and receive                              
;========================================================================
; send(), receive(), send_rec() all save bp, but destroy ax, bx, and cx.

send:
	push ebp
	mov ebp,esp 
	push ebx
	push ecx
	mov eax,SEND		; send(dest, ptr)
	mov ebx,[ebp +8]	;dest
	mov ecx,[ebp +12]	;ptr
	jmp L0

receive:
	push ebp
	mov ebp,esp
	push ebx
	push ecx
	mov eax,RECEIVE		; receive(src, ptr)
	mov ebx,[ebp +8]	;src
	mov ecx,[ebp +12]	;ptr
	jmp L0

sendrec:
send_rec:
	push ebp
	mov ebp,esp
	push ebx
	push ecx
	mov eax,BOTH		; send_rec(srcdest, ptr)
	mov ebx,[ebp +8]	;srcdest
	mov ecx,[ebp +12]	;ptr
	jmp L0

  L0:			
	int INT_VECTOR_SYS_CALL	
	
	pop ecx
	pop ebx
	pop ebp
	ret
	


