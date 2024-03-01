/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
                            const.h
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
                                                    Toby Du, 2014
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
#ifndef	_CONST_H_
#define	_CONST_H_

#define BLOCK_SIZE      1024	/* # bytes in a disk block */

/* Flag bits for i_mode in the inode. */
#define I_TYPE          0170000	/* this field gives inode type */
#define I_REGULAR       0100000	/* regular file, not dir or special */
#define I_BLOCK_SPECIAL 0060000	/* block special file */
#define I_DIRECTORY     0040000	/* file is a directory */
#define I_CHAR_SPECIAL  0020000	/* character special file */
#define I_SET_UID_BIT   0004000	/* set effective uid on exec */
#define I_SET_GID_BIT   0002000	/* set effective gid on exec */
#define ALL_MODES       0006777	/* all bits for user, group and others */
#define RWX_MODES       0000777	/* mode bits for RWX only */
#define R_BIT           0000004	/* Rwx protection bit */
#define W_BIT           0000002	/* rWx protection bit */
#define X_BIT           0000001	/* rwX protection bit */
#define I_NOT_ALLOC     0000000	/* this inode is free */

/* Miscellaneous */
#define BYTE            0377	/* mask for 8 bits */
#define TO_USER            0	/* flag telling to copy from fs to user */
#define FROM_USER          1	/* flag telling to copy from user to fs */
#define READING            0	/* copy data to user */
#define WRITING            1	/* copy data from user */
#define ABS             -999	/* this process means absolute memory */

#define WORD_SIZE          4		/* number of bytes per word */

#define MAJOR	           8	/* major device = (dev>>MAJOR) & 0377 */
#define MINOR	           0	/* minor device = (dev>>MINOR) & 0377 */

#define YES 1
#define NO 0
#endif /* _CONST_H_ */

