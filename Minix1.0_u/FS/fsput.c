
/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
                               fsput.c
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
                                                    Toby Du, 2015/3/2
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

#include "../H/const.h"
#include "../H/type.h"
#include "../H/com.h"

#define STDOUTPUT          1	/* file descriptor for standard output */
#define BUFSIZE          100	/* print buffer size */

PRIVATE int bufcount;		/* # characters in the buffer */
PRIVATE char printbuf [BUFSIZE];	/* output is buffered here */
PRIVATE message putchmsg;	/* used for message to TTY task */

/*===========================================================================*
 *				flush					     *
 *===========================================================================*/
PRIVATE void fsflush()
{
/* Flush the print buffer. */


	if (bufcount == 0) return;
	putchmsg.m_type = TTY_WRITE;
	putchmsg.PROC_NR  = 1;
	putchmsg.TTY_LINE = 0;
	putchmsg.ADDRESS  = printbuf;
	putchmsg.COUNT = bufcount;
	sendrec(TTY, &putchmsg);
	bufcount = 0;
}

/*===========================================================================*
 *				putc					     *
 *===========================================================================*/
PUBLIC void fsputc(char c)
{

	if (c == 0) {
		fsflush();
		return;
	}
	printbuf[bufcount++] = c;
	if (bufcount == BUFSIZE) fsflush();
	if (c == '\n')  fsflush();
}





