#include "../INCLUDE/stdio.h"

void fputs(char *s,FILE *file)
//char *s;
//FILE *file;
{
	while ( *s ) 
		putc(*s++,file);
}
