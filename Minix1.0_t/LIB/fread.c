#include "../INCLUDE/stdio.h"

void fread(char *ptr, unsigned size, unsigned count, FILE *file)
//char *ptr;
//unsigned size, count;
//FILE *file;
{
	int c;
	unsigned ndone = 0, s;

	ndone = 0;
	if (size)
		while ( ndone < count ) {
			s = size;
			do {
				if ((c = getc(file)) != EOF)
					*ptr++ = (char) c;
				else
					return(ndone);
			} while (--s);
			ndone++;
		}
	return(ndone);
}

