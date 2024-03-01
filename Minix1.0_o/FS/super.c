
/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
                               super.c
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
                                                    Toby Du, 2015/2/24
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/


#include "../H/const.h"
#include "../H/type.h"
#include "../H/error.h"
#include "const.h"
#include "type.h"
#include "buf.h"
#include "inode.h"
#include "super.h"

#define INT_BITS (sizeof(int)<<3)
#define BIT_MAP_SHIFT     13	/* (log2 of BLOCK_SIZE) + 3; 13 for 1k blocks */

/*===========================================================================*
 *				get_super				     *
 *===========================================================================*/
PUBLIC struct super_block *get_super(dev_nr dev)
//dev_nr dev;			/* device number whose super_block is sought */
{
/* Search the superblock table for this device.  It is supposed to be there. */
}
