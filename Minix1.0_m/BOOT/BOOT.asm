;TITLE nasm BOOT, Version 1         (BOOT.asm)

; nasm BOOT.asm -o BOOT.bin
; ==========================================
; This program is the booter of Minix.
; Last update: 2014/06/26    Toby_Du


	org  07c00h			
;---------------
BLOCKSHIFT equ 1
INODESHIFT equ 5
SUPERBLOCK equ 1
BLOCK_SIZE equ 1024

;================================================================================================
INODESIZE		equ     32
SBSIZE                  equ     20
DIRSTRUCTSZ		equ     16
BaseOfStack		equ	7c00h	 
BaseOfLoader		equ	09000h	; LOADER.BIN should be load in this seg
OffsetOfLoader		equ	0000h	; LOADER.BIN should be load in this offset
SuperBlock              equ     03000h  ; super block load here
SB_ninodes		equ     0       ; offset of s_ninodes
SB_nzones		equ     2	; offset of s_nzones
SB_imap_blocks		equ     4	; offset of s_imap_blocks
SB_zmap_blocks		equ     6       ; offset of s_zmap_blocks
SB_firstdatazone	equ	8	; offset of s_firstdatazone
SB_log_zone_size	equ     10	; offset of s_log_zone_size
SB_maxsize		equ     12      ; offset of s_maxsize
SB_magic		equ     16	; offset of s_magic
Inode			equ     0400h   ; loader inode here 0x30400
I_mode			equ     0	; offset of i_mode
I_uid			equ	2	; offset of i_uid
I_size			equ	4	; offset of i_size
I_modtime		equ     8	; offset of i_modtime
I_gid			equ	12	; offset of i_gid
I_nlinks		equ	13	; offset of i_nlinks
I_zone			equ	14	; offset of i_zone
DirEntry		equ     0800h  ; dir entry here 0x30800
D_inum			equ	0	; offset of d_inum
D_name			equ	2	; offset of d_name
BlockBuff		equ     0C00h  ; block buffer  0x30C00

;================================================================================================
%define Arg16(i) [bp + 4 + (i)*2]
%define nr_inode(i) [0400h + (i)*INODESIZE]
%define nr_dir(i) [0800h + (i)*DIRSTRUCTSZ]
%define blk2sec(i) (i)*2

;--------------------------------------------------------------------------------------------------

	jmp short LABEL_START		; Start to boot.
	nop				; this nop is needed

LABEL_START:	
	mov	ax, cs
	mov	ds, ax
	mov	ss, ax
	mov	sp, BaseOfStack

	mov ax,SuperBlock
	mov es,ax

	call cls

	;read super block
	mov dword [BlockNumL], blk2sec(1)
	call ExInt13HR

	;read root directry	
	mov word [BufferOffset], DirEntry
	mov eax,[es:SB_firstdatazone]
	shl eax,BLOCKSHIFT
	mov dword [BlockNumL], eax
	call ExInt13HR
	
	;read inode list
	mov eax,SUPERBLOCK + 1  ; imap block starts here
	add ax,[es:SB_imap_blocks]
	add ax,[es:SB_zmap_blocks]
	shl eax,BLOCKSHIFT
	mov dword [BlockNumL], eax
	mov word [BufferOffset], Inode
	call ExInt13HR

	;find loader inode
	mov bx,DirEntry
	call CmpFileName
	cmp bx,0ffffh
	jne .loading
	;There are not LOADER.bin
	mov bx,(80 * 2 + 0) * 2
	mov si,NoLoaderMsg
	call show_str
	jmp $
		
.loading:	
	;read loader	
	xor di,di
	xor edx,edx
	mov eax,OffsetOfLoader
	dec bx
	shl bx,INODESHIFT
	mov dx,[es:(Inode + bx + I_zone + di)]
.readloader:	
	add edx,[es:SB_firstdatazone]
	shl edx,BLOCKSHIFT
	mov dword [BlockNumL], edx
	mov word [BufferSeg], BaseOfLoader
	mov word [BufferOffset], ax
	call ExInt13HR
	inc di
	inc di
	add ax,BLOCK_SIZE
	mov dx,[es:(Inode + bx + I_zone + di)]
	cmp edx,0
	jne .readloader
		
	jmp	BaseOfLoader:OffsetOfLoader




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
ExInt13HR:
;
;Int 13h LBA read sectors.
;Receives: 
;Returns: Nothing
;Build: Du Hui 2013/3/2 ; Update: Du Hui 2013/3/2
;--------------------------
	pushad
	mov ah,0x42			
	mov dl,80H							
	mov si,PacketSize			  
	int 13H					
	popad
	ret

;--------------------------
CmpFileName:
;
;find file.
;Receives:  es:bx = where dir is.
;Returns: bx = inode number.
;Build: Du Hui 2014/6/25 ; Update: Du Hui 2014/6/25
;--------------------------

		push cx
		push di

.find:		xor di,di
		mov cx,0eh

.s:		mov al,[es:(bx + di + D_name)]
		cmp [ds:(di + LoaderFileName)],al
		jne .next
		inc di
		loop .s
		jmp short .matched

.next:		add bx,16
		cmp bx,DirEntry + 400h
		je  .nomatched
		jmp short .find
			
.matched:	
		mov bx,[es:bx]
		jmp short .finish
		
.nomatched:	or bx,0ffffh
	
.finish:	pop di
		pop cx		
		ret

;===============================================================
;Ex Int 13h DAP Message
;---------------------------------------------------------------
	PacketSize	db 10h
	Reserved	db 0h
	BlockCount	dw 2h
	BufferOffset	dw 0h	
	BufferSeg       dw SuperBlock	
	BlockNumL	dd 0h	
	BlockNumH	dd 0h	

;============================================================================
;Stings
;----------------------------------------------------------------------------
LoaderFileName:		db	"LOADER.bin",0,0,0,0
NoLoaderMsg:		db	"No LOADER",0

times 	510-($-$$)	db	0	
dw 	0xaa55				

