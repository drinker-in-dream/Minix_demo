;TITLE Minix LOADER, Version 1         (LOADER.asm)

; nasm LOADER.asm -o LOADER.bin
; ==========================================
; This program is the loader of Minix.
; Last update: 2014/06/26    Toby_Du

%include "pm.inc"
;---------------
BLOCKSHIFT equ 1
INODESHIFT equ 5
SUPERBLOCK equ 1
BLOCK_SIZE equ 1024

;================================================================================================
INODESIZE		equ     32
SBSIZE                  equ     20
DIRSTRUCTSZ		equ     16	 
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
BaseOfKernelFile	equ	07000h	; KERNEL.BIN should be load in this seg
OffsetOfKernelFile	equ	0000h	; KERNEL.BIN should be load in this offset
BaseOfLoaderPhyAddr	equ	BaseOfLoader * 10h
BaseOfKernelFilePhyAddr equ     BaseOfKernelFile * 10h
KernelEntryPoint	equ     030400h
;================================================================================================
%define Arg16(i) [bp + 4 + (i)*2] 
%define Arg32(i) [ebp + 8 + (i)*4]
%define nr_inode(i) [0400h + (i)*INODESIZE]
%define nr_dir(i) [0800h + (i)*DIRSTRUCTSZ]
%define blk2sec(i) (i)*2

	org	0000h
	jmp  START
[SECTION .gdt]
;GDT
					;     base           limit	       Attribute	
LABEL_GDT: 	            Descriptor		    0,	            0,  0                 ;
LABEL_DESC_FLAT_C:	    Descriptor              0,        0fffffh, DA_CR  | DA_32 | DA_LIMIT_4K
LABEL_DESC_FLAT_RW:	    Descriptor              0,        0fffffh, DA_DRW | DA_32 | DA_LIMIT_4K
LABEL_DESC_VIDEO:	    Descriptor	      0B8000h,         0ffffh,  DA_DRW	+ DA_DPL3	  ; Video Segment
; GDT 结束

GdtLen		equ	$ - LABEL_GDT
GdtPtr		dw	GdtLen - 1				; 段界限
		dd	BaseOfLoaderPhyAddr + LABEL_GDT		; 基地址

SelectorFlatC		equ	LABEL_DESC_FLAT_C	- LABEL_GDT
SelectorFlatRW		equ	LABEL_DESC_FLAT_RW	- LABEL_GDT
SelectorVideo		equ	LABEL_DESC_VIDEO	- LABEL_GDT + SA_RPL3

;============================================================================
;Stings
;----------------------------------------------------------------------------
KERNEL			db	"KERNEL.bin",0,0,0,0	
LoadMessage		db	"Loading...",0
NoKernelMsg 		db	"No KERNEL",0
;============================================================================

;===============================================================
;DAP Message
;---------------------------------------------------------------
	PacketSize	db 10h
	Reserved	db 0h
	BlockCount	dw 2h
	BufferOffset	dw 0h	
	BufferSeg       dw 0h	
	BlockNumL	dd 0h	
	BlockNumH	dd 0h	
;================================================================

; Stack
[SECTION .gs]
ALIGN	32
[BITS	32]
LABEL_STACK:
	times 1024 db 0

TopOfStack	equ	$ - LABEL_STACK - 1
; END of [SECTION .gs]

[SECTION s16]
[BITS	16]
START:
	mov	ax,cs
	mov	ds,ax
	mov	es,ax
	mov	ss,ax
	mov	sp,0000h
	   
	call cls
	
	;set es point to file info
	mov ax,SuperBlock
	mov es,ax

	;read super block
	mov word [BufferSeg], SuperBlock
	mov word [BufferOffset], 0
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

	;find kernel inode
	mov bx,DirEntry
	call CmpFileName
	cmp bx,0ffffh
	jne .loading
	;There are not KERNEL.bin
	mov bx,(80 * 2 + 0) * 2
	mov si,NoKernelMsg
	call show_str
	jmp $

.loading:	
	;read loader	
	xor di,di
	xor edx,edx
	mov eax,OffsetOfKernelFile
	dec bx
	shl bx,INODESHIFT
	mov dx,[es:(Inode + bx + I_zone + di)]
.reading:	
	add edx,[es:SB_firstdatazone]
	shl edx,BLOCKSHIFT
	mov dword [BlockNumL], edx
	mov word [BufferSeg], BaseOfKernelFile
	mov word [BufferOffset], ax
	call ExInt13HR
	inc di
	inc di
	add ax,BLOCK_SIZE
	mov dx,[es:(Inode + bx + I_zone + di)]
	cmp edx,0
	jne .reading

	;Loading msg
	mov bx,(80 * 2 + 0) * 2
	mov si,LoadMessage
	call show_str
		
	;Cursor position
	mov bh,0
	mov dh,21 ;line
	mov dl,0 ;row
	mov ah,2
	int 10h

	; Jump to protect mode -------------------------------------------
	; load GDTR
	lgdt	[GdtPtr]
	; turn off interrupt
	cli
	; open A20
	in al, 92h
	or al, 00000010b
	out 92h, al
	; switch to protect mode 
	mov eax, cr0
	or eax, 1
	mov cr0, eax
	; after this jump system run in protect mode
	jmp	dword SelectorFlatC:(BaseOfLoaderPhyAddr+LABEL_PM_START)

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
		cmp [ds:(di + KERNEL)],al
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

; protect mode code ----------------------------------------------------
[SECTION .s32]

ALIGN	32

[BITS	32]

LABEL_PM_START:
	mov ax,SelectorVideo
	mov gs,ax
	mov ax,SelectorFlatRW
	mov ds,ax
	mov es,ax
	mov fs,ax
	mov ss,ax
	mov esp,TopOfStack

	call InitKernel

	jmp SelectorFlatC:KernelEntryPoint

;--------------------------
MemCpy:
;
;Copy memery from sourec to des.
;Receives: es:Arg0 = where to load the seg,
;          ds:Arg1 = where the seg is.
;          Arg2 = filesize .
;Returns:  nothing .
;Build: Du Hui 2012/10/22 ; Update: Du Hui 2012/10/22
;--------------------------
	
	push ebp
	mov ebp,esp
	push eax
	push ecx
	push esi
	push edi
	
	mov ecx,Arg32(2)
	mov edi,Arg32(0)
	mov esi,Arg32(1)
	cmp ecx,0
	je .ok
.s:	
	mov al,[ds:esi]
	mov byte [es:edi], al
	inc esi
	inc edi
	loop .s

.ok:
	pop edi
	pop esi
	pop ecx
	pop eax
	pop ebp
	ret
;--------------------------
InitKernel:
;
;Load the kernel elf file where suitable.
;Returns:  nothing .
;Build: Du Hui 2012/10/22 ; Update: Du Hui 2012/10/22
;--------------------------
	push eax
	push ecx
	push esi

	xor ecx,ecx
	mov cx,word [BaseOfKernelFilePhyAddr + 2Ch]	;ecx <- pELFHdr->e_phnum
	mov	esi, [BaseOfKernelFilePhyAddr + 1Ch]	; esi <- pELFHdr->e_phoff
	add	esi, BaseOfKernelFilePhyAddr		; esi <- OffsetOfKernel + pELFHdr->e_phoff

.s:	
	push dword [(esi + 10h)]
	mov eax,[(esi + 4h)]
	add eax,BaseOfKernelFilePhyAddr
	push eax
	push dword [esi + 08h]
	call MemCpy
	add esp, 12
	add esi,20h
	loop .s
	
	pop esi
	pop ecx
	pop eax
	ret
