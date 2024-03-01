
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
#include "file.h"
#include "fproc.h"
#include "glo.h"
#include "inode.h"
#include "param.h" 

/*===========================================================================*
 *				winchester_task				     * 
 *===========================================================================*/
PUBLIC void fs()
{
	int error;
		message mc1;
	int i = 0;

	fs_init();
	//disp_str("\n");
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

}




