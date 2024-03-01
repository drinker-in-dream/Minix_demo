#include "../INCLUDE/lib.h"

PUBLIC int mount(char *special,char * name, int rwflag)
{
  return callm1(FS, MOUNT, len(special), len(name), rwflag, special, name, NIL_PTR);
}
