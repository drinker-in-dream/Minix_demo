/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
                            fsconst.h
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
                                                    Toby Du, 2014
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
#ifndef	_FSCONST_H_
#define	_FSCONST_H_

#include "type.h"
#include "const.h"


/* Miscellaneous constants */
#define SUPER_MAGIC   0x137F	/* magic number contained in super-block */

/* Tables sizes */
#define NR_ZONE_NUMS       9	/* # zone numbers in an inode */
#define NAME_SIZE         14	/* # bytes in a directory component */

#define BOOT_BLOCK (block_nr) 0	/* block number of boot block */
#define SUPER_BLOCK (block_nr)1	/* block number of super block */
#define ROOT_INODE (inode_nr) 1	/* inode number for root directory */

/* Derived sizes */
#define ZONE_NUM_SIZE    sizeof(zone_nr)		/* # bytes in zone nr*/
#define INODE_SIZE       (sizeof (d_inode))		/* bytes in disk inode*/
#define DIR_ENTRY_SIZE   sizeof(dir_struct)		/* # bytes/dir entry */
#define INODES_PER_BLOCK (BLOCK_SIZE/INODE_SIZE)	/* # inodes/disk blk */
#define NR_INDIRECTS     (BLOCK_SIZE/ZONE_NUM_SIZE)	/* # zones/indir blk */
#define INTS_PER_BLOCK   (BLOCK_SIZE/sizeof(int))	/* # integers/blk */
#define NR_DIR_ENTRIES   (BLOCK_SIZE/DIR_ENTRY_SIZE)	/* # dir entries/blk*/
#define NR_DZONE_NUM     (NR_ZONE_NUMS-2)		/* # zones in inode */
#define MAX_ZONES (NR_DZONE_NUM+NR_INDIRECTS+(long)NR_INDIRECTS*NR_INDIRECTS)
						/* max # of zones in a file */

#endif /* _FSCONST_H_ */

