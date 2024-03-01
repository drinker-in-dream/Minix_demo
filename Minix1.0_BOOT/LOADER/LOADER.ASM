;TITLE Minix LOADER, Version 1         (LOADER.asm)

; nasm LOADER.asm -o LOADER.bin
; ==========================================
; This program is the loader of Minix.
; Last update: 09/10/2013    Toby_Du

%define Arg16(i) [bp + 4 + (i)*2] 
%define Arg32(i) [ebp + 8 + (i)*4]



org	0100h

	jmp  START


[SECTION s16]
[BITS	16]
START:	
	mov	ax, cs
	mov	ds, ax
	mov	es, ax
	mov	ss, ax
	mov	sp, 0100h
	
	call cls

	call boot



	;Cursor position
	mov bh,0
	mov dh,10 ;line
	mov dl,52 ;row
	mov ah,2
	int 10h


	jmp	$		


;--------------------------
show_str:
;
;Show a colorful string on the
;screen at specific position.
;Receives: bx = position
;          color = cl, ds:si = string address
;Returns: Nothing
;Build: Du Hui 2012/2/8 ; Update: Du Hui 2012/4/15
;--------------------------
		pushad
		push es

		push 0b800h
		pop es    ;es point to Video memory area
				
.s:		mov ah,0ch
		mov al,[si]  ;al =  the char, ah = the color.
		cmp al,0 
		je .ok		;jump to ok else
		mov [es:bx],ax
		inc bx
		inc bx
		inc si

		jmp short .s


.ok:		pop es
		popad

		ret

;--------------------------
cls:
;clear the screen.
;Receives:nothing.
;Returns: Nothing.
;Creat: Du Hui 2012/4/7 ; Update: Du Hui 2012/4/7
;--------------------------
		push ds
		push cx
		push bx
		push ax
		
		mov ax,0b800h
		mov ds,ax
		mov bx,0
		mov ax,0720H
		mov cx,2000
.scl:
		mov [bx],ax
		inc bx
		inc bx
		loop .scl
		
		pop ax
		pop bx
		pop cx
		pop ds
		ret

;--------------------------
boot:
;boot minix.
;Receives:nothing.
;Returns: Nothing.
;Creat: Du Hui 2013/9/9 ; Update: Du Hui 2012/9/9
;--------------------------

		ret

;--------------------------
reset_video:
;reset video mode and clear screen.
;Receives:nothing.
;Returns: Nothing.
;Creat: Du Hui 2013/9/10 ; Update: Du Hui 2012/9/10
;--------------------------
		push bp
		mov bp,sp
		pushad

		mov ax,Arg16(0)
		mov [cur_vid_mode],ax
		test ah,ah
		jnz	xvesa		; VESA extended mode?
		int	10h		; Reset video (ah = 0)
		jmp	setcur 
xvesa:		mov	bx, ax		; bx = extended mode
		mov	ax, 4F02h	; Reset video
		int	10h
setcur:		xor	dx, dx		; dl = column = 0, dh = row = 0
		xor	bh, bh		; Page 0
		mov	ah, 02h		; Set cursor position
		int	10h
		
		popad
		pop bp
		ret


; Stack
[SECTION .data]
	cur_vid_mode dw 0

