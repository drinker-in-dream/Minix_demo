
/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
                               pipe.c
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
                                                    Toby Du, 2015/2/10
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

#include "../H/const.h"
#include "../H/type.h"
#include "../H/callnr.h"
#include "../H/com.h"
#include "../H/error.h"
#include "../H/signal.h"
#include "const.h"
#include "type.h"
#include "file.h"
#include "fproc.h"
#include "glo.h"
#include "inode.h"
#include "param.h"

PRIVATE message mess;

/*===========================================================================*
 *				revive					     *
 *===========================================================================*/
PUBLIC void revive(int proc_nr, int bytes)
//int proc_nr;			/* process to revive */
//int bytes;			/* if hanging on task, how many bytes read */
{
/* Revive a previously blocked process. When a process hangs on tty, this
 * is the way it is eventually released.
 */
	struct fproc *rfp;
	if (proc_nr < 0 || proc_nr >= NR_PROCS) printk("revive err", proc_nr);
	rfp = &fproc[proc_nr];
	if (rfp->fp_suspended == NOT_SUSPENDED) return;
	/* The 'reviving' flag only applies to pipes.  Processes waiting for TTY get
     * a message right away.  The revival process is different for TTY and pipes.
     * For TTY revival, the work is already done, for pipes it is not: the proc
     * must be restarted so it can try again.
     */
	if (rfp->fp_task == XPIPE) {
		/* Revive a process suspended on a pipe. */
		rfp->fp_revived = REVIVING;
		reviving++;		/* process was waiting on pipe */
	} else {
		/* Revive a process suspended on TTY or other device. */
		rfp->fp_suspended = NOT_SUSPENDED;
		rfp->fp_nbytes = bytes;	/* pretend it only wants what there is */
		reply(proc_nr, bytes);	/* unblock the process */
	}

}


