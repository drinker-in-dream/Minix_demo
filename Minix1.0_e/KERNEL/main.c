
/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
                            main.c
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
                                                  Toby Du, 2014/6/30
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
#include "../H/const.h"
#include "../H/type.h"
#include "const.h"
#include "type.h"
#include "protect.h"
#include "proc.h"
#include "glo.h"

EXTERN	int		disp_pos;
EXTERN	u32		k_reenter;
EXTERN	DESCRIPTOR	gdt[GDT_SIZE];
EXTERN	char task_stack[STACK_SIZE_TOTAL];
EXTERN struct s_proc *proc_ptr;	/* &proc[cur_proc] */
PUBLIC void clock_handler(int irq);
PUBLIC void put_irq_handler(int irq, t_pf_irq_handler handler);

/*======================================================================*
                            main
 *======================================================================*/
PUBLIC int main(){

	struct s_proc* p_proc	= proc;
	u16		selector_ldt	= SELECTOR_LDT_FIRST;
	int i=0;
	extern int (*task[])();
	_lock();			/* we can't handle interrupts yet */
	cls();
	disp_str("\n\n\n\n                    Hello Minix! \n");

	for (p_proc = &proc[0]; p_proc <= &proc[NR_TASKS+LOW_USER]; p_proc++,i++) {
		p_proc->p_pid	= i;			/* pid */
		p_proc->ldt_sel	= selector_ldt;
		memcpy(&p_proc->ldts[0], &gdt[SELECTOR_KERNEL_CS >> 3], sizeof(DESCRIPTOR));
		p_proc->ldts[0].attr1 = DA_C | PRIVILEGE_TASK << 5;	// change the DPL
		memcpy(&p_proc->ldts[1], &gdt[SELECTOR_KERNEL_DS >> 3], sizeof(DESCRIPTOR));
		p_proc->ldts[1].attr1 = DA_DRW | PRIVILEGE_TASK << 5;	// change the DPL
		p_proc->regs.cs		= ((8 * 0) & SA_RPL_MASK & SA_TI_MASK) | SA_TIL | RPL_TASK;
		p_proc->regs.ds		= ((8 * 1) & SA_RPL_MASK & SA_TI_MASK) | SA_TIL | RPL_TASK;
		p_proc->regs.es		= ((8 * 1) & SA_RPL_MASK & SA_TI_MASK) | SA_TIL | RPL_TASK;
		p_proc->regs.fs		= ((8 * 1) & SA_RPL_MASK & SA_TI_MASK) | SA_TIL | RPL_TASK;
		p_proc->regs.ss		= ((8 * 1) & SA_RPL_MASK & SA_TI_MASK) | SA_TIL | RPL_TASK;
		p_proc->regs.gs		= (SELECTOR_KERNEL_GS & SA_RPL_MASK) | RPL_TASK;
		p_proc->regs.eip	= (u32)task[i];
		p_proc->regs.esp	= (u32) task_stack + (i+1)*1024;
		p_proc->regs.eflags	= 0x1202;	// IF=1, IOPL=1, bit 2 is always 1.

		selector_ldt += 1 << 3;
	}

	k_reenter	= 0;

	proc_ptr=proc;
	put_irq_handler(CLOCK_IRQ, clock_handler);;
	/* Now go to the assembly code to start running the current process. */
	out_byte(INT_M_CTLMASK,	0);	// Master 8259, OCW1. 
	out_byte(INT_S_CTLMASK,	0);	// Slave  8259, OCW1.



	restart();
	while(1){}
	return 0;
}

/*======================================================================*
                           clock_handler
 *======================================================================*/
PUBLIC void clock_handler(int irq)
{	
	proc_ptr++ ;
	if (proc_ptr > proc + 6) {
		proc_ptr = proc;
	}
	
}

/*======================================================================*
                               sys_task
 *======================================================================*/
void sys_task()
{	int i = 0;
	while(1){
		disp_str(" Toby");
		disp_str(" ");
		delay(1);
	}
}


/*======================================================================*
                               clock_task
 *======================================================================*/
void clock_task()
{
	int i = 0;
	while(1){
		disp_str(" Yammy");
		disp_str(" ");
		delay(2);
	}
}


/*======================================================================*
                               mem_task
 *======================================================================*/
void mem_task()
{	
	int i = 0;
	while(1){
		disp_str("LOVE");
		disp_str("->");
		delay(4);
	}
	
}

/*======================================================================*
                               floppy_task
 *======================================================================*/
void floppy_task()
{	
	int i = 0;
	while(1){
		if(disp_pos > 2000){
			cls();
		}
		delay(16);
	}
	
}

/*======================================================================*
                               winchester_task
 *======================================================================*/
void winchester_task()
{	
	int i = 0;
	while(1){
		disp_str("care");
		disp_str("->");
		delay(8);
	}
	
}


/*======================================================================*
                               tty_task
 *======================================================================*/
void tty_task()
{	
	int i = 0;
	while(1){
		disp_str("eat");
		disp_str("->");
		delay(10);
	};
	
}


/*======================================================================*
                               printer_task
 *======================================================================*/
void printer_task()
{	
	int i = 0;
	while(1){
		disp_str("hung");
		disp_str("->");
		delay(5);
	}
	
}


