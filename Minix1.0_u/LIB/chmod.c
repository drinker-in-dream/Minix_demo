#include "../INCLUDE/lib.h"

PUBLIC int chmod(char* name, int mode)
//char* name;
//int mode;
{
  return callm3(FS, CHMOD, mode, name);

}
