int strcmp(char *s1, char *s2)
//char *s1, *s2;
{
/* Compare 2 strings. */

  while (1) {
	if (*s1 != *s2) return(*s1 - *s2);
	if (*s1 == 0) return(0);
	s1++;
	s2++;
  }
}
