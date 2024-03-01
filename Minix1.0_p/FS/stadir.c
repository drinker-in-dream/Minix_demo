/* This file contains the code for performing four system calls relating to
 * status and directories.
 *
 * The entry points into this file are
 *   do_chdir:	perform the CHDIR system call
 *   do_chroot:	perform the CHROOT system call
 *   do_stat:	perform the STAT system call
 *   do_fstat:	perform the FSTAT system call
                                                    Toby Du, 2015/3/6
 ---------------------------------------------------------------------*/

#include "../H/const.h"
#include "../H/type.h"
#include "../H/error.h"
#include "../H/stat.h"
#include "const.h"
#include "type.h"
#include "file.h"
#include "fproc.h"
#include "glo.h"
#include "inode.h"
#include "param.h"


