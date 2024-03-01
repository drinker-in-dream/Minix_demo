void bcopy(char *old,char * new, int n)
//char *old, *new;
//int n;
{
/* Copy a block of data. */

  while (n--) *new++ = *old++;
}
