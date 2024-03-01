#include "../INCLUDE/ctype.h"

long atol(char *s)
//char *s;
{
  long total = 0;
  unsigned digit;
  int minus = 0;

  while (isspace(*s))
	s++;
  if (*s == '-') {
	s++;
	minus = 1;
  }
  while ((digit = *s++ - '0') < 10) {
	total *= 10;
	total += digit;
  }
  return(minus ? -total : total);
}
