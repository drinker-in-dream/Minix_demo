#include "../INCLUDE/lib.h"

PUBLIC int chdir(char *name)
//char *name;
{
  return callm3(FS, CHDIR, 0, name);

}
