
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
EXTERN struct s_proc *proc_ptr;	/* &proc[cur_proc] */

extern int sys_task(), clock_task(), mem_task(), floppy_task(),
           winchester_task(), tty_task(), printer_task();

/* The startup routine of each task is given below, from -NR_TASKS upwards.
 * The order of the names here MUST agree with the numerical values assigned to
 * the tasks in ../h/com.h.
 */
int (*task[NR_TASKS+INIT_PROC_NR+1])() = {
 printer_task, tty_task, winchester_task, floppy_task, mem_task,
 clock_task, sys_task, 0, 0, 0, 0
};

