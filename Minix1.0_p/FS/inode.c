
/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
                               inode.c
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
                                                    Toby Du, 2015/2/7
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

#include "../h/const.h"
#include "../h/type.h"
#include "../h/error.h"
#include "const.h"
#include "type.h"
#include "buf.h"
#include "glo.h"
#include "inode.h"


/*===========================================================================*
 *				rw_inode				     *
 *===========================================================================*/
PUBLIC void rw_inode(struct inode *rip, int rw_flag)
//struct inode *rip;	/* pointer to inode to be read/written */
//int rw_flag;			/* READING or WRITING */
{
/* An entry in the inode table is to be copied to or from the disk. */
	struct buf *bp;
	d_inode *dip;
	struct super_block *sp;
	block_nr b;
}

/*===========================================================================*
 *				dup_inode				     *
 *===========================================================================*/
PUBLIC void dup_inode(struct inode *ip)
//struct inode *ip;		/* The inode to be duplicated. */
{
/* This routine is a simplified form of get_inode() for the case where
 * the inode pointer is already known.
 */

  ip->i_count++;
}


/*===========================================================================*
 *				get_inode				     *
 *===========================================================================*/
PUBLIC struct inode *get_inode(dev_nr dev, inode_nr numb)
{
/* Find a slot in the inode table, load the specified inode into it, and
 * return a pointer to the slot.  If 'dev' == NO_DEV, just return a free slot.
 */
	struct inode *rip, *xp;
	/* Search the inode table both for (dev, numb) and a free slot. */
    xp = NIL_INODE;
	for (rip = &inode[0]; rip < &inode[NR_INODES]; rip++) {
		if (rip->i_count > 0) { /* only check used slots for (dev, numb) */
			if (rip->i_dev == dev && rip->i_num == numb) {
				/* This is the inode that we are looking for. */
				rip->i_count++;
				return(rip);	/* (dev, numb) found */
			}
		} 
		else
		xp = rip;	/* remember this free slot for later */
	}
	/* Inode we want is not currently in use.  Did we find a free slot? */
	if (xp == NIL_INODE) {	/* inode table completely full */
		//err_code = ENFILE;
		return(NIL_INODE);
	}
	/* A free inode slot has been located.  Load the inode into it. */
	xp->i_dev = dev;
	xp->i_num = numb;
	xp->i_count = 1;
	if (dev != NO_DEV) rw_inode(xp, READING);	/* get inode from disk */

	return(xp);
}


