#include "../INCLUDE/lib.h"
#include "../INCLUDE/signal.h"

PRIVATE alfun(){}		/* used with sleep() below */
PUBLIC sleep(int n)
{
/* sleep(n) pauses for 'n' seconds by scheduling an alarm interrupt. */
  signal(SIGALRM, alfun);
  alarm(n);
  pause();
}
