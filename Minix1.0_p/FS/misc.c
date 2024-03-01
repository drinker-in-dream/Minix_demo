/* This file contains a collection of miscellaneous procedures.  Some of them
 * perform simple system calls.  Some others do a little part of system calls
 * that are mostly performed by the Memory Manager.
 *
 * The entry points into this file are
 *   do_dup:	perform the DUP system call
 *   do_sync:	perform the SYNC system call
 *   do_fork:	adjust the tables after MM has performed a FORK system call
 *   do_exit:	a process has exited; note that in the tables
 *   do_set:	set uid or gid for some process
 *   do_revive:	revive a process that was waiting for something (e.g. TTY)

                                                    Toby Du, 2015/3/2
 ---------------------------------------------------------------------*/

#include "../H/const.h"
#include "../H/type.h"
#include "../H/callnr.h"
#include "../H/com.h"
#include "../H/error.h"
#include "const.h"
#include "type.h"
#include "buf.h"
#include "dev.h"
#include "file.h"
#include "fproc.h"
#include "glo.h"
#include "inode.h"
#include "param.h"
#include "super.h"

