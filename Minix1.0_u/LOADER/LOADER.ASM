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
NR_ZONE_NUMS equ 9
NR_DZONE_NUM equ NR_ZONE_NUMS - 2
NR_INDIRECTS equ 512

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
BlockBuff2		equ     01000h  ; block buffer  0x31000
BaseOfKernelFile	equ	06000h	; KERNEL.BIN should be load in this seg
OffsetOfKernelFile	equ	0000h	; KERNEL.BIN should be load in this offset
BaseOfMMFile		equ	07000h	; MM.BIN should be load in this seg
OffsetOfMMFile		equ	0000h	; MM.BIN should be load in this offset
BaseOfFSFile		equ	08000h	; FS.BIN should be load in this seg
OffsetOfFSFile		equ	0000h	; FS.BIN should be load in this offset
BaseOfINITFile		equ	02000h	; INIT.BIN should be load in this seg
OffsetOfINITFile	equ	0000h	; INIT.BIN should be load in this offset
BaseOfLoaderPhyAddr	equ	BaseOfLoader * 10h
BaseOfKernelFilePhyAddr equ     BaseOfKernelFile * 10h
BaseOfInitFilePhyAddr   equ     BaseOfINITFile * 10h
KernelEntryPoint	equ     030400h
PageDirBase		equ	100000h	; 页目录开始地址:		1M
PageTblBase		equ	101000h	; 页表开始地址:			1M + 4K
;================================================================================================
%define Arg16(i) [bp + 4 + (i)*2] 
%define Arg1632(i) [bp + 6 + (i)*4] 
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
INIT			db	"INIT.bin",0,0,0,0,0,0
MM			db	"MM.bin",0,0,0,0,0,0,0,0
FS			db	"FS.bin",0,0,0,0,0,0,0,0
LoadMessage		db	"Loading...",0
NoKernelMsg 		db	"No KERNEL",0
NoInitMsg 		db	"No INIT",0
FILENAME                db      "KERNEL.bin",0,0,0,0
;============================================================================

;============================================================================
;Stings
;----------------------------------------------------------------------------
PosSeg dw BaseOfKernelFile
KernelPosOffset dw OffsetOfKernelFile
_dwMCRNumber:			dd	0	; Memory Check Result
_MemChkBuf:	times	1024	db	0
;============================================================================
dwMCRNumber		equ	BaseOfLoaderPhyAddr + _dwMCRNumber
MemChkBuf		equ	BaseOfLoaderPhyAddr + _MemChkBuf

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
	mov	ax,BaseOfLoader - 01000h
	mov	ss,ax
	mov	sp,0000h
	   
	call cls
	call GetMemoSize
	
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
	
	mov si,KERNEL
	call setfilename
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
	mov eax,OffsetOfKernelFile
	call readfile


	;load INIT
	mov si,INIT
	call setfilename
	;find init inode
	mov bx,DirEntry
	call CmpFileName
	cmp bx,0ffffh
	jne .loadinginit
	;There are not KERNEL.bin
	mov bx,(80 * 2 + 0) * 3
	mov si,NoInitMsg
	call show_str
	jmp $

.loadinginit:
	mov word [PosSeg],BaseOfINITFile
	mov eax,OffsetOfINITFile
	call readfile



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


indzoneread:
	push ebp
	mov ebp,esp
	push edi
	push ebx
	push edx
	
	mov edi,Arg1632(0) ;zone list
	mov ebx,Arg1632(1) ;level

	mov dx,[es:edi]
	add edx,[es:SB_firstdatazone]
	shl edx,BLOCKSHIFT
	mov dword [BlockNumL], edx
	mov word [BufferSeg], SuperBlock
	mov word [BufferOffset], BlockBuff
	call ExInt13HR

	dec ebx
	push ebx
	push dword NR_INDIRECTS
	push dword BlockBuff	
	call readzones
	add esp,12
	
	pop edx
	pop ebx
	pop edi
	pop ebp
	ret

;--------------------------
zoneread:
;readzones(zone_nr *zlist, int level)
;read a zone.
;Build: Du Hui 2014/11/16 ; Update: Du Hui 2014/11/16
;--------------------------
	push ebp
	mov ebp,esp
	push edx
	push ebx
	push edi
	xor edx,edx
	mov edi,Arg1632(0) ;zone list
	mov ebx,Arg1632(1) ;level

	cmp ebx,0
	jne .indzone
	mov dx,[es:edi]
	add edx,[es:SB_firstdatazone]
	shl edx,BLOCKSHIFT
	mov dword [BlockNumL], edx
	mov dx,[PosSeg]
	mov word [BufferSeg], dx
	mov word [BufferOffset], ax
	call ExInt13HR
	cmp ax,0FC00h
	jne .offadd
	mov dx,[PosSeg]
	add dx,01000h
	mov [PosSeg], dx
.offadd:
	add ax,BLOCK_SIZE
	jmp .OK

.indzone:
	push ebx
	push edi
	call indzoneread
	add esp,8

.OK:
	pop edi
	pop ebx
	pop edx
	pop ebp
	ret

;--------------------------
readzones:
;readzones(zone_nr *zlist,int len, int level)
;read all zones in an inode.
;Build: Du Hui 2014/11/16 ; Update: Du Hui 2014/11/16
;--------------------------
	push ebp
	mov ebp,esp
	push edi
	push ecx
	push ebx

	mov edi,Arg1632(0) ;zone list
	mov ecx,Arg1632(1) ;lenth
	mov ebx,Arg1632(2) ;level

