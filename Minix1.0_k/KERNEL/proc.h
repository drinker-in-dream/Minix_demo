
/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
                               proc.h
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
                                                    Toby Du, 2014/7/12
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/


typedef struct s_stackframe {
	u32	gs;		
	u32	fs;		
	u32	es;		
	u32	ds;		
	u32	edi;	
	u32	esi;	
	u32	ebp;	
	u32	kernel_esp;	
	u32	ebx;			
	u32	edx;	
	u32	ecx;		
	u32	eax;		
	u32	retaddr;
	u32	eip;	
	u32	cs;			
	u32	eflags;		
	u32	esp;	
	u32	ss;	
}STACK_FRAME;


EXTERN struct s_proc {
	STACK_FRAME			regs;

	u16				ldt_sel;
	DESCRIPTOR			ldts[LDT_SIZE];
	int				 p_pid;
	int             *p_esp;			/* stack pointer */
	int             *p_esplimit;		/* lowest legal stack value */
	int              p_flags;			/* P_SLOT_FREE, SENDING, RECEIVING, etc. */
	char			 p_name[16];
	struct mem_map p_map[NR_SEGS];/* memory map */
	real_time user_time;		/* user time in ticks */
	real_time sys_time;		/* sys time in ticks */
	real_time child_utime;	/* cumulative user time of children */
	real_time child_stime;	/* cumulative sys time of children */
	real_time p_alarm;		/* time of next alarm in ticks, or 0 */
	struct s_proc *p_callerq;	/* head of list of procs wishing to send */
	struct s_proc *p_sendlink;	/* link to next proc wishing to send */
	message *p_messbuf;		/* pointer to message buffer */
	int p_getfrom;		/* from whom does process want to receive? */
	struct s_proc   *p_nextready;	/* pointer to next ready process */
    int p_pending;		/* bit map for pending signals 1-16 */
}proc[NR_TASKS+NR_PROCS];


/* Bits for p_flags in proc[].  A process is runnable iff p_flags == 0 */
#define P_SLOT_FREE      001	/* set when slot is not in use */
#define NO_MAP           002	/* keeps unmapped forked child from running */
#define SENDING          004	/* set when process blocked trying to send */
#define RECEIVING        010	/* set when process blocked trying to recv */

#define proc_addr(n) &proc[NR_TASKS + n]
#define NIL_PROC (struct s_proc *) 0

EXTERN struct s_proc *proc_ptr;	/* &proc[cur_proc] */
EXTERN struct s_proc *bill_ptr;	/* ptr to process to bill for clock ticks */
EXTERN struct s_proc *rdy_head[NQ];	/* pointers to ready list headers */
EXTERN struct s_proc *rdy_tail[NQ];	/* pointers to ready list tails */

EXTERN unsigned busy_map;		/* bit map of busy tasks */
EXTERN message *task_mess[NR_TASKS+1];	/* ptrs to messages for busy tasks */





