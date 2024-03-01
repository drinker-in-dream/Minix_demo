
/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
                            glo.h
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
                                                    Toby Du, 2014/7/11
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

/* Clocks and timers */
EXTERN real_time realtime;	/* real time clock */
EXTERN int lost_ticks;		/* incremented when clock int can't send mess*/

/* Processes, signals, and messages. */
EXTERN int cur_proc;		/* current process */
EXTERN int prev_proc;		/* previous process */
EXTERN int sig_procs;		/* number of procs with p_pending != 0 */
//EXTERN message int_mess;	/* interrupt routines build message here */

/* CPU type. */
//EXTERN int olivetti;		/* TRUE for Olivetti-style keyboard */ (not use in 386)
//EXTERN int pc_at;		/*  PC-AT type diskette drives (360K/1.2M) ? */ (not use in 386)

/* The kernel and task stacks. */ 
//EXTERN struct t_stack { (no need in 386 protect mode )
//  int stk[TASK_STACK_BYTES/sizeof(int)];
//} t_stack[NR_TASKS - 1];	/* task stacks; task = -1 never really runs */

//EXTERN char k_stack[K_STACK_BYTES];	/* The kernel stack. */



