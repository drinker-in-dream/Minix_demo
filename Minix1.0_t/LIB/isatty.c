#include "../INCLUDE/stat.h"

int isatty(int fd)
//int fd;
{
  struct stat s;

  fstat(fd, &s);
  if ( (s.st_mode&S_IFMT) == S_IFCHR)
	return(1);
  else
	return(0);
}
