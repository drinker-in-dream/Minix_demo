/* System calls. */
#define SEND		   1	/* function code for sending messages */
#define RECEIVE		   2	/* function code for receiving messages */
#define BOTH		   3	/* function code for SEND + RECEIVE */
#define ANY   (NR_PROCS+100)	/* receive(ANY, buf) accepts from any source */

/* Task numbers, function codes and reply codes. */
#define HARDWARE          -1	/* used as source on interrupt generated msgs */