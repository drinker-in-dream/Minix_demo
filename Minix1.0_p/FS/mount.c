/* This file performs the MOUNT and UMOUNT system calls.
 *
 * The entry points into this file are
 *   do_mount:	perform the MOUNT system call
 *   do_umount:	perform the UMOUNT system call
 
                                                     Toby Du, 2015/3/2
 ---------------------------------------------------------------------*/

#include "../H/const.h"
#include "../H/type.h"
#include "../H/error.h"
#include "const.h"
#include "type.h"
#include "buf.h"
#include "file.h"
#include "fproc.h"
#include "glo.h"
#include "inode.h"
#include "param.h"
#include "super.h"

