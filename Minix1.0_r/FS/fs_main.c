
/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
                               fs_main.c
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
                                                    Toby Du, 2014/12/20
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

#include "../h/const.h"
#include "../h/type.h"
#include "../h/callnr.h"
#include "../h/com.h"
#include "../h/error.h"
#include "const.h"
#include "type.h"
#include "buf.h"
#include "file.h"
#include "fproc.h"
#include "glo.h"
#include "inode.h"
#include "param.h"
#include "super.h" 

#define M64K     0xFFFF0000L	/* 16 bit mask for DMA check */
#define INFO               2	/* where in data_org is info from build */
#define MAX_RAM          512	/* maxium RAM disk size in blocks */

/*===========================================================================*
 *				buf_pool				     *
 *===========================================================================*/
PRIVATE void buf_pool()
{
/* Initialize the buffer pool.  On the IBM PC, the hardware DMA chip is
 * not able to cross 64K boundaries, so any buffer that happens to lie
 * across such a boundary is not used.  This is not very elegant, but all
 * the alternative solutions are as bad, if not worse.  The fault lies with
 * the PC hardware.
 */
  struct buf *bp;
  vir_bytes low_off, high_off;
  phys_bytes org;
  extern phys_clicks get_base();
	
  bufs_in_use = 0;
  front = &buf[0];
  rear = &buf[NR_BUFS - 1];

  for (bp = &buf[0]; bp < &buf[NR_BUFS]; bp++) {
	bp->b_blocknr = NO_BLOCK;
	bp->b_dev = NO_DEV;
	bp->b_next = bp + 1;
	bp->b_prev = bp - 1;
  }
  buf[0].b_prev = NIL_BUF;
  buf[NR_BUFS - 1].b_next = NIL_BUF;

  for (bp = &buf[0]; bp < &buf[NR_BUFS]; bp++) bp->b_hash = bp->b_next;
  buf_hash[NO_BLOCK & (NR_BUF_HASH - 1)] = front;

}

/*===========================================================================*
 *				load_ram				     *
 *===========================================================================*/
PRIVATE void load_ram()
{
/* The root diskette contains a block-by-block image of the root file system
 * starting at 0.  Go get it and copy it to the RAM disk. 
 */
  struct buf *bp, *bp1;
  int count;
  long k_loaded;
  struct super_block *sp;
  block_nr i;
  phys_clicks ram_clicks, init_org, init_text_clicks, init_data_clicks;
  extern phys_clicks data_org[INFO + 2];
  extern struct buf *get_block();



}

/*===========================================================================*
 *				winchester_task				     * 
 *===========================================================================*/
PUBLIC void fs()
{
	int error;
		message mc1;
	int i = 0;
	char testbuff[BLOCK_SIZE];

	fs_init();
	
	while(1){
		disp_str(" fs ");
		receive(12, &mc1);		/* go get a message */
		delay(1);
	}
}


/*===========================================================================*
 *				reply					     *
 *===========================================================================*/
PUBLIC void reply(int whom, int result)
//int whom;			/* process to reply to */
//int result;			/* result of the call (usually OK or error #) */
{
/* Send a reply to a user process. It may fail (if the process has just
 * been killed by a signal, so don't check the return code.  If the send
 * fails, just ignore it.
 */

  reply_type = result;
  send(whom, &m1);
}


/*===========================================================================*
 *				fs_init					     *
 *===========================================================================*/
PRIVATE void fs_init()
{
/* Initialize global variables, tables, etc. */
	struct inode *rip;
	int i;
	extern struct inode *get_inode();
	
	buf_pool();			/* initialize buffer pool */
	load_ram();			/* Load RAM disk from root diskette. */
}




