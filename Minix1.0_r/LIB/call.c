#include "../INCLUDE/lib.h"

PUBLIC int errno;		/* place where error numbers go */

PUBLIC int callx(int proc, int syscallnr)
//int proc;			/* FS or MM */
//int syscallnr;			/* which system call */
{
/* Send a message and get the response.  The 'M.m_type' field of the
 * reply contains a value (>= 0) or an error code (<0). 
 */
  int k;

  M.m_type = syscallnr;
  k = sendrec(proc, &M);
  if (k != OK) return(k);	/* send itself failed */
  if (M.m_type < 0) {errno = -M.m_type; return(-1);}
  return(M.m_type);
}

PUBLIC int callm1(int proc, int syscallnr, int int1, int int2, int int3, char *ptr1, char *ptr2, char *ptr3)
//int proc;			/* FS or MM */
//int syscallnr;			/* which system call */
//int int1;			/* first integer parameter */
//int int2;			/* second integer parameter */
//int int3;			/* third integer parameter */
//char *ptr1;			/* pointer parameter */
//char *ptr2;			/* pointer parameter */
//char *ptr3;			/* pointer parameter */
{
/* Send a message and get the response.  The 'M.m_type' field of the
 * reply contains a value (>= 0) or an error code (<0). Use message format m1.
 */
  M.m1_i1 = int1;
  M.m1_i2 = int2;
  M.m1_i3 = int3;
  M.m1_p1 = ptr1;
  M.m1_p2 = ptr2;
  M.m1_p3 = ptr3;
  return callx(proc, syscallnr);
}

