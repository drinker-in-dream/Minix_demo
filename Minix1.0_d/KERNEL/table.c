
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
#include "protect.h"
#include "proc.h"
#include "glo.h"

EXTERN	int		disp_pos;
EXTERN	u8		gdt_ptr[6];	// 0~15:Limit  16~47:Base
EXTERN	DESCRIPTOR	gdt[GDT_SIZE];
EXTERN	u8		idt_ptr[6];	// 0~15:Limit  16~47:Base
EXTERN	GATE		idt[IDT_SIZE];

EXTERN	TSS		tss;
EXTERN	u32		k_reenter;
EXTERN t_pf_irq_handler	irq_table[NR_IRQ];


EXTERN	char			task_stack[STACK_SIZE_TOTAL];
EXTERN struct proc *proc_ptr;	/* &proc[cur_proc] */

