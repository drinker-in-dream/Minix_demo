
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

//------------->> P R O G R A M    S E T U P <<-------------
EXTERN u8		gdt_ptr[6];	// 0~15:Limit  16~47:Base
EXTERN DESCRIPTOR	gdt[GDT_SIZE];
EXTERN u8		idt_ptr[6];	// 0~15:Limit  16~47:Base
EXTERN GATE		idt[IDT_SIZE];
EXTERN disp_pos;
/*======================================================================*
                            main
 *======================================================================*/
PUBLIC void main(){
/* Start the ball rolling. */

  struct proc *rp;
  int t;
  extern int (*task[])();

  /* Set up proc table entry for user processes.  Be very careful about
   * sp, since the 3 words prior to it will be clobbered when the kernel pushes
   * pc, cs, and psw onto the USER's stack when starting the user the first
   * time.  This means that with initial sp = 0x10, user programs must leave 
   * the words at 0x000A, 0x000C, and 0x000E free.
   */
   _lock();			/* we can't handle interrupts yet */
   disp_pos = 0;
  disp_str("\n\n\n\n\n\n\n\n\n             Hello Minix! \n");
   // copy the GDT in LOADER to new GDT table here
	memcpy(	&gdt,				    // New GDT
		(void*)(*((u32*)(&gdt_ptr[2]))),   // Base  of Old GDT
		*((u16*)(&gdt_ptr[0])) + 1	    // Limit of Old GDT
		);

	// gdt_ptr[6]  6 byte total£º0~15:Limit  16~47:Base
	u16* p_gdt_limit = (u16*)(&gdt_ptr[0]);
	u32* p_gdt_base  = (u32*)(&gdt_ptr[2]);
	*p_gdt_limit = GDT_SIZE * sizeof(DESCRIPTOR) - 1;
	*p_gdt_base  = (u32)&gdt;

	// idt_ptr[6] 6 byte total£º0~15:Limit  16~47:Base.
	u16* p_idt_limit = (u16*)(&idt_ptr[0]);
	u32* p_idt_base  = (u32*)(&idt_ptr[2]);
    *p_idt_limit = IDT_SIZE * sizeof(GATE) - 1;
    *p_idt_base  = (u32)&idt;
	
    prot_init();

    for (rp = &proc[0]; rp <= &proc[NR_TASKS+LOW_USER]; rp++) {
	   //for (t=0; t< NR_REGS; t++) rp->p_reg[t] = 0100*t;	/* debugging */
	   t = rp - proc - NR_TASKS;	/* task number */
	   //----x86 may not use--->>>>>>>>>>
	   //rp->p_sp = (rp < &proc[NR_TASKS] ? t_stack[NR_TASKS+t+1].stk : INIT_SP);//init sp
	   //rp->p_splimit = rp->p_sp;
	   //if (rp->p_splimit != INIT_SP)
		//rp->p_splimit -= (TASK_STACK_BYTES - SAFETY)/sizeof(int);
		//rp->p_pcpsw.pc = task[t + NR_TASKS];
		//rp->p_pcpsw.psw = INIT_PSW;
	   //-----x86 may not use-------<<<<<<<<<
	   //if (rp->p_pcpsw.pc != 0 || t >= 0) 
		   ready(rp); //here to schedue process	   
	   rp->p_flags = 0;

	   /* Set up memory map for tasks and MM, FS, INIT. */
		if (t < 0) {
			/* I/O tasks. */
			//rp->p_map[T].mem_len  = VERY_BIG; 
			//rp->p_map[T].mem_phys = base_click;
			//rp->p_map[D].mem_len  = VERY_BIG; 
			//rp->p_map[D].mem_phys = base_click + sizes[0];
			//rp->p_map[S].mem_len  = VERY_BIG; 
			//rp->p_map[S].mem_phys = base_click + sizes[0] + sizes[1];
			//rp->p_map[S].mem_vir = sizes[0] + sizes[1];
		} else {
			/* MM, FS, and INIT. */
			//previous_base = proc[NR_TASKS + t - 1].p_map[S].mem_phys;
			//rp->p_map[T].mem_len  = sizes[2*t + 2];
			//rp->p_map[T].mem_phys = (t == 0 ? mm_base : previous_base);
			//rp->p_map[D].mem_len  = sizes[2*t + 3];
			//rp->p_map[D].mem_phys = rp->p_map[T].mem_phys + sizes[2*t + 2];
			//rp->p_map[S].mem_vir  = sizes[2*t + 3];
			//rp->p_map[S].mem_phys = rp->p_map[D].mem_phys + sizes[2*t + 3];
		}
	   
		//rp->p_reg[CS_REG] = rp->p_map[T].mem_phys;  //(may not use in x86)
		//rp->p_reg[DS_REG] = rp->p_map[D].mem_phys;  //(may not use in x86)
		//rp->p_reg[SS_REG] = rp->p_map[D].mem_phys;  //(may not use in x86)
		//rp->p_reg[ES_REG] = rp->p_map[D].mem_phys;  //(may not use in x86)
   }
   //proc[NR_TASKS+(HARDWARE)].p_sp = (int *) k_stack;
   //proc[NR_TASKS+(HARDWARE)].p_sp += K_STACK_BYTES/2;
   //proc[NR_TASKS+(HARDWARE)].p_splimit = (int *) k_stack;
   //proc[NR_TASKS+(HARDWARE)].p_splimit += SAFETY/2;
	
	for (rp = proc_addr(LOW_USER+1); rp < proc_addr(NR_PROCS); rp++)
		rp->p_flags = P_SLOT_FREE;
	/* Determine if display is color or monochrome and CPU type (from BIOS). */
	//color = get_chrome();		/* 0 = mono, 1 = color */
	//t = get_byte(CPU_TY1, CPU_TY2);	/* is this PC, XT, AT ... ? */
	//if (t == PC_AT) pc_at = TRUE;

	/* Save the old interrupt vectors. */
	//phys_b = umap(proc_addr(HARDWARE), D, (vir_bytes) vec_table, VECTOR_BYTES);
	//phys_copy(0L, phys_b, (long) VECTOR_BYTES);	/* save all the vectors */
	  
	/* Set up the new interrupt vectors. */
	//for (t = 0; t < 16; t++) set_vec(t, surprise, base_click);
	//for (t = 16; t < 256; t++) set_vec(t, trp, base_click);
	//set_vec(DIVIDE_VECTOR, divide, base_click);
	//set_vec(SYS_VECTOR, s_call, base_click);
	//set_vec(CLOCK_VECTOR, clock_int, base_click);
	//set_vec(KEYBOARD_VECTOR, tty_int, base_click);
	//set_vec(FLOPPY_VECTOR, disk_int, base_click);
	//set_vec(PRINTER_VECTOR, lpr_int, base_click);
	//if (pc_at)
	//  set_vec(AT_WINI_VECTOR, wini_int, base_click);
	//else
	//  set_vec(XT_WINI_VECTOR, wini_int, base_click);

	/* Put a ptr to proc table in a known place so it can be found in /dev/mem */
	//set_vec( (BASE - 4)/4, proc, (phys_clicks) 0);

	//bill_ptr = proc_addr(HARDWARE);	/* it has to point somewhere */
	//pick_proc();

	/* Now go to the assembly code to start running the current process. */
	//port_out(INT_CTLMASK, 0);	/* do not mask out any interrupts in 8259A */
	//port_out(INT2_MASK, 0);	/* same for second interrupt controller */
	//restart();

}






