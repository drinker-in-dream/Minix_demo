
/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
                               read.c
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
                                                    Toby Du, 2015/2/24
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

#include "../H/const.h"
#include "../H/type.h"
#include "../H/com.h"
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

#define FD_MASK          077	/* max file descriptor is 63 */

PRIVATE message umess;		/* message for asking SYSTASK for user copy */
PUBLIC int rdwt_err;		/* set to EIO if disk error occurs */



