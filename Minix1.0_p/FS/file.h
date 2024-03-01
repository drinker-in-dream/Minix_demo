
/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
                            file.h
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
                                                  Toby Du, 2015/2/12
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

EXTERN struct filp {
  mask_bits filp_mode;		/* RW bits, telling how file is opened */
  int filp_count;		/* how many file descriptors share this slot? */
  struct inode *filp_ino;	/* pointer to the inode */
  file_pos filp_pos;		/* file position */
} filp[NR_FILPS];

#define NIL_FILP (struct filp *) 0	/* indicates absence of a filp slot */

