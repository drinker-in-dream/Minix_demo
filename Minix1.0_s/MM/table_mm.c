/* This file contains the table used to map system call numbers onto the
 * routines that perform them.
 */

#include "../h/const.h"
#include "../h/type.h"
#include "const.h"

#undef EXTERN
#define EXTERN

#include "../h/callnr.h"
#include "glo.h"
#include "mproc.h"
#include "param.h"

/* Miscellaneous */
char core_name[] = {"core"};	/* file name where core images are produced */
unshort core_bits = 0x0EFC;	/* which signals cause core images */

extern char mm_stack[];
char *stackpt = &mm_stack[MM_STACK_BYTES];	/* initial stack pointer */


