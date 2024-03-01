#include "../H/const.h"
#include "../H/type.h"
#include "../H/error.h"
#include "../H/callnr.h"

extern message M;

#define MM                 0
#define FS                 1

extern int callm1(), callm3(), callx(), len();
extern int errno;
extern int begsig();		/* interrupts all vector here */

