#include "../INCLUDE/stdio.h"

void _cleanup()
{
	
	int i;

	for ( i = 0 ; i < NFILES ; i++ )
		if ( _io_table[i] != NULL )
			fflush(_io_table[i]);
			
}

