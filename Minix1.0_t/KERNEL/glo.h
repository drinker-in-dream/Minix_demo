
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
EXTERN int mem_proc;		/* previous process */
EXTERN int sig_procs;		/* number of procs with p_pending != 0 */
EXTERN message int_mess;	/* interrupt routines build message here */

EXTERN u32 *page_dir ;		/* previous process */
EXTERN u32 *page_table;		/* previous process */

/* The kernel and task stacks. */
EXTERN struct t_stack {
  int stk[TASK_STACK_BYTES/sizeof(int)];
} t_stack[NR_TASKS];	/* task stacks; task = -1 never really runs */

/* The user stacks. */
//EXTERN struct u_stack {
//  int stk[TASK_STACK_BYTES/sizeof(int)];
//} u_stack[NR_PROCS];

/* The service stacks. */
EXTERN struct s_stack {
  int stk[4096/sizeof(int)];
}s_stack[NR_SERVICES + 3];

PUBLIC void send(int dest,message *m_ptr);
PUBLIC void receive(int src,message *m_ptr);
PUBLIC void sendrec(int srcdest,message *m_ptr);
PUBLIC void send_rec(int srcdest,message *m_ptr);
