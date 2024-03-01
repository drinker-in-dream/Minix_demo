
/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
                               table.c
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
                                                    Toby Du, 2015/2/8
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

#include "../h/const.h"
#include "../h/type.h"
//#include "../h/stat.h"
#include "const.h"
#include "type.h"
#include "dev.h"
#include "../h/callnr.h"
#include "../h/com.h"
#include "../h/error.h"

#undef EXTERN
#define EXTERN


#include "buf.h"
#include "file.h"
#include "fproc.h"
#include "glo.h"
#include "inode.h"
//#include "super.h"

extern  no_call();

extern rw_dev(), rw_dev2();

/* The order of the entries here determines the mapping between major device
 * numbers and tasks.  The first entry (major device 0) is not used.  The
 * next entry is major device 1, etc.  Character and block devices can be
 * intermixed at random.  If this ordering is changed, BOOT_DEV and ROOT_DEV
 * must be changed to correspond to the new values.
 */
struct dmap dmap[] = {
/*  Open       Read/Write   Close       Task #      Device  File
    ----       ----------   -----       -------     ------  ----      */
    0,         0,           0,          0,           /* 0 = not used  */
    no_call,   rw_dev,      no_call,    MEM,         /* 1 = /dev/mem  */
    no_call,   rw_dev,      no_call,    FLOPPY,      /* 2 = /dev/fd0  */
    no_call,   rw_dev,      no_call,    WINCHESTER,  /* 3 = /dev/hd0  */
    no_call,   rw_dev,      no_call,    TTY,         /* 4 = /dev/tty0 */
    no_call,   rw_dev2,     no_call,    TTY,         /* 5 = /dev/tty  */
    no_call,   rw_dev,      no_call,    PRINTER      /* 6 = /dev/lp   */
};

int max_major = sizeof(dmap)/sizeof(struct dmap);

