
/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
                               clock.c
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
                                                    Toby Du, 2014/7/15
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/


#include "../H/const.h"
#include "../H/type.h"
#include "../H/com.h"
#include "const.h"
#include "type.h"
#include "protect.h"
#include "proc.h"
#include "glo.h"

/* Constant definitions. */
#define MILLISEC         100	/* how often to call the scheduler (msec) */
#define SCHED_RATE (MILLISEC*HZ/1000)	/* number of ticks per schedule */

/* Clock parameters. */
#define TIMER0          0x40	/* port address for timer channel 0 */
#define TIMER_MODE      0x43	/* port address for timer channel 3 */
#define IBM_FREQ    1193182L	/* IBM clock frequency for setting timer */
#define SQUARE_WAVE     0x36	/* mode for generating square wave */
#define RATE_GENERATOR  0x34

/* Clock task variables. */
PRIVATE real_time boot_time;	/* time in seconds of system boot */
PRIVATE real_time next_alarm;	/* probable time of next alarm */
PRIVATE int sched_ticks = SCHED_RATE;	/* counter: when 0, call scheduler */
PRIVATE struct proc *prev_ptr;	/* last user process run by clock task */
PRIVATE message mc;		/* message buffer for both input and output */
PRIVATE int (*watch_dog[NR_TASKS+1])();	/* watch_dog functions to call */


/*===========================================================================*
 *				do_get_time				     *
 *===========================================================================*/
PRIVATE do_get_time()
{
/* Get and return the current clock time in ticks. */

  mc.m_type = REAL_TIME;	/* set message type for reply */
  mc.NEW_TIME = boot_time + realtime/HZ;	/* current real time */
}


/*===========================================================================*
 *				do_set_time				     *
 *===========================================================================*/
PRIVATE do_set_time(message *m_ptr)
{
/* Set the real time clock.  Only the superuser can use this call. */
	boot_time = m_ptr->NEW_TIME - realtime/HZ;
}

/*===========================================================================*
 *				accounting				     *
 *===========================================================================*/
PRIVATE accounting()
{
/* Update user and system accounting times.  The variable 'bill_ptr' is always
 * kept pointing to the process to charge for CPU usage.  If the CPU was in
 * user code prior to this clock tick, charge the tick as user time, otherwise
 * charge it as system time.
 */

  if (prev_proc >= LOW_USER)
	bill_ptr->user_time++;	/* charge CPU time */
  else
	bill_ptr->sys_time++;	/* charge system time */
}

/*===========================================================================*
 *				clock_task				     *
 *===========================================================================*/
PUBLIC void  clock_task()
{
/* Main program of clock task.  It determines which of the 4 possible
 * calls this is by looking at 'mc.m_type'.   Then it dispatches.
 */
 
  int opcode;

    /* Main loop of the clock task.  Get work, process it, sometimes reply. */
  while (TRUE) {
     //receive(ANY, &mc);		/* go get a message */
     //opcode = mc.m_type;	/* extract the function code */

    switch (opcode) {
	//	case SET_ALARM:	 do_setalarm(&mc);	break;
	//	case GET_TIME:	 do_get_time();		break;
	//	case SET_TIME:	 do_set_time(&mc);	break;
	//	case CLOCK_TICK: do_clocktick();	break;
	//	default: panic("clock task got bad message", mc.m_type);
     }

    /* Send reply, except for clock tick. */
    //mc.m_type = OK;
    //if (opcode != CLOCK_TICK) send(mc.m_source, &mc);
	;
  }

}


/*======================================================================*
                           clock_handler
 *======================================================================*/
PUBLIC void clock_handler(int irq)
{	
	//int_mess.m_type = CLOCK_TICK;
	//interrupt(CLOCK,&int_mess);

	proc_ptr++ ;
	if (proc_ptr > proc + 6) {
		proc_ptr = proc;
	}
	
}


/*======================================================================*
                           init_clock
 *======================================================================*/
PUBLIC void init_clock()
{
	//out_byte(TIMER_MODE, SQUARE_WAVE);	/* set timer to run continuously */
	out_byte(TIMER_MODE, RATE_GENERATOR);
	out_byte(TIMER0, (u8) (IBM_FREQ/HZ));		/* load timer low byte */
	out_byte(TIMER0, (u8) (IBM_FREQ/HZ) >> 8);		/* load timer high byte */

	put_irq_handler(CLOCK_IRQ, clock_handler);	/* 设定时钟中断处理程序 */
	enable_irq(CLOCK_IRQ);				/* 让8259A可以接收时钟中断 */
}


