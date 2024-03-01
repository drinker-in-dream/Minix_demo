    /* library routine for copying structs with unpleasant alignment */

    __stb(int n, char *f, char *t)
    {
	    if (n > 0)
		    do
			    *t++ = *f++;
		    while (--n);
    }
