
/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
                            table.c
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
                                                    Toby Du, 2014/7/1
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

#include "../H/const.h"
#include "../H/type.h"
#include "const.h"
#include "type.h"
#undef   EXTERN
#define  EXTERN
#include "glo.h"
#include "proc.h"
#include "protect.h"

//------------->> P R O G R A M    S E T U P <<-------------
EXTERN u8		gdt_ptr[6];	// 0~15:Limit  16~47:Base
EXTERN DESCRIPTOR	gdt[GDT_SIZE];
EXTERN u8		idt_ptr[6];	// 0~15:Limit  16~47:Base
EXTERN GATE		idt[IDT_SIZE];
EXTERN t_pf_irq_handler	irq_table[NR_IRQ];
EXTERN	int		disp_pos;
EXTERN	u32		k_reenter;
EXTERN	TSS		tss;
#define STACK_SIZE_TOTAL	0x200
EXTERN	char	task_stack[STACK_SIZE_TOTAL];






