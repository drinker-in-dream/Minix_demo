#include "../INCLUDE/stdio.h"


char *fgets(char *str, unsigned n, FILE *file)
//char *str;
//unsigned n;
//FILE *file;
{
	int ch;
	char *ptr;

	ptr = str;
	while ( --n > 0 && (ch = getc(file)) != EOF){
		*ptr++ = ch;
		if ( ch == '\n')
			break;
	}
	if (ch == EOF && ptr==str)
		return(NULL);
	*ptr = '\0';
	return(str);
}

