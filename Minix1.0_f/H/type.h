
/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
                            type.h
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
                                                  Toby Du, 2014/06/30
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
typedef	unsigned long long	u64;
typedef	unsigned int		u32;
typedef	unsigned short		u16;
typedef	unsigned char		u8;
typedef long long	        s64;
typedef	int		            s32;
typedef	short		        s16;
typedef	char		        s8;

/* Macros */
#define MAX(a,b)	(a > b ? a : b)
#define MIN(a,b)	(a < b ? a : b)

typedef long long real_time;		/* real time in seconds since Jan 1, 1980 */
