/* This file takes care of those system calls that deal with time.
 *
 * The entry points into this file are
 *   do_utime:	perform the UTIME system call
 *   do_time:	perform the TIME system call
 *   do_stime:	perform the STIME system call
 *   do_tims:	perform the TIMES system call
                                                    Toby Du, 2015/3/2
---------------------------------------------------------------------*/

#include "../H/const.h"
#include "../H/type.h"
#include "../H/callnr.h"
#include "../H/com.h"
#include "../H/error.h"
#include "const.h"
#include "type.h"
#include "file.h"
#include "fproc.h"
#include "glo.h"
#include "inode.h"
#include "param.h"

PRIVATE message clock_mess;

/*===========================================================================*
 *				do_utime				     *
 *===========================================================================*/
PUBLIC int do_utime()
{
/* Perform the utime(name, timep) system call. */
	struct inode *rip;
	int r;

	return(r);
}

