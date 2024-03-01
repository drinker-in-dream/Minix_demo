/* This file deals with creating processes (via FORK) and deleting them (via
 * EXIT/WAIT).  When a process forks, a new slot in the 'mproc' table is
 * allocated for it, and a copy of the parent's core image is made for the
 * child.  Then the kernel and file system are informed.  A process is removed
 * from the 'mproc' table when two events have occurred: (1) it has exited or
 * been killed by a signal, and (2) the parent has done a WAIT.  If the process
 * exits first, it continues to occupy a slot until the parent does a WAIT.
 *
 * The entry points into this file are:
 *   do_fork:	perform the FORK system call
 *   do_mm_exit:	perform the EXIT system call (by calling mm_exit())
 *   mm_exit:	actually do the exiting
 *   do_wait:	perform the WAIT system call
 */

#include "../H/const.h"
#include "../H/type.h"
#include "../H/callnr.h"
#include "../H/error.h"
#include "const.h"
#include "glo.h"
#include "mproc.h"
#include "param.h"

#define LAST_FEW            2	/* last few slots reserved for superuser */

PRIVATE next_pid = INIT_PROC_NR+1;	/* next pid to be assigned */



/*===========================================================================*
 *				do_fork					     *
 *===========================================================================*/
PUBLIC int do_fork_mm()
{
/* The process pointed to by 'mp' has forked.  Create a child process. */
	return 0;

}

/*===========================================================================*
 *				do_mm_exit				     *
 *===========================================================================*/
PUBLIC int do_mm_exit()
{
/* Perform the exit(status) system call. The real work is done by mm_exit(),
 * which is also called when a process is killed by a signal.
 */

  return(OK);			/* pro forma return code */
}

/*===========================================================================*
 *				mm_exit					     *
 *===========================================================================*/
PUBLIC void mm_exit(struct mproc *rmp, int exit_status)
//struct mproc *rmp;	/* pointer to the process to be terminated */
//int exit_status;		/* the process' exit status (for parent) */
{

}

/*===========================================================================*
 *				do_wait					     *
 *===========================================================================*/
PUBLIC int do_wait()
{
	 return(OK);
}


/*===========================================================================*
 *				cleanup					     *
 *===========================================================================*/
PRIVATE void cleanup(struct mproc *child)
//struct mproc *child;	/* tells which process is exiting */
{

}