.readzones:
	cmp word [es:edi],0
	je .OK
	push ebx
	push edi
	call zoneread
	add esp,8
	inc edi
	inc edi
	loop .readzones
	

.OK:
	pop ebx
	pop ecx
	pop edi
	pop ebp
	ret

;--------------------------
readfile:
;
;read all zones in an inode.
;Receives: bx = inode num , eax = OffsetOfKernelFile         
;Returns: Nothing
;Build: Du Hui 2014/11/16 ; Update: Du Hui 2014/11/16
;--------------------------
	push edi
	push edx
	push ebx
	push ecx
	push esi
	
	xor edi,edi
	xor edx,edx
	dec bx ;inode count from 1 to x
	shl bx,INODESHIFT
	add edi,Inode
	add di,bx
	add edi,I_zone ;edi now point to zones

	push dword 0
	push dword NR_DZONE_NUM 
	push edi
	call readzones
	add esp,12

	mov ecx,2
	mov esi,1 ;level
	add edi,NR_DZONE_NUM * 2
.indrect:
	push esi
	push dword NR_ZONE_NUMS - NR_DZONE_NUM
	push edi
	call readzones	
	add esp,12
	inc esi
	inc edi
	inc edi
	loop .indrect



.OK:
	pop esi
	pop ecx
	pop ebx
	pop edx
	pop edi
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
setfilename:
;Build: Du Hui 2015/7/14 ; Update: Du Hui 2015/7/14
;--------------------------
		
		push bx
		push cx
		push ax

		xor bx,bx
		mov cx,14
.s1:
		mov al,[bx + si] ;source
		mov [bx+FILENAME],al
		inc bx
		loop .s1

		pop ax
		pop cx
		pop bx

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
;Receives:  es:bx = where dir is, ds:si = file name.
;Returns: bx = inode number.
;Build: Du Hui 2014/6/25 ; Update: Du Hui 2014/6/25
;--------------------------

		push cx
		push di

.find:		xor di,di
		mov cx,0eh

.s:		mov al,[es:(bx + di + D_name)]
		cmp [ds:(di + FILENAME)],al
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

;--------------------------
GetMemoSize:
;
;get memory size.
;Receives:  .
;Returns: .
;Build: Du Hui 2015/6/1 ; Update: Du Hui 2015/6/1
;--------------------------
	push eax
	push ebx
	push ecx
	push edx
	push edi

	mov	ebx, 0			
	mov	di, _MemChkBuf		; （Address Range Descriptor Structure）
.MemChkLoop:
	mov	eax, 0E820h		; eax = 0000E820h
	mov	ecx, 20			; ecx = 地址范围描述符结构的大小
	mov	edx, 0534D4150h		; edx = 'SMAP'
	int	15h			; int 15h
	jc	.MemChkFail
	add	di, 20
	inc	dword [_dwMCRNumber]	; dwMCRNumber = ARDS 的个数
	cmp	ebx, 0
	jne	.MemChkLoop
	jmp	.MemChkOK
.MemChkFail:
	mov	dword [_dwMCRNumber], 0
.MemChkOK:
	
	pop edi
	pop edx
	pop ecx
	pop ebx
	pop eax
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
	
	call SetupPaging
	call InitKernel
	call InitInit
	mov eax,dwMCRNumber
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


;--------------------------
InitInit:
;
;Load the kernel elf file where suitable.
;Returns:  nothing .
;Build: Du Hui 2012/10/22 ; Update: Du Hui 2012/10/22
;--------------------------
	push eax
	push ecx
	push esi

	xor ecx,ecx
	mov cx,word [BaseOfInitFilePhyAddr + 2Ch]	;ecx <- pELFHdr->e_phnum
	mov	esi, [BaseOfInitFilePhyAddr + 1Ch]	; esi <- pELFHdr->e_phoff
	add	esi, BaseOfInitFilePhyAddr		; esi <- OffsetOfKernel + pELFHdr->e_phoff

.s:	
	push dword [(esi + 10h)]
	mov eax,[(esi + 4h)]
	add eax,BaseOfInitFilePhyAddr
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



;--------------------------
SetupPaging:
;
;Set up page.
;Returns:  nothing .
;Build: Du Hui 2015/7/4 ; Update: Du Hui 2015/7/4
;--------------------------

	; 首先初始化页目录
	pushad
	mov	ecx, 1024		; 共 1K 个表项
	mov	edi, PageDirBase	; 此段首地址为 PageDirBase
	xor	eax, eax
	mov	eax, PageTblBase | PG_P  | PG_USU | PG_RWW
.1:
	stosd
	add	eax, 4096		; 为了简化, 所有页表在内存中是连续的.
	loop	.1

	; 再初始化所有页表 (1K 个, 4M 内存空间)
	mov	ecx, 1024 * 1024	; 共 1M 个页表项, 也即有 1M 个页
	mov	edi, PageTblBase	; 此段首地址为 PageTblBase
	xor	eax, eax
	mov	eax, PG_P  | PG_USU | PG_RWW
.2:
	stosd
	add	eax, 4096		; 每一页指向 4K 的空间
	loop	.2

	mov	eax, PageDirBase
	mov	cr3, eax
	mov	eax, cr0
	or	eax, 80000000h
	mov	cr0, eax
	jmp	short .3
.3:
	nop
	popad
	ret

