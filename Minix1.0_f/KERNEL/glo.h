
/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
                            glo.h
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
                                                    Toby Du, 2014/7/11
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

/* Processes, signals, and messages. */
EXTERN int cur_proc;		/* current process */
EXTERN int prev_proc;		/* previous process */
EXTERN int sig_procs;		/* number of procs with p_pending != 0 */

/* The kernel and task stacks. */
EXTERN struct t_stack {
  int stk[TASK_STACK_BYTES/sizeof(int)];
} t_stack[NR_TASKS];	/* task stacks; task = -1 never really runs */

/* The user stacks. */
EXTERN struct u_stack {
  int stk[TASK_STACK_BYTES/sizeof(int)];
} u_stack[NR_PROCS];

/* The user stacks. */
EXTERN struct s_stack {
  int stk[TASK_STACK_BYTES/sizeof(int)];
}s_stack[NR_SERVICES];


