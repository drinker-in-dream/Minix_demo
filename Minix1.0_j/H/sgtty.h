
/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
                            sgtty.h
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
                                                  Toby Du, 2014/8/26
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/


/* Fields in t_flags. */
#define XTABS	     0006000	/* do tab expansion */
#define RAW	     0000040	/* enable raw mode */
#define CRMOD	     0000020	/* map lf to cr + lf */
#define ECHO	     0000010	/* echo input */
#define CBREAK	     0000002	/* enable cbreak mode */
#define COOKED       0000000	/* neither CBREAK nor RAW */


