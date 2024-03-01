#include "../INCLUDE/lib.h"

PUBLIC int open(char* name, int mode)
//char* name;
//int mode;
{
  return callm3(FS, OPEN, mode, name);
}
