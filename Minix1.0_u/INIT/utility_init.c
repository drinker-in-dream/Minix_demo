/* MM must occasionally print some message.  It uses the standard library
 * routine prink().  (The name "printf" is really a macro defined as "printk").
 * Printing is done by calling the TTY task directly, not going through FS.
 */

#include "../H/const.h"
#include "../H/type.h"
#include "../H/com.h"

#define STD_OUTPUT          1	/* file descriptor for standard output */
#define BUF_SIZE          100	/* print buffer size */
disp_pos = 0;

int buf_count;		/* # characters in the buffer */
char print_buf[BUF_SIZE];	/* output is buffered here */
message putch_msg;	/* used for message to TTY task */

/*===========================================================================*
 *				putc					     *
 *===========================================================================*/
PUBLIC putc_init(char c)
{

  /* Accumulate another character.  If '\n' or buffer full, print it. */
  print_buf[buf_count++] = c;
  if (buf_count == BUF_SIZE) F_l_u_s_h();
  if (c == '\n')  {F_l_u_s_h();
	//disp_str(print_buf);
	//disp_int(print_buf);
	//disp_str("\n");
	}
}


/*===========================================================================*
 *				F_l_u_s_h				     *
 *===========================================================================*/
PRIVATE void F_l_u_s_h()
{
/* Flush the print buffer by calling TTY task. */	
  if (buf_count == 0) return;
  putch_msg.m_type = TTY_WRITE;
  putch_msg.PROC_NR  = 0;
  putch_msg.TTY_LINE = 0;
  putch_msg.ADDRESS  = print_buf;
  putch_msg.COUNT = buf_count;
  sendrec(TTY, &putch_msg);
  buf_count = 0;
}


#define MAXDIGITS         12

static int bintoascii(long num, int radix, char a[MAXDIGITS]);

void printk(char *s,int *arglist)
{
  int w, k, r, *valp;
  unsigned u;
  long l, *lp;
  char a[MAXDIGITS], *p, *p1, c;

  valp = (int *)  &arglist;
  while (*s != '\0') {
	if (*s !=  '%') {
		putc_init(*s++);
		continue;
	}

	w = 0;
	s++;
	while (*s >= '0' && *s <= '9') {
		w = 10 * w + (*s - '0');
		s++;
	}

	lp = (long *) valp;

	switch(*s) {
	    case 'd':	k = *valp++; l = k;  r = 10;  break;
	    case 'o':	k = *valp++; u = k; l = u;  r = 8;  break;
	    case 'x':	k = *valp++; u = k; l = u;  r = 16;  break;
	    case 'D':	l = *lp++;  r = 10; valp = (int *) lp; break;
	    case 'O':	l = *lp++;  r = 8;  valp = (int *) lp; break;
	    case 'X':	l = *lp++;  r = 16; valp = (int *) lp; break;
	    case 'c':	k = *valp++; putc_init(k); s++; continue;
	    case 's':	p = (char *) *valp++; 
			p1 = p;
			while(c = *p++) putc_init(c); s++;
			if ( (k = w - (p-p1-1)) > 0) while (k--) putc_init(' ');
			continue;
	    default:	putc_init('%'); putc_init(*s++); continue;
	}

	k = bintoascii(l, r, a);
	if ( (r = w - k) > 0) while(r--) putc_init(' ');
	for (r = k - 1; r >= 0; r--) putc_init(a[r]);
	s++;
  }
}



static int bintoascii(long num, int radix, char a[MAXDIGITS])
{

  int i, n, hit, negative;

  negative = 0;
  if (num == 0) {a[0] = '0'; return(1);}
  if (num < 0 && radix == 10) {num = -num; negative++;}
  for (n = 0; n < MAXDIGITS; n++) a[n] = 0;
  n = 0;

  do {
	if (radix == 10) {a[n] = num % 10; num = (num -a[n])/10;}
	if (radix ==  8) {a[n] = num & 0x7;  num = (num >> 3) & 0x1FFFFFFF;}
	if (radix == 16) {a[n] = num & 0xF;  num = (num >> 4) & 0x0FFFFFFF;}
	n++;
  } while (num != 0);

  /* Convert to ASCII. */
  hit = 0;
  for (i = n - 1; i >= 0; i--) {
	if (a[i] == 0 && hit == 0) {
		a[i] = ' ';
	} else {
		if (a[i] < 10)
			a[i] += '0';
		else
			a[i] += 'A' - 10;
		hit++;
	}
  }
  if (negative) a[n++] = '-';
  return(n);
}


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

