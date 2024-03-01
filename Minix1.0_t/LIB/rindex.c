char *rindex(char *s,char *c)
{
  register char *result;

  result = 0;
  do
	if (*s == c)
		result = s;
  while (*s++ != 0);
  return(result);
}
