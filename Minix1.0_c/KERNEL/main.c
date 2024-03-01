
/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
                            main.c
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
                                                  Toby Du, 2014/6/30
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
#include "../H/const.h"
#include "../H/type.h"
#include "const.h"
#include "type.h"
#include "glo.h"
#include "proc.h"
#include "protect.h"

//------------->> P R O G R A M    S E T U P <<-------------
EXTERN u8		gdt_ptr[6];	// 0~15:Limit  16~47:Base
EXTERN DESCRIPTOR	gdt[GDT_SIZE];
EXTERN u8		idt_ptr[6];	// 0~15:Limit  16~47:Base
EXTERN GATE		idt[IDT_SIZE];
EXTERN disp_pos;
EXTERN	char	task_stack[0x8000];
EXTERN	u32		k_reenter;
/*======================================================================*
                            main
 *======================================================================*/
PUBLIC void main(){
/* Start the ball rolling. */

  struct proc *rp;
  int t;
  extern int (*task[])();
  u16		selector_ldt	= SELECTOR_LDT_FIRST;
  int i=0;
  u8 privilege = PRIVILEGE_TASK;
  u8 rpl = RPL_TASK;
  int	eflags = 0x1202;

  /* Set up proc table entry for user processes.  Be very careful about
   * sp, since the 3 words prior to it will be clobbered when the kernel pushes
   * pc, cs, and psw onto the USER's stack when starting the user the first
   * time.  This means that with initial sp = 0x10, user programs must leave 
   * the words at 0x000A, 0x000C, and 0x000E free.
   */
  _lock();			/* we can't handle interrupts yet */
  disp_pos = 0;
  disp_str("\n\n\n\n\n\n\n\n\n             Hello Minix! \n");
	
    for (rp = &proc[0]; rp <= &proc[NR_TASKS+LOW_USER]; rp++,i++) {
	   rp->p_pid	= i;			/* pid */
	   rp->ldt_sel	= selector_ldt;
	   memcpy(&rp->p_ldt[0], &gdt[SELECTOR_KERNEL_CS >> 3], sizeof(DESCRIPTOR));
	   rp->p_ldt[0].attr1 = DA_C | privilege << 5;	/* change the DPL */
	   memcpy(&rp->p_ldt[1], &gdt[SELECTOR_KERNEL_DS >> 3], sizeof(DESCRIPTOR));
	   rp->p_ldt[1].attr1 = DA_DRW | privilege << 5;/* change the DPL */
	   memcpy(&rp->p_ldt[2], &gdt[SELECTOR_KERNEL_DS >> 3], sizeof(DESCRIPTOR));
	   rp->p_ldt[2].attr1 = DA_DRW | privilege << 5;/* change the DPL */
	   memcpy(&rp->p_ldt[3], &gdt[SELECTOR_KERNEL_DS >> 3], sizeof(DESCRIPTOR));
	   rp->p_ldt[3].attr1 = DA_DRW | privilege << 5;/* change the DPL */
	   rp->p_reg.cs		= ((8 * 0) & SA_RPL_MASK & SA_TI_MASK) | SA_TIL | rpl;
	   rp->p_reg.ds		= ((8 * 1) & SA_RPL_MASK & SA_TI_MASK) | SA_TIL | rpl;
	   rp->p_reg.es		= ((8 * 2) & SA_RPL_MASK & SA_TI_MASK) | SA_TIL | rpl;
	   rp->p_reg.fs		= ((8 * 3) & SA_RPL_MASK & SA_TI_MASK) | SA_TIL | rpl;
	   rp->p_reg.ss		= ((8 * 1) & SA_RPL_MASK & SA_TI_MASK) | SA_TIL | rpl;
	   rp->p_reg.gs		= (SELECTOR_KERNEL_GS & SA_RPL_MASK) | rpl;
	   rp->p_reg.eip	= (u32)task[i];
	   rp->p_reg.esp	= (u32) task_stack + 0x8000;
	   rp->p_reg.eflags	= eflags;

	   selector_ldt += 1 << 3;
	
   }


	k_reenter	= 0;
	/* Now go to the assembly code to start running the current process. */
	//port_out(INT_CTLMASK, 0);	/* do not mask out any interrupts in 8259A */
	//port_out(INT2_MASK, 0);	/* same for second interrupt controller */

	restart();


	while(1){}
}

//tast for test

/*======================================================================*
                               itoa
 *======================================================================*/
PUBLIC char * itoa(char * str, int num)
{
	char *	p = str;
	char	ch;
	int	i;
	int	flag = FALSE;

	*p++ = '0';
	*p++ = 'x';

	if(num == 0){
		*p++ = '0';
	}
	else{	
		for(i=28;i>=0;i-=4){
			ch = (num >> i) & 0xF;
			if(flag || (ch > 0)){
				flag = TRUE;
				ch += '0';
				if(ch > '9'){
					ch += 7;
				}
				*p++ = ch;
			}
		}
	}

	*p = 0;

	return str;
}


/*======================================================================*
                               disp_int
 *======================================================================*/
PUBLIC void disp_int(int input)
{
	char output[16];
	itoa(output, input);
	disp_str(output);
}

/*======================================================================*
                               delay
 *======================================================================*/
PUBLIC void delay(int time)
{
	int i, j, k;
	for(k=0;k<time;k++){
		/*for(i=0;i<10000;i++){	for Virtual PC	*/
		for(i=0;i<10;i++){/*	for Bochs	*/
			for(j=0;j<10000;j++){}
		}
	}
}

/*======================================================================*
                               sys_task
 *======================================================================*/
void sys_task()
{	int i = 0;
	while(1){
		disp_str("sys_task");
		disp_int(i++);
		disp_str(".");
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
		disp_str("clock_task");
		disp_int(i++);
		disp_str(".");
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
		disp_str("mem_task");
		disp_int(i++);
		disp_str(".");
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
		disp_str("floppy_task");
		disp_int(i++);
		disp_str(".");
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
		disp_str("winchester_task");
		disp_int(i++);
		disp_str(".");
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
		disp_str("tty_task");
		disp_int(i++);
		disp_str(".");
		delay(1);
	};
	
}


/*======================================================================*
                               printer_task
 *======================================================================*/
void printer_task()
{	
	int i = 0;
	while(1){
		disp_str("printer_task");
		disp_int(i++);
		disp_str(".");
		delay(1);
	}
	
}




