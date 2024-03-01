; ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
;                               KERNEL.asm
; ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
;                                                 Toby Du, 2014/06/30
; ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++#

; import function
extern	main

; import variable
extern	gdt_ptr
extern	idt_ptr

[SECTION .bss]
StackSpace		resb	2 * 1024
StackTop:		; top of stack

[section .text]	; code here

global _start	; ecport _start to link

_start:

	; mov esp from LOADER to KERNEL
	mov	esp, StackTop	
	sgdt	[gdt_ptr]	
	call	main		
	lgdt	[gdt_ptr]	


M1:	jmp	M1  ; this should never be executed




