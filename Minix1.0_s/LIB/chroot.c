#include "../INCLUDE/lib.h"

PUBLIC int chroot(char* name)
//char* name;
{
  return callm3(FS, CHROOT, 0, name);

}
