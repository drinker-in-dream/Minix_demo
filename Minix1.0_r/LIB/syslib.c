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


