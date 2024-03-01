
/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
                            table.c
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
                                                    Toby Du, 2014/7/2
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
#include "../H/const.h"
#include "../H/type.h"
#include "const.h"
#include "type.h"
#include "glo.h"
#include "proc.h"




/*===========================================================================*
 *				           ready					                         * 
 *===========================================================================*/	
PUBLIC ready(struct proc *rp){
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







