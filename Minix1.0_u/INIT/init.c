/* This process is the father (mother) of all MINIX user processes.  When
 * MINIX comes up, this is process 2.  It executes the /etc/rc shell file and
 * then reads the /ect/ttys file to find out which terminals need a login
 * process.
 */

#include "../H/signal.h"
#include "../H/const.h"
#include "../H/type.h"
#include "../H/com.h"
#include "../H/error.h"

int main(){
	char line[10];		/* /etc/ttys lines are supposed to be 3 chars */
    int rc, tty, k, status, ttynr, ct, i;
	volatile int *fsready = FS_READY;
	
	while( (*fsready) != OK ); //wait until fs ok
	
	sync();			/* force buffers out onto RAM disk */
	if(fork()){
		/* Parent just waits. */
		printk("father\n");
		while(1){ 
			delay(1);
		}
		//wait(&k);
	}
	else{
		/* Child exec's the shell to do the work. */
		//if (open("/etc/rc", 0) < 0) exit(-1);
		open("/dev/tty0", 1);	/* std output */
		open("/dev/tty0", 1);	/* std error */
		printk("child\n");
		execn("/bin/sh");
		while(1){ 
			delay(1);
		}
		exit(-2);		/* impossible */
	}


	return 0;

}



