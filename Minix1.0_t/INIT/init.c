/* This process is the father (mother) of all MINIX user processes.  When
 * MINIX comes up, this is process 2.  It executes the /etc/rc shell file and
 * then reads the /ect/ttys file to find out which terminals need a login
 * process.
 */

#include "../H/signal.h"
#include "../H/const.h"
#include "../H/type.h"
#include "../H/com.h"

disp_pos = 0;


/*======================================================================*
                               delay
 *======================================================================*/
PUBLIC void delay(int time)
{
	int i, j, k;
	for(k=0;k<time;k++)
	{
		for(i=0;i<1000;i++)
		{
			for(i=0;i<2500;i++)
			{
				for(j=0;j<1000;j++){}
			}
		}
	}
}


/*======================================================================*
                               itoa
 *======================================================================*/
PUBLIC char * itoatinix(char * str, int num)/* 数字前面的 0 不被显示出来, 比如 0000B800 被显示成 B800 */
{
	char *	p = str;
	char	ch;
	int	i;
	int	flag = FALSE;

	*p++ = '0';
	*p++ = 'x';

	if(num == 0){
		*p++ = '0';
	}
	else{	
		for(i=28;i>=0;i-=4){
			ch = (num >> i) & 0xF;
			if(flag || (ch > 0)){
				flag = TRUE;
				ch += '0';
				if(ch > '9'){
					ch += 7;
				}
				*p++ = ch;
			}
		}
	}

	*p = 0;

	return str;
}


/*======================================================================*
                               disp_int
 *======================================================================*/
PUBLIC void disp_int(int input)
{
	char output[16];
	itoatinix(output, input);
	disp_str(output);
}


int main(){

	char line[10];		/* /etc/ttys lines are supposed to be 3 chars */
    int rc, tty, k, status, ttynr, ct, i;
	
	//sync();			/* force buffers out onto RAM disk */
	if(fork()){
		/* Parent just waits. */
		wait(&k);
	}
	else{

		while(1){
			disp_str(" child ");
			delay(1);
		}
	}


	return 0;

}


void printk(char *s,int *arglist)
{


}


