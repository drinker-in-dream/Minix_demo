/* This file contains the table used to map system call numbers onto the
 * routines that perform them.
 */

#include "../h/const.h"
#include "../h/type.h"
#include "const.h"

#undef EXTERN
#define EXTERN

#include "../h/callnr.h"
#include "glo.h"
#include "mproc.h"
#include "param.h"

/* Miscellaneous */
char core_name[] = {"core"};	/* file name where core images are produced */
unshort core_bits = 0x0EFC;	/* which signals cause core images */

extern char mm_stack[];
char *stackpt = &mm_stack[MM_STACK_BYTES];	/* initial stack pointer */

extern no_sys_mm(),do_mm_exit(), do_fork_mm(),do_wait(),do_brk(),do_getset(), do_exec(); 
extern do_signal(), do_kill(), do_pause(), do_alarm();
extern  unpause(), do_ksig(), do_brk2();

int (*call_vec[NCALLS])() = {
	no_sys_mm,		/*  0 = unused	*/
	do_mm_exit,  	/*  1 = exit	*/
	do_fork_mm,	    /*  2 = fork	*/
	no_sys_mm,		/*  3 = read	*/
	no_sys_mm,		/*  4 = write	*/
	no_sys_mm,		/*  5 = open	*/
	no_sys_mm,		/*  6 = close	*/
	do_wait,	    /*  7 = wait	*/
	no_sys_mm,		/*  8 = creat	*/
	no_sys_mm,		/*  9 = link	*/
	no_sys_mm,		/* 10 = unlink	*/
	no_sys_mm,		/* 11 = exec	*/
	no_sys_mm,		/* 12 = chdir	*/
	no_sys_mm,		/* 13 = time	*/
	no_sys_mm,		/* 14 = mknod	*/
	no_sys_mm,		/* 15 = chmod	*/
	no_sys_mm,		/* 16 = chown	*/
	do_brk,		    /* 17 = break	*/
	no_sys_mm,		/* 18 = stat	*/
	no_sys_mm,		/* 19 = lseek	*/
	do_getset,	    /* 20 = getpid	*/
	no_sys_mm,		/* 21 = mount	*/
	no_sys_mm,		/* 22 = umount	*/
	do_getset,	    /* 23 = setuid	*/
	do_getset,	    /* 24 = getuid	*/
	no_sys_mm,		/* 25 = stime	*/
	no_sys_mm,		/* 26 = (ptrace)*/
	do_alarm,	    /* 27 = alarm	*/
	no_sys_mm,		/* 28 = fstat	*/
	do_pause,	    /* 29 = pause	*/
	no_sys_mm,		/* 30 = utime	*/
	no_sys_mm,		/* 31 = (stty)	*/
	no_sys_mm,		/* 32 = (gtty)	*/
	no_sys_mm,		/* 33 = access	*/
	no_sys_mm,		/* 34 = (nice)	*/
	no_sys_mm,		/* 35 = (ftime)	*/
	no_sys_mm,		/* 36 = sync	*/
	do_kill,	    /* 37 = kill	*/
	no_sys_mm,		/* 38 = unused	*/
	no_sys_mm,		/* 39 = unused	*/
	no_sys_mm,		/* 40 = unused	*/
	no_sys_mm,		/* 41 = dup	*/
	no_sys_mm,		/* 42 = pipe	*/
	no_sys_mm,		/* 43 = times	*/
	no_sys_mm,		/* 44 = (prof)	*/
	no_sys_mm,		/* 45 = unused	*/
	do_getset,	    /* 46 = setgid	*/
	do_getset,	    /* 47 = getgid	*/
	do_signal,	    /* 48 = sig	*/
	no_sys_mm,		/* 49 = unused	*/
	no_sys_mm,		/* 50 = unused	*/
	no_sys_mm,		/* 51 = (acct)	*/
	no_sys_mm,		/* 52 = (phys)	*/
	no_sys_mm,		/* 53 = (lock)	*/
	no_sys_mm,		/* 54 = ioctl	*/
	no_sys_mm,		/* 55 = unused	*/
	no_sys_mm,		/* 56 = (mpx)	*/
	no_sys_mm,		/* 57 = unused	*/
	no_sys_mm,		/* 58 = unused	*/
	do_exec,	    /* 59 = exece	*/
	no_sys_mm,		/* 60 = umask	*/
	no_sys_mm,		/* 61 = chroot	*/
	no_sys_mm,		/* 62 = unused	*/
	no_sys_mm,		/* 63 = unused	*/
	do_ksig,	    /* 64 = KSIG: signals originating in the kernel	*/
	no_sys_mm,		/* 65 = UNPAUSE	*/
	do_brk2, 	    /* 66 = BRK2 (used to tell MM size of FS,INIT) */
	no_sys_mm,		/* 67 = REVIVE	*/
	no_sys_mm		/* 68 = TASK_REPLY	*/
	
};

