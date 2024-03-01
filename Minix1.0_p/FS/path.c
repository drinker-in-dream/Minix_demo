/* This file contains the procedures that look up path names in the directory
 * system and determine the inode number that goes with a given path name.
 *
 *  The entry points into this file are
 *   eat_path:	 the 'main' routine of the path-to-inode conversion mechanism
 *   last_dir:	 find the final directory on a given path
 *   advance:	 parse one component of a path name
 *   search_dir: search a directory for a string and return its inode number

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
#include "super.h"


/*===========================================================================*
 *				search_dir				     *
 *===========================================================================*/
PUBLIC int search_dir(struct inode *ldir_ptr, char string[NAME_SIZE], inode_nr *numb, int flag)
//struct inode *ldir_ptr;	/* ptr to inode for dir to search */
//char string[NAME_SIZE];		/* component to search for */
//inode_nr *numb;			/* pointer to inode number */
//int flag;			/* LOOK_UP, ENTER, or DELETE */
{
/* This function searches the directory whose inode is pointed to by 'ldip':
 * if (flag == LOOK_UP) search for 'string' and return inode # in 'numb';
 * if (flag == ENTER)  enter 'string' in the directory with inode # '*numb';
 * if (flag == DELETE) delete 'string' from the directory;
 */
	dir_struct *dp;
	struct buf *bp;
	int r;
	mask_bits bits;
	file_pos pos;
	unsigned new_slots, old_slots;
	block_nr b;
	int e_hit;
	extern struct buf *get_block(), *new_block();
	extern block_nr read_map();
	extern real_time clock_time();

	/* If 'ldir_ptr' is not a pointer to a searchable dir inode, error. */
	if ( (ldir_ptr->i_mode & I_TYPE) != I_DIRECTORY) return(ENOTDIR);
	bits = (flag == LOOK_UP ? X_BIT : W_BIT|X_BIT);
	//if ( (r = forbidden(ldir_ptr, bits, 0)) != OK)
		return(r);

}

/*===========================================================================*
 *				advance					     *
 *===========================================================================*/
PUBLIC struct inode *advance(struct inode *dirp, char string[NAME_SIZE])
//struct inode *dirp;		/* inode for directory to be searched */
//char string[NAME_SIZE];		/* component name to look for */
{
/* Given a directory and a component of a path, look up the component in
 * the directory, find the inode, open it, and return a pointer to its inode
 * slot.  If it can't be done, return NIL_INODE.
 */
	struct inode *rip;
	struct inode *rip2;
	struct super_block *sp;
	int r;
	dev_nr mnt_dev;
	inode_nr numb;
	extern struct inode *get_inode();

	/* If 'string' is empty, yield same inode straight away. */
	if (string[0] == '\0') return(get_inode(dirp->i_dev, dirp->i_num));

	/* If 'string' is not present in the directory, signal error. */
	if ( (r = search_dir(dirp, string, &numb, LOOK_UP)) != OK) {
		err_code = r;
		return(NIL_INODE);
	}

	/* The component has been found in the directory.  Get inode. */
	if ( (rip = get_inode(dirp->i_dev, numb)) == NIL_INODE) return(NIL_INODE);
/*
	if (rip->i_num == ROOT_INODE)
		if (dirp->i_num == ROOT_INODE) {
			if (string[1] == '.') {
				for (sp = &super_block[1]; sp < &super_block[NR_SUPERS]; sp++) {
					if (sp->s_dev == rip->i_dev) {
						//Release the root inode.  Replace by the
						//inode mounted on.
						 
						put_inode(rip);
						mnt_dev = sp->s_imount->i_dev;
						rip2 = get_inode(mnt_dev, sp->s_imount->i_num);
						rip = advance(rip2, string);
						put_inode(rip2);
						break;
					}
				}
			}
		}
  
*/
}

/*===========================================================================*
 *				last_dir				     *
 *===========================================================================*/
PUBLIC struct inode *last_dir(char *path, char string[NAME_SIZE])
//char *path;			/* the path name to be parsed */
//char string[NAME_SIZE];		/* the final component is returned here */
{
/* Given a path, 'path', located in the fs address space, parse it as
 * far as the last directory, fetch the inode for the last directory into
 * the inode table, and return a pointer to the inode.  In
 * addition, return the final component of the path in 'string'.
 * If the last directory can't be opened, return NIL_INODE and
 * the reason for failure in 'err_code'.
 */
	
}

/*===========================================================================*
 *				eat_path				     *
 *===========================================================================*/
PUBLIC struct inode *eat_path(char *path)
//char *path;			/* the path name to be parsed */
{
/* Parse the path 'path' and put its inode in the inode table.  If not
 * possible, return NIL_INODE as function value and an error code in 'err_code'.
 */

}
