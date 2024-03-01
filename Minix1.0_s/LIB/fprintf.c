#include "../INCLUDE/stdio.h"

void fprintf (FILE *file, char *fmt, int args)
//FILE *file;
//char *fmt;
//int args;
{
	_doprintf (file, fmt, &args);
	if ( testflag(file,PERPRINTF) )
        	fflush(file);
}


void printf (char *fmt, int args)
//char *fmt;
//int args;
{
	_doprintf (stdout, fmt, &args);
	if ( testflag(stdout,PERPRINTF) )
        	fflush(stdout);
}
