
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


/*===========================================================================*
 *				main					     *
 *===========================================================================*/
PUBLIC void mm()
{
/* Main routine of the memory manager. */

  int error;

  message mc1;
	int i = 0;
	while(1){
		disp_str(" mm123 ");
		receive(12, &mc1);		/* go get a message */
		delay(1);
	}
}

