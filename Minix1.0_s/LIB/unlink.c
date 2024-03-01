#include "../INCLUDE/lib.h"

PUBLIC int unlink(char *name)
{
  return callm3(FS, UNLINK, 0, name);
}
