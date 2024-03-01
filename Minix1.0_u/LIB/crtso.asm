
extern	main
extern  exit

[section .text]	; code here

global _start	; export _start to link
global environ 

_start:
	mov ebx,esp
	mov ecx,[ebx] ;get argc
	add ebx,4     ;point at next parameter
	mov eax,ecx   ;
	inc eax       ;calculate envp
	shl eax,2     ;
        add eax,ebx   

	mov [environ],eax
	push	eax		; stack envp
	push	ebx		; stack argv
	push	ecx		; stack argc
	call main

	add	esp,12
	push	eax		; stack program-termination status
	call	exit		; this will never return


[section .data]	; data here
environ dd 0


