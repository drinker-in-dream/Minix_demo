/* Integer to ASCII for signed decimal integers. */

static int next;
static char qbuf[16];

char *itoa(int n)
//int n;
{
  int r, k;
  int flag = 0;

  next = 0;
  if (n < 0) {
	qbuf[next++] = '-';
	n = -n;
  }
  if (n == 0) {
	qbuf[next++] = '0';
  } else {
	k = 1000000000;
	while (k > 0) {
		r = n/k;
		if (flag || r > 0) {
			qbuf[next++] = '0' + r;
			flag = 1;
		}
		n -= r * k;
		k = k/10;
	}
  }
  qbuf[next] = 0;
  return(qbuf);
}
