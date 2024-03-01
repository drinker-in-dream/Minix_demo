#include "../H/const.h"
#include "../H/type.h"
#include "../H/callnr.h"
#include "../H/com.h"
#include "../H/error.h"

#define FS          FS_PROC_NR
#define MM          MMPROCNR

extern int errno;
extern message M;

PUBLIC void sys_copy(message *mptr)
//message *mptr;			/* pointer to message */
{
/* A proc wants to use local copy. */

  /* Make this routine better.  Also check other guys' error handling -DEBUG */
  mptr->m_type = SYS_COPY;
  if (sendrec(SYSTASK, mptr) != OK) panic("sys_copy can't send", NO_NUM);
}

PUBLIC sys_times(int proc, real_time ptr[4])
//int proc;			/* proc whose times are needed */
//real_time ptr[4];			/* pointer to time buffer */
{
/* Fetch the accounting info for a proc. */

  callm1(SYSTASK, SYS_TIMES, proc, 0, 0, ptr, NIL_PTR, NIL_PTR);
  ptr[0] = M.USER_TIME;
  ptr[1] = M.SYSTEM_TIME;
  ptr[2] = M.CHILD_UTIME;
  ptr[3] = M.CHILD_STIME;
}

PUBLIC int tell_fs(int what,int p1,int p2,int p3)
//int what, p1, p2, p3;
{
/* This routine is only used by MM to inform FS of certain events:
 *      tell_fs(CHDIR, slot, dir, 0)
 *      tell_fs(EXIT, proc, 0, 0)
 *      tell_fs(FORK, parent, child, 0)
 *      tell_fs(SETGID, proc, realgid, effgid)
 *      tell_fs(SETUID, proc, realuid, effuid)
 *      tell_fs(SYNC, 0, 0, 0)
 *      tell_fs(UNPAUSE, proc, signr, 0)
 */
  callm1(FS, what, p1, p2, p3, NIL_PTR, NIL_PTR, NIL_PTR);
}

PUBLIC sys_fork(int parent, int child,int pid)
//int parent;			/* proc doing the fork */
//int child;			/* which proc has been created by the fork */
//int pid;			/* process id assigned by MM */
{
/* A proc has forked.  Tell the kernel. */

  callm1(SYSTASK, SYS_FORK, parent, child, pid, NIL_PTR, NIL_PTR, NIL_PTR);
}


PUBLIC void sys_newmap(int proc, char *ptr)
//int proc;			/* proc whose map is to be changed */
//char *ptr;			/* pointer to new map */
{
/* A proc has been assigned a new memory map.  Tell the kernel. */


  callm1(SYSTASK, SYS_NEWMAP, proc, 0, 0, ptr, NIL_PTR, NIL_PTR);
}



