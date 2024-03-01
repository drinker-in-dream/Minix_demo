
; ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
;                               pm.inc
; ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
;                                                     Toby Du, 2013
; ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++


;----------------------------------------------------------------------------
; 描述符类型值说明
; 其中:
;       DA_  : Descriptor Attribute
;       D    : Data Segment
;       C    : Code Segment
;       S    : System Segment
;       R    : Read Only
;       RW   : Read & Write
;       A    : Accessed
;----------------------------------------------------------------------------
DA_32		EQU	4000h	; 32 Bit Segment
DA_LIMIT_4K	EQU	8000h	;  4K Bit 

DA_DPL0		EQU	  00h	; DPL = 0
DA_DPL1		EQU	  20h	; DPL = 1
DA_DPL2		EQU	  40h	; DPL = 2
DA_DPL3		EQU	  60h	; DPL = 3
;----------------------------------------------------------------------------
; Storage segment definitions
;----------------------------------------------------------------------------
DA_DR		EQU	90h	; 
DA_DRW		EQU	92h	; 
DA_DRWA		EQU	93h	; 
DA_C		EQU	98h	; 
DA_CR		EQU	9Ah	; 
DA_CCO		EQU	9Ch	; 
DA_CCOR		EQU	9Eh	; 
;----------------------------------------------------------------------------
; System segment definitions
;----------------------------------------------------------------------------
DA_LDT		EQU	  82h	; 
DA_TaskGate	EQU	  85h	; 
DA_386TSS	EQU	  89h	; 
DA_386CGate	EQU	  8Ch	; 
DA_386IGate	EQU	  8Eh	; 
DA_386TGate	EQU	  8Fh	;
;----------------------------------------------------------------------------

;----------------------------------------------------------------------------
; Sector describe
;       SA_  : Selector Attribute

SA_RPL0		EQU	0	; ┓
SA_RPL1		EQU	1	; ┣ RPL
SA_RPL2		EQU	2	; ┃
SA_RPL3		EQU	3	; ┛

SA_TIG		EQU	0	; ┓TI
SA_TIL		EQU	4	; ┛
;----------------------------------------------------------------------------


;----------------------------------------------------------------------------
; Paging Macro 
;----------------------------------------------------------------------------
PG_P		EQU	1	; 
PG_RWR		EQU	0	; R/W 
PG_RWW		EQU	2	; R/W 
PG_USS		EQU	0	; U/S 
PG_USU		EQU	4	; U/S 
;----------------------------------------------------------------------------




; =========================================
; FLAGS - Intel 8086 Family Flags Register
; =========================================
;
;      |11|10|F|E|D|C|B|A|9|8|7|6|5|4|3|2|1|0|
;        |  | | | | | | | | | | | | | | | | '---  CF……Carry Flag
;        |  | | | | | | | | | | | | | | | '---  1
;        |  | | | | | | | | | | | | | | '---  PF……Parity Flag
;        |  | | | | | | | | | | | | | '---  0
;        |  | | | | | | | | | | | | '---  AF……Auxiliary Flag
;        |  | | | | | | | | | | | '---  0
;        |  | | | | | | | | | | '---  ZF……Zero Flag
;        |  | | | | | | | | | '---  SF……Sign Flag
;        |  | | | | | | | | '---  TF……Trap Flag  (Single Step)
;        |  | | | | | | | '---  IF……Interrupt Flag
;        |  | | | | | | '---  DF……Direction Flag
;        |  | | | | | '---  OF……Overflow flag
;        |  | | | '-----  IOPL……I/O Privilege Level  (286+ only)
;        |  | | '-----  NT……Nested Task Flag  (286+ only)
;        |  | '-----  0
;        |  '-----  RF……Resume Flag (386+ only)
;        '------  VM……Virtual Mode Flag (386+ only)
;
;        注: see   PUSHF  POPF  STI  CLI  STD  CLD
;


; 宏 ------------------------------------------------------------------------------------------------------
;
; 描述符
; usage: Descriptor Base, Limit, Attr
;        Base:  dd
;        Limit: dd (low 20 bits available)
;        Attr:  dw (lower 4 bits of higher byte are always 0)
%macro Descriptor 3
	dw	%2 & 0FFFFh				; 段界限 1				(2 字节)
	dw	%1 & 0FFFFh				; 段基址 1				(2 字节)
	db	(%1 >> 16) & 0FFh			; 段基址 2				(1 字节)
	dw	((%2 >> 8) & 0F00h) | (%3 & 0F0FFh)	; 属性 1 + 段界限 2 + 属性 2		(2 字节)
	db	(%1 >> 24) & 0FFh			; 段基址 3				(1 字节)
%endmacro ; 共 8 字节
;
; 门
; usage: Gate Selector, Offset, DCount, Attr
;        Selector:  dw
;        Offset:    dd
;        DCount:    db
;        Attr:      db
%macro Gate 4
	dw	(%2 & 0FFFFh)				; 偏移 1				(2 字节)
	dw	%1					; 选择子				(2 字节)
	dw	(%3 & 1Fh) | ((%4 << 8) & 0FF00h)	; 属性					(2 字节)
	dw	((%2 >> 16) & 0FFFFh)			; 偏移 2				(2 字节)
%endmacro ; 共 8 字节
; ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
