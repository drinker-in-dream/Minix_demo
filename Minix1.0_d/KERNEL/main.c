
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

EXTERN	u32		k_reenter;
EXTERN	DESCRIPTOR	gdt[GDT_SIZE];
EXTERN	char task_stack[STACK_SIZE_TOTAL];
EXTERN struct proc *proc_ptr;	/* &proc[cur_proc] */
void TestA();

/*======================================================================*
                            tinix_main
 *======================================================================*/
PUBLIC int main(){
	disp_str("              in main now ! \n");

	struct s_proc* p_proc	= proc;
	p_proc->ldt_sel	= SELECTOR_LDT_FIRST;
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
	p_proc->regs.eip	= (u32)TestA;
	p_proc->regs.esp	= (u32) task_stack + STACK_SIZE_TOTAL;
	p_proc->regs.eflags	= 0x1202;	// IF=1, IOPL=1, bit 2 is always 1.

	k_reenter	= 0;

	proc_ptr=proc;
	/* Now go to the assembly code to start running the current process. */
	//port_out(INT_CTLMASK, 0);	/* do not mask out any interrupts in 8259A */
	//port_out(INT2_MASK, 0);	/* same for second interrupt controller */



	restart();
	while(1){}
	return 0;
}

/*======================================================================*
                               TestA
 *======================================================================*/
void TestA()
{
	int i = 0;
	while(1){
		disp_str("A");
		disp_int(i++);
		disp_str(".");
		delay(1);
	}
}

