/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
                            type.h
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
                                                    Toby Du, 2014
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
#ifndef	_TYPE_H_
#define	_TYPE_H_
/* Data type */
#define WORLDSIZE_32
typedef	int			t_bool;
#ifndef WORLDSIZE_32
typedef	unsigned long long	u64;
typedef	unsigned int		u32;
typedef	unsigned short		u16;
typedef	unsigned char		u8;
typedef long long	        s64;
typedef	int		            s32;
typedef	short		        s16;
typedef	char		        s8;
#else
typedef	unsigned long long	u64;
typedef	unsigned int		u32;
typedef	unsigned short		u16;
typedef	unsigned char		u8;
typedef long long	        s64;
typedef	int		            s32;
typedef	short		        s16;
typedef	char		        s8;
#endif /* WORLDSIZE_32 */

/* Macros */
#define MAX(a,b)	(a > b ? a : b)
#define MIN(a,b)	(a < b ? a : b)

/* Type definitions */
typedef unsigned short unshort;	/* must be 16-bit unsigned */
typedef unshort block_nr;	/* block number */
#define NO_BLOCK (block_nr) 0	/* indicates the absence of a block number */
#define MAX_BLOCK_NR (block_nr) 0177777

typedef unshort inode_nr;	/* inode number */
#define NO_ENTRY (inode_nr) 0	/* indicates the absence of a dir entry */
#define MAX_INODE_NR (inode_nr) 0177777

typedef unshort zone_nr;	/* zone number */
#define NO_ZONE   (zone_nr) 0	/* indicates the absence of a zone number */
#define HIGHEST_ZONE (zone_nr)  0177777

typedef unshort mask_bits;	/* mode bits */

typedef unshort bit_nr;		/* if inode_nr & zone_nr both unshort,
				   then also unshort, else long */

typedef unshort mask_bits;	/* mode bits */
typedef unshort dev_nr;		/* major | minor device number */
#define NO_DEV    (dev_nr) ~0	/* indicates absence of a device number */

typedef char links;		/* number of links to an inode */
#define MAX_LINKS 	0177

/* File property*/
typedef long real_time;		/* real time in seconds since Jan 1, 1980 */
typedef long file_pos;		/* position in, or length of, a file */
#define MAX_FILE_POS 017777777777L
typedef short int uid;		/* user id */
typedef char gid;		/* group id */

#endif /* _TYPE_H_ */