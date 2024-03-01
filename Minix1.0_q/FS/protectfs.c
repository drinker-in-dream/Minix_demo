/* This file deals with protection in the file system.  It contains the code
 * for four system calls that relate to protection.
 *
 * The entry points into this file are
 *   do_chmod:	perform the CHMOD system call
 *   do_chown:	perform the CHOWN system call
 *   do_umask:	perform the UMASK system call
 *   do_access:	perform the ACCESS system call
 *   forbidden:	check to see if a given access is allowed on a given inode

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

/*===========================================================================*
 *				do_chmod				     *
 *===========================================================================*/
PUBLIC int do_chmod()
{
/* Perform the chmod(name, mode) system call. */
	
	struct inode *rip;
	int r;
	extern struct inode *eat_path();

	/* Temporarily open the file. */
	if (fetch_name(name, name_length, M3) != OK) return(err_code);
	if ( (rip = eat_path(user_path)) == NIL_INODE) return(err_code);


}

