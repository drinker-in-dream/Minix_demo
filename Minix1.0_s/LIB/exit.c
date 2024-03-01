#include "../INCLUDE/lib.h"

PUBLIC int exit(int status)
//int status;
{
  return callm1(MM, EXIT, status, 0, 0, NIL_PTR, NIL_PTR, NIL_PTR);
}
