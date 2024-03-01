/* This file contains the procedures for creating, opening, closing, and
 * seeking on files.
 *
 * The entry points into this file are
 *   do_creat:	perform the CREAT system call
 *   do_mknod:	perform the MKNOD system call
 *   do_open:	perform the OPEN system call
 *   do_close:	perform the CLOSE system call
 *   do_lseek:  perform the LSEEK system call
                                                     Toby Du, 2015/3/6
 ---------------------------------------------------------------------*/

#include "../H/const.h"
#include "../H/type.h"
#include "../H/callnr.h"
#include "../H/error.h"
#include "const.h"
#include "type.h"
#include "buf.h"
#include "file.h"
#include "fproc.h"
#include "glo.h"
#include "inode.h"
#include "param.h"

