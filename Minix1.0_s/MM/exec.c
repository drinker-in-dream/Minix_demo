/* This file handles the EXEC system call.  It performs the work as follows:
 *    - see if the permissions allow the file to be executed
 *    - read the header and extract the sizes
 *    - fetch the initial args and environment from the user space
 *    - allocate the memory for the new process
 *    - copy the initial stack from MM to the process
 *    - read in the text and data segments and copy to the process
 *    - take care of setuid and setgid bits
 *    - fix up 'mproc' table
 *    - tell kernel about EXEC
 *
 *   The only entry point is do_exec.
 */

#include "../H/const.h"
#include "../H/type.h"
#include "../H/callnr.h"
#include "../H/error.h"
#include "../H/stat.h"
#include "const.h"
#include "glo.h"
#include "mproc.h"
#include "param.h"

#define MAGIC    0x04000301L	/* magic number with 2 bits masked off */
#define SEP      0x00200000L	/* value for separate I & D */
#define TEXTB              2	/* location of text size in header */
#define DATAB              3	/* location of data size in header */
#define BSSB               4	/* location of bss size in header */
#define TOTB               6	/* location of total size in header */


/*===========================================================================*
 *				patch_ptr				     *
 *===========================================================================*/
PRIVATE void patch_ptr(char stack[MAX_ISTACK_BYTES], vir_bytes base)
//char stack[MAX_ISTACK_BYTES];	/* pointer to stack image within MM */
//vir_bytes base;			/* virtual address of stack base inside user */
{
/* When doing an exec(name, argv, envp) call, the user builds up a stack
 * image with arg and env pointers relative to the start of the stack.  Now
 * these pointers must be relocated, since the stack is not positioned at
 * address 0 in the user's address space.
 */

}

/*===========================================================================*
 *				do_exec					     *
 *===========================================================================*/
PUBLIC int do_exec()
{
/* Perform the exece(name, argv, envp) call.  The user library builds a
 * complete stack image, including pointers, args, environ, etc.  The stack
 * is copied to a buffer inside MM, and then to the new core image.
 */
}

/*===========================================================================*
 *				read_header				     *
 *===========================================================================*/
PRIVATE int read_header(int fd, int *ft, vir_bytes *text_bytes, vir_bytes *data_bytes, vir_bytes *bss_bytes, phys_bytes *tot_bytes,vir_clicks sc)
//int fd;				/* file descriptor for reading exec file */
//int *ft;			/* place to return ft number */
//vir_bytes *text_bytes;		/* place to return text size */
//vir_bytes *data_bytes;		/* place to return initialized data size */
//vir_bytes *bss_bytes;		/* place to return bss size */
//phys_bytes *tot_bytes;		/* place to return total size */
//vir_clicks sc;			/* stack size in clicks */
{
/* Read the header and extract the text, data, bss and total sizes from it. */

	return(OK);
}

/*===========================================================================*
 *				load_seg				     *
 *===========================================================================*/
PRIVATE void load_seg(int fd, int seg, vir_bytes seg_bytes)
//int fd;				/* file descriptor to read from */
//int seg;			/* T or D */
//vir_bytes seg_bytes;		/* how big is the segment */
{
	
}




