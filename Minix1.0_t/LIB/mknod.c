#include "../INCLUDE/lib.h"

PUBLIC int mknod(char *name,int mode,int addr)
//char *name;
//int mode, addr;
{
  return callm1(FS, MKNOD, len(name), mode, addr, name, NIL_PTR, NIL_PTR);
}
