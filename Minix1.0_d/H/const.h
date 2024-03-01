
/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
                            const.h
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
                                                    Toby Du, 2013
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
#define EXTERN        extern	/* used in *.h files */
#define PRIVATE       static	/* PRIVATE x limits the scope of x */
#define PUBLIC			/* PUBLIC is the opposite of PRIVATE */

#define TRUE               1	/* used for turning integers into Booleans */
#define FALSE              0	/* used for turning integers into Booleans */


#define NR_TASKS           8	/* number of tasks in the transfer vector */
#define NR_PROCS          16	/* number of slots in proc table */
#define NR_SEGS            3	/* # segments per process */

/* Memory is allocated in clicks. */

/* Process numbers of some important processes */
#define MM_PROC_NR         0	/* process number of memory manager */
#define FS_PROC_NR         1	/* process number of file system */
#define INIT_PROC_NR       2	/* init -- the process that goes multiuser */
#define LOW_USER           2	/* first user not part of operating system */

/* Miscellaneous */

/* Device numbers of root (RAM) and boot (fd0) devices. */

/* Flag bits for i_mode in the inode. */




