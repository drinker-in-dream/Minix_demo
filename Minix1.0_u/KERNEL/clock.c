
/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
                               clock.c
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
                                                    Toby Du, 2014/7/15
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/


#include "../H/const.h"
#include "../H/type.h"
#include "../H/com.h"
#include "../H/signal.h"
#include "../H/error.h"
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
PRIVATE real_time boot_time = 0;	/* time in seconds of system boot */
PRIVATE real_time next_alarm = 0;	/* probable time of next alarm */
PRIVATE volatile int sched_ticks = SCHED_RATE;	/* counter: when 0, call scheduler */
PRIVATE struct s_proc *prev_ptr = NIL_PROC;	/* last user process run by clock task */
PRIVATE message mc;		/* message buffer for both input and output */
PRIVATE int (*watch_dog[NR_TASKS+1])();	/* watch_dog functions to call */
PRIVATE void accounting();

/*===========================================================================*
 *				do_get_time				     *
 *===========================================================================*/
PRIVATE void do_get_time()
{
/* Get and return the current clock time in ticks. */

  mc.m_type = REAL_TIME;	/* set message type for reply */
  mc.NEW_TIME = boot_time + realtime/HZ;	/* current real time */
}


/*===========================================================================*
 *				do_set_time				     *
 *===========================================================================*/
PRIVATE void do_set_time(message *m_ptr)
{
/* Set the real time clock.  Only the superuser can use this call. */
	boot_time = m_ptr->NEW_TIME - realtime/HZ;
}

/*===========================================================================*
 *				do_setalarm				     *
 *===========================================================================*/
PRIVATE void do_setalarm(message *m_ptr)
//message *m_ptr;			/* pointer to request message */
{
/* A process wants an alarm signal or a task wants a given watch_dog function
 * called after a specified interval.  Record the request and check to see
 * it is the very next alarm needed.
 */

  register struct s_proc *rp;
  int proc_nr;			/* which process wants the alarm */
  long delta_ticks;		/* in how many clock ticks does he want it? */
  int (*function)();		/* function to call (tasks only) */
  /* Extract the parameters from the message. */
  proc_nr = m_ptr->CLOCK_PROC_NR;	/* process to interrupt later */
  delta_ticks = m_ptr->DELTA_TICKS;	/* how many ticks to wait */
  function = m_ptr->FUNC_TO_CALL;	/* function to call (tasks only) */
  rp = proc_addr(proc_nr);
  mc.SECONDS_LEFT = (rp->p_alarm == 0L ? 0 : (rp->p_alarm - realtime)/HZ );
  rp->p_alarm = (delta_ticks == 0L ? 0L : realtime + delta_ticks);
  if (proc_nr < 0) watch_dog[-proc_nr] = function;

  /* Which alarm is next? */
  next_alarm = MAX_P_LONG;
  for (rp = &proc[0]; rp < &proc[NR_TASKS+NR_PROCS]; rp++)
	if(rp->p_alarm != 0 && rp->p_alarm < next_alarm)next_alarm=rp->p_alarm;

}

/*===========================================================================*
 *				do_clocktick				     *
 *===========================================================================*/
PRIVATE void do_clocktick()
{
/* This routine called on every clock tick. */
	register struct s_proc *rp;
	register int t, proc_nr;
  /* To guard against race conditions, first copy 'lost_ticks' to a local
   * variable, add this to 'realtime', and then subtract it from 'lost_ticks'.
   */
    t = lost_ticks;		/* 'lost_ticks' counts missed interrupts */
    realtime += t + 1;		/* update the time of day */
    lost_ticks -= t;		/* these interrupts are no longer missed */

	if (next_alarm <= realtime) {
	/* An alarm may have gone off, but proc may have exited, so check. */
	    next_alarm = MAX_P_LONG;	/* start computing next alarm */
	    for (rp = &proc[0]; rp < &proc[NR_TASKS+NR_PROCS]; rp++) {
		    if (rp->p_alarm != (real_time) 0) {
			    /* See if this alarm time has been reached. */
			    if (rp->p_alarm <= realtime) {
				/* A timer has gone off.  If it is a user proc,
				 * send it a signal.  If it is a task, call the
				 * function previously specified by the task.
				 */
				    proc_nr = rp - proc - NR_TASKS;
				    if (proc_nr >= 0)
					   cause_sig(proc_nr, SIGALRM);
				    else
					   (*watch_dog[-proc_nr])();
				    rp->p_alarm = 0;
			    }

			    /* Work on determining which alarm is next. */
			    if (rp->p_alarm != 0 && rp->p_alarm < next_alarm)
				    next_alarm = rp->p_alarm;
		    }
	    }
  }

  accounting();			/* keep track of who is using the cpu */

  /* If a user process has been running too long, pick another one. */
  if(--sched_ticks == 0){
	  if (bill_ptr == prev_ptr) 
		sched();	
	sched_ticks = SCHED_RATE;		
	prev_ptr = bill_ptr;
  }

}

/*===========================================================================*
 *				accounting				     *
 *===========================================================================*/
PRIVATE void accounting()
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
  init_clock();
    /* Main loop of the clock task.  Get work, process it, sometimes reply. */
  while (TRUE) {
     receive(ANY, &mc);		/* go get a message */
     opcode = mc.m_type;	/* extract the function code */
    switch (opcode) {
		case SET_ALARM:	 do_setalarm(&mc);	break;
		case GET_TIME:	 do_get_time();		break;
		case SET_TIME:	 do_set_time(&mc);	break;
		case CLOCK_TICK: do_clocktick();	break;
		//default: panic("clock task got bad message", mc.m_type);
		default: disp_str("clock task got bad message");
     }

    /* Send reply, except for clock tick. */
    mc.m_type = OK;
    if (opcode != CLOCK_TICK) send(mc.m_source, &mc);
	
  }
  

}


/*======================================================================*
                           clock_handler
 *======================================================================*/
PUBLIC void clock_handler(int irq)
{	
	int_mess.m_type = CLOCK_TICK;
	interrupt(CLOCK,&int_mess);	
	return;
}


/*======================================================================*
                           init_clock
 *======================================================================*/
PUBLIC void init_clock()
{
/* Initialize channel 2 of the 8253A timer to e.g. 60 Hz. */

    unsigned int count, low_byte, high_byte;

    count = (unsigned) (IBM_FREQ/HZ);	/* value to load into the timer */
    low_byte = count & BYTE;		/* compute low-order byte */
    high_byte = (count >> 8) & BYTE;	/* compute high-order byte */
    port_out(TIMER_MODE, SQUARE_WAVE);	/* set timer to run continuously */
    port_out(TIMER0, low_byte);		/* load timer low byte */
    port_out(TIMER0, high_byte);		/* load timer high byte */

	put_irq_handler(CLOCK_IRQ, clock_handler);	/* 设定时钟中断处理程序 */
	enable_irq(CLOCK_IRQ);	/* 让8259A可以接收时钟中断 */
	return;
}


