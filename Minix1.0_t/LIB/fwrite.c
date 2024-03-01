#include "../INCLUDE/stdio.h"

fwrite(char *ptr, unsigned size, unsigned count, FILE *file)
//unsigned size, count;
//char *ptr;
//FILE *file;
{
	unsigned s;
	unsigned ndone = 0;

	if (size)
		while ( ndone < count ) {
			s = size;
			do {
				putc(*ptr++, file);
				if (ferror(file))
					return(ndone);
			} 
			while (--s);
			ndone++;
		}
	return(ndone);
}
