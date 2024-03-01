
/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
                               mm_main.c
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
                                                    Toby Du, 2015/5/26
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
#include "../H/const.h"
#include "../H/type.h"
#include "../H/callnr.h"
#include "../H/com.h"
#include "../H/error.h"
#include "const.h"
#include "glo.h"
#include "mproc.h"
#include "param.h"

#define ENOUGH (phys_clicks) 4096	/* any # > max(FS size, INIT size) */
#define CLICK_TO_K (1024L/CLICK_SIZE)	/* convert clicks to K */
#define MEM1MSTART			0x100000	 // 1M


PRIVATE phys_clicks tot_mem;
extern (*call_vec[])();
extern PUBLIC MemoInfo * meminfoptr;
extern	u32 dwMCRNumber;
extern	u32 MemChkBuf[256];
/* The service stacks. */
EXTERN struct s_stack {
  int stk[4096/sizeof(int)];
}s_stack[NR_SERVICES + 3];


PRIVATE void  get_tot_mem(u32 *membase,u32 *memlength){
	int i;
	
	for(i=0;i<dwMCRNumber;i++){
		if( meminfoptr[i].BaseAddrLow >= MEM1MSTART && meminfoptr[i].type==1){
			(*membase)= (meminfoptr[i].BaseAddrLow + 0x600000) >> CLICK_SHIFT ;
			(*memlength) = (meminfoptr[i].LengthLow - 0x600000)  >> CLICK_SHIFT ;
			return ;	
		}
	}

	return ;
}

/*===========================================================================*
 *				mm_init					     *
 *===========================================================================*/
PRIVATE void mm_init()
{
/* Initialize the memory manager. */

	/* Find out how much memory the machine has and set up core map.  MM and FS
     * are part of the map.  Tell the kernel.
     */
	u32 membase;
	u32 memlength;
	extern unsigned sizes[8];	/* table filled in by build */
	meminfoptr = MemChkBuf;
    get_tot_mem(&membase,&memlength);	/* # clicks in mem starting at absolute 0 */
	mem_init(membase,memlength);		/* initialize tables to all physical mem */
	

	/* Initialize MM's tables. */
    mproc[MM_PROC_NR].mp_flags |= IN_USE;
    mproc[FS_PROC_NR].mp_flags |= IN_USE;
    mproc[INIT_PROC_NR].mp_flags |= IN_USE;

	mproc[MM_PROC_NR].mp_seg[T].mem_phys = sizes[0];
	mproc[MM_PROC_NR].mp_seg[T].mem_vir = sizes[0];
	mproc[MM_PROC_NR].mp_seg[T].mem_len = sizes[1];
    mproc[MM_PROC_NR].mp_seg[S].mem_phys = s_stack[MM_PROC_NR].stk;
	mproc[MM_PROC_NR].mp_seg[S].mem_vir = s_stack[MM_PROC_NR].stk;
	mproc[MM_PROC_NR].mp_seg[S].mem_len = 4096  >> CLICK_SHIFT;

	mproc[FS_PROC_NR].mp_seg[T].mem_phys = sizes[0];
	mproc[FS_PROC_NR].mp_seg[T].mem_vir = sizes[0];
	mproc[FS_PROC_NR].mp_seg[T].mem_len = sizes[1];
    mproc[FS_PROC_NR].mp_seg[S].mem_phys = s_stack[FS_PROC_NR].stk;
	mproc[FS_PROC_NR].mp_seg[S].mem_vir = s_stack[FS_PROC_NR].stk;
	mproc[FS_PROC_NR].mp_seg[S].mem_len = 4096  >> CLICK_SHIFT;

	mproc[INIT_PROC_NR].mp_seg[T].mem_phys = sizes[6];
	mproc[INIT_PROC_NR].mp_seg[T].mem_vir = sizes[6];
	mproc[INIT_PROC_NR].mp_seg[T].mem_len = sizes[7];
    mproc[INIT_PROC_NR].mp_seg[S].mem_phys = 0x7f000 >> CLICK_SHIFT;
	mproc[INIT_PROC_NR].mp_seg[S].mem_vir = 0x7f000 >> CLICK_SHIFT;
	mproc[INIT_PROC_NR].mp_seg[S].mem_len = 4096  >> CLICK_SHIFT;

    procs_in_use = 3;

}

/*===========================================================================*
 *				get_work				     *
 *===========================================================================*/
PRIVATE void get_work()
{  
/* Wait for the next message and extract useful information from it. */

  if (receive(ANY, &mm_in) != OK) panic("MM receive error", NO_NUM);
  who = mm_in.m_source;		/* who sent the message */
  if (who < HARDWARE || who >= NR_PROCS) panic("MM called by", who);
  mm_call = mm_in.m_type;	/* system call number */
}


/*===========================================================================*
 *				reply					     *
 *===========================================================================*/
PUBLIC void mm_reply(int proc_nr, int result, int res2, char *respt)
//int proc_nr;			/* process to reply to */
//int result;			/* result of the call (usually OK or error #)*/
//int res2;			/* secondary result */
//char *respt;			/* result if pointer */
{
/* Send a reply to a user process. */

  struct mproc *proc_ptr;
  /* To make MM robust, check to see if destination is still alive. */
  proc_ptr = &mproc[proc_nr];
  if ( (proc_ptr->mp_flags&IN_USE) == 0 || (proc_ptr->mp_flags&HANGING)) return;
  reply_type = result;
  reply_i1 = res2;
  reply_p1 = respt;
  if (send(proc_nr, &mm_out) != OK) panic("MM can't reply", NO_NUM);
}



/*===========================================================================*
 *				main					     *
 *===========================================================================*/
PUBLIC void mm()
{
/* Main routine of the memory manager. */

  int error;
  mm_init();			/* initialize memory manager tables */
  while(TRUE){
	/* Wait for message. */
	get_work();		/* wait for an MM system call */
	mp = &mproc[who];
	/* Set some flags. */
	error = OK;
	dont_reply = FALSE;
	err_code = -999;

	/* If the call number is valid, perform the call. */
	if (mm_call < 0 || mm_call >= NCALLS)
		error = E_BAD_CALL;
	else
		error = (*call_vec[mm_call])();

	/* Send the results back to the user to indicate completion. */
	if (dont_reply) continue;	/* no reply for EXIT and WAIT */
	if (mm_call == EXEC && error == OK) continue;
	mm_reply(who, error, result2, res_ptr);
  }
}

/*===========================================================================*
 *				do_brk2	   				     *
 *===========================================================================*/
PUBLIC do_brk2()
{
/* This "call" is made once by FS during system initialization and then never
 * again by anyone.  It contains the origin and size of INIT, and the combined
 * size of the 1536 bytes of unused mem, MINIX and RAM disk.
 *   m1_i1 = size of INIT text in clicks
 *   m1_i2 = size of INIT data in clicks
 *   m1_i3 = number of bytes for MINIX + RAM DISK
 *   m1_p1 = origin of INIT in clicks
 */


	return(OK);

}

/*===========================================================================*
 *				set_map	   				     *
 *===========================================================================*/
PRIVATE void set_map(int proc_nr, phys_clicks base, phys_clicks clicks)
//int proc_nr;			/* whose map to set? */
//phys_clicks base;		/* where in memory does the process start? */
//phys_clicks clicks;		/* total size in clicks (sep I & D not used) */
{
}



