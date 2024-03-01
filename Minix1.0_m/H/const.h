
/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
                            const.h
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
                                                    Toby Du, 2013
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
#define EXTERN        extern	/* used in *.h files */
#define PRIVATE       static	/* PRIVATE x limits the scope of x */
#define PUBLIC			/* PUBLIC is the opposite of PRIVATE */

#define HZ	          50	/* clock freq (software settable on IBM-PC) */
#define BLOCK_SIZE      1024	/* # bytes in a disk block */

/* Miscellaneous */
#define BYTE            0377	/* mask for 8 bits */

#define TRUE               1	/* used for turning integers into Booleans */
#define FALSE              0	/* used for turning integers into Booleans */


#define NR_TASKS           8	/* number of tasks in the transfer vector */
#define NR_PROCS          16	/* number of slots in proc table */
#define NR_SERVICES          3	    /* number of service MM FS INIT */
#define NR_SEGS            3	/* # segments per process */
#define T                  0	/* proc[i].mem_map[T] is for text */
#define D                  1	/* proc[i].mem_map[D] is for data */
#define S                  2	/* proc[i].mem_map[S] is for stack */
#define MAX_P_LONG  2147483647	/* maximum positive long, i.e. 2**31 - 1 */
/* Memory is allocated in clicks. */
#define CLICK_SIZE      0x20	/* unit in which memory is allocated */
#define CLICK_SHIFT        5	/* log2 of CLICK_SIZE */

/* Process numbers of some important processes */
#define MM_PROC_NR         0	/* process number of memory manager */
#define FS_PROC_NR         1	/* process number of file system */
#define INIT_PROC_NR       2	/* init -- the process that goes multiuser */
#define LOW_USER           2	/* first user not part of operating system */

#define TASK_STACK_BYTES 1024	/* how many bytes for each task stack */
#define K_STACK_BYTES    1024	/* how many bytes for the kernel stack */

/* Miscellaneous */
#define NIL_PTR   (char *) 0	/* generally useful expression */

/* Device numbers of root (RAM) and boot (fd0) devices. */

/* Flag bits for i_mode in the inode. */

/* The following items pertain to the 3 scheduling queues. */
#define NQ                 3	/* # of scheduling queues */
#define TASK_Q             0	/* ready tasks are scheduled via queue 0 */
#define SERVER_Q           1	/* ready servers are scheduled via queue 1 */
#define USER_Q             2	/* ready users are scheduled via queue 2 */




