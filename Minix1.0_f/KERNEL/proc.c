
/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
                            table.c
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
                                                    Toby Du, 2014/7/14
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

#include "../H/const.h"
#include "../H/type.h"
#include "../H/com.h"
#include "const.h"
#include "type.h"
#include "protect.h"
#include "proc.h"
#include "glo.h"


/*===========================================================================*
 *				pick_proc				     * 
 *===========================================================================*/
PUBLIC void pick_proc(){
	/* Decide who to run now. */
	int q;		/* which queue to use */
	if (rdy_head[TASK_Q] != NIL_PROC) q = TASK_Q;
	else if (rdy_head[SERVER_Q] != NIL_PROC) q = SERVER_Q;
	else q = USER_Q;

	/* Set 'cur_proc' and 'proc_ptr'. If system is idle, set 'cur_proc' to a
	 * special value (IDLE), and set 'proc_ptr' to point to an unused proc table
	 * slot, namely, that of task -1 (HARDWARE), so save() will have somewhere to
	 * deposit the registers when a interrupt occurs on an idle machine.
	 * Record previous process so that when clock tick happens, the clock task
	 * can find out who was running just before it began to run.  (While the
	 * clock task is running, 'cur_proc' = CLOCKTASK. In addition, set 'bill_ptr'
	 * to always point to the process to be billed for CPU time.
	 */
	 prev_proc = cur_proc;
	if (rdy_head[q] != NIL_PROC) {
		/* Someone is runnable. */
		cur_proc = rdy_head[q] - proc - NR_TASKS;
		proc_ptr = rdy_head[q];
		if (cur_proc >= LOW_USER) bill_ptr = proc_ptr;
	} else {
		/* No one is runnable. */
		cur_proc = IDLE;
		proc_ptr = proc_addr(HARDWARE);
		bill_ptr = proc_ptr;
  }
}

/*===========================================================================*
 *				           ready					                         * 
 *===========================================================================*/	
PUBLIC void ready(struct s_proc *rp){
	/* this process is now runnable */
/* Add 'rp' to the end of one of the queues of runnable processes. Three
 * queues are maintained:
 *   TASK_Q   - (highest priority) for runnable tasks
 *   SERVER_Q - (middle priority) for MM and FS only
 *   USER_Q   - (lowest priority) for user processes
 */

  int q;		/* TASK_Q, SERVER_Q, or USER_Q */
  int r;

  _lock();			/* disable interrupts */
  r = (rp - proc) - NR_TASKS;	/* task or proc number */
  q = (r < 0 ? TASK_Q : r < LOW_USER ? SERVER_Q : USER_Q);

  /* See if the relevant queue is empty. */
  if (rdy_head[q] == NIL_PROC)
	rdy_head[q] = rp;	/* add to empty queue */
  else
	rdy_tail[q]->p_nextready = rp;	/* add to tail of nonempty queue */
   rdy_tail[q] = rp;		/* new entry has no successor */
  rp->p_nextready = NIL_PROC;
  _restore();			/* restore interrupts to previous state */
}










