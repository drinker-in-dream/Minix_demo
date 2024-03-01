#include "../INCLUDE/lib.h"

PUBLIC int read(int fd, char *buffer, int nbytes)
//int fd;
//char *buffer;
//int nbytes;
{
  int n;
  n = callm1(FS, READ, fd, nbytes, 0, buffer, NIL_PTR, NIL_PTR);
  return(n);
}
