
/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
                            main.c
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
                                                  Toby Du, 2014/6/30
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
#include "../H/const.h"
#include "../H/type.h"
#include "../H/com.h"
#include "const.h"
#include "type.h"
#include "protect.h"
#include "proc.h"
#include "glo.h"

EXTERN	int		disp_pos;
EXTERN	u32		k_reenter;
EXTERN	DESCRIPTOR	gdt[GDT_SIZE];
EXTERN struct s_proc *proc_ptr;	/* &proc[cur_proc] */
PUBLIC void clock_handler(int irq);
PUBLIC void put_irq_handler(int irq, t_pf_irq_handler handler);

/*======================================================================*
                            main
 *======================================================================*/
PUBLIC int main(){

	struct s_proc* p_proc	= proc;
	int i=0;
	int t;
	int		eflags;
	u8		privilege;
	u8		rpl;
	vir_clicks size;
	phys_clicks base_click, mm_base, previous_base;
	phys_bytes phys_b;
	extern int (*task[])();
	extern unsigned sizes[8];	/* table filled in by build */
	rdy_head[TASK_Q]=NIL_PROC;
	rdy_head[SERVER_Q]=NIL_PROC;
	rdy_head[USER_Q]=NIL_PROC;

	message mes1;
	message mes2;

	mes1.m_type = 6;
	mes2.m_type = 3;

	_lock();			/* we can't handle interrupts yet */
	cls();
	disp_str("                       Hello Minix! \n");

	base_click = sizes[0];
	
	for (p_proc = &proc[0]; p_proc <= &proc[NR_TASKS+LOW_USER]; p_proc++,i++) {
		t = p_proc - proc - NR_TASKS;	/* task number */
		p_proc->p_esp = (p_proc < &proc[NR_TASKS] ? t_stack[NR_TASKS+t+1].stk : s_stack[t+1].stk);
		p_proc->p_esplimit = p_proc->p_esp;
		p_proc->p_pid	= i;			/* pid */

		/*  S E T U P   V A R I A B L E  */
		p_proc->p_callerq	= NIL_PROC;			/* head of list of procs wishing to send */
		p_proc->p_sendlink	= NIL_PROC;			/* link to next proc wishing to send */
		p_proc->p_nextready	= NIL_PROC;			/* pointer to next ready process */

		if (t < 0) {	
			privilege	= PRIVILEGE_TASK;
			rpl		= RPL_TASK;
			eflags		= 0x1202;	/* IF=1, IOPL=1, bit 2 is always 1 */
		}
		else {			
			privilege	= PRIVILEGE_SERVICE;
			rpl		= RPL_SERVICE;
	 	    eflags		= 0x1202;	/* IF=1, IOPL=1, bit 2 is always 1 */
		}

		memcpy(&p_proc->ldts[0], &gdt[SELECTOR_KERNEL_CS >> 3], sizeof(DESCRIPTOR));
		p_proc->ldts[0].attr1 = DA_C | privilege << 5;	/* change the DPL */
		memcpy(&p_proc->ldts[1], &gdt[SELECTOR_KERNEL_DS >> 3], sizeof(DESCRIPTOR));
		p_proc->ldts[1].attr1 = DA_DRW | privilege << 5;/* change the DPL */
		p_proc->regs.cs		= ((8 * 0) & SA_RPL_MASK & SA_TI_MASK) | SA_TIL | rpl;
		p_proc->regs.ds		= ((8 * 1) & SA_RPL_MASK & SA_TI_MASK) | SA_TIL | rpl;
		p_proc->regs.es		= ((8 * 1) & SA_RPL_MASK & SA_TI_MASK) | SA_TIL | rpl;
		p_proc->regs.fs		= ((8 * 1) & SA_RPL_MASK & SA_TI_MASK) | SA_TIL | rpl;
		p_proc->regs.ss		= ((8 * 1) & SA_RPL_MASK & SA_TI_MASK) | SA_TIL | rpl;
		p_proc->regs.gs		= (SELECTOR_KERNEL_GS & SA_RPL_MASK) | rpl;
		p_proc->regs.eip	= (u32)task[i];
		p_proc->regs.esp	= (u32) p_proc->p_esp;
		p_proc->regs.eflags	= eflags;	
		
		//if (p_proc->regs.eip != 0 || t >= 0) ready(p_proc);
		if (p_proc < &proc[NR_TASKS-1]) ready(p_proc);
		
		p_proc->p_flags = 0;
		/* Set up memory map for tasks and MM, FS, INIT. */
	  if (t < 0) {
		/* I/O tasks. */
		p_proc->p_map[T].mem_len  = sizes[1]; 
		p_proc->p_map[T].mem_phys = base_click;
		p_proc->p_map[T].mem_vir = base_click;
		p_proc->p_map[D].mem_len  = sizes[1]; 
		p_proc->p_map[D].mem_phys = base_click;
		p_proc->p_map[D].mem_vir = base_click;
		p_proc->p_map[S].mem_len  = sizes[1]; 
		p_proc->p_map[S].mem_phys = base_click;

	  } else {
		/* MM, FS, and INIT. */
		;
	  }

	}

	for (p_proc = proc_addr(LOW_USER+1); p_proc < proc_addr(NR_PROCS); p_proc++)
	p_proc->p_flags = P_SLOT_FREE;


	k_reenter	= 0;
	busy_map = 0;
	bill_ptr = proc_addr(HARDWARE);	/* it has to point somewhere */
	pick_proc();

	init_clock();
	/* Now go to the assembly code to start running the current process. */
	out_byte(INT_M_CTLMASK,	0);	// Master 8259, OCW1. 
	out_byte(INT_S_CTLMASK,	0);	// Slave  8259, OCW1.
	
	restart();
	while(1){};
	return 0;
}



/*======================================================================*
                               sys_task
 *======================================================================*/
void sys_task()
{	int i = 0;
	while(1){
		disp_str(" sys_task");
		disp_str(" ");
		delay(1);
	}
}



/*======================================================================*
                               mem_task
 *======================================================================*/
void mem_task()
{	
	int i = 0;
	while(1){
		disp_str(" mem_task");
		disp_str(" ");
		delay(1);
	}
	
}

/*======================================================================*
                               floppy_task
 *======================================================================*/
void floppy_task()
{	
	int i = 0;
	while(1){
		disp_str(" floppy_task");
		disp_str(" ");
		delay(1);
	}
	
}

/*======================================================================*
                               winchester_task
 *======================================================================*/
void winchester_task()
{	
	int i = 0;
	while(1){
		disp_str(" winchester_task");
		disp_str(" ");
		delay(1);
	}
	
}


/*======================================================================*
                               tty_task
 *======================================================================*/
void tty_task()
{	
	int i = 0;
	while(1){
		disp_str(" tty_task");
		disp_str(" ");
		delay(1);
	}
	
}


/*======================================================================*
                               printer_task
 *======================================================================*/
void printer_task()
{	
	int i = 0;
	while(1){
		disp_str(" printer_task");
		disp_str(" ");
		delay(1);
	}
	
}


