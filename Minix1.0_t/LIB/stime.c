#include "../INCLUDE/lib.h"

PUBLIC int stime(long *top)
{
  M.m2_l1 = *top;
  return callx(FS, STIME);
}
