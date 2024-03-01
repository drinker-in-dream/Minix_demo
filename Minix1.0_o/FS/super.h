
/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
                               super.h
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
                                                    Toby Du, 2015/2/9
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

EXTERN struct super_block {
  inode_nr s_ninodes;		/* # usable inodes on the minor device */
  zone_nr s_nzones;		/* total device size, including bit maps etc */
  unshort s_imap_blocks;	/* # of blocks used by inode bit map */
  unshort s_zmap_blocks;	/* # of blocks used by zone bit map */
  zone_nr s_firstdatazone;	/* number of first data zone */
  short int s_log_zone_size;	/* log2 of blocks/zone */
  file_pos s_max_size;		/* maximum file size on this device */
  int s_magic;			/* magic number to recognize super-blocks */

  /* The following items are only used when the super_block is in memory. */
  struct buf *s_imap[I_MAP_SLOTS]; /* pointers to the in-core inode bit map */
  struct buf *s_zmap[ZMAP_SLOTS]; /* pointers to the in-core zone bit map */
  dev_nr s_dev;			/* whose super block is this? */
  struct inode *s_isup;		/* inode for root dir of mounted file sys */
  struct inode *s_imount;	/* inode mounted on */
  real_time s_time;		/* time of last update */
  char s_rd_only;		/* set to 1 iff file sys mounted read only */
  char s_dirt;			/* CLEAN or DIRTY */
} super_block[NR_SUPERS];

#define NIL_SUPER (struct super_block *) 0

