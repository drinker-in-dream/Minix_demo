
/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
                            const.h
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
                                                    Toby Du, 2013
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

#define	GDT_SIZE	128
#define	IDT_SIZE	256

/* privilege defininition */
#define	PRIVILEGE_KRNL	0
#define	PRIVILEGE_TASK	1
#define	PRIVILEGE_USER	3

/* privilege defininition */
#define	PRIVILEGE_KRNL	0
#define	PRIVILEGE_TASK	1
#define	PRIVILEGE_USER	3
/* RPL */
#define	RPL_KRNL	SA_RPL0
#define	RPL_TASK	SA_RPL1
#define	RPL_USER	SA_RPL3

/* 8259A interrupt controller ports. */
#define	INT_M_CTL	0x20	/* I/O port for interrupt controller         <Master> */
#define	INT_M_CTLMASK	0x21	/* setting bits in this port disables ints   <Master> */
#define	INT_S_CTL	0xA0	/* I/O port for second interrupt controller  <Slave>  */
#define	INT_S_CTLMASK	0xA1	/* setting bits in this port disables ints   <Slave>  */

/* Hardware interrupts */
#define	NR_IRQ		16	/* Number of IRQs */
#define	CLOCK_IRQ	0
#define	KEYBOARD_IRQ	1
#define	CASCADE_IRQ	2	/* cascade enable for 2nd AT controller */
#define	ETHER_IRQ	3	/* default ethernet interrupt vector */
#define	SECONDARY_IRQ	3	/* RS232 interrupt vector for port 2 */
#define	RS232_IRQ	4	/* RS232 interrupt vector for port 1 */
#define	XT_WINI_IRQ	5	/* xt winchester */
#define	FLOPPY_IRQ	6	/* floppy disk */
#define	PRINTER_IRQ	7
#define	AT_WINI_IRQ	14	/* at winchester */

/* p_reg contains: ax, bx, cx, dx, si, di, bp, es, ds, cs, ss in that order. */
//#define NR_REGS           11	/* number of general regs in each proc slot (may not use in x86) */  
#define INIT_PSW      0x0200	/* initial psw */
//#define INIT_SP (int*)0x0010	/* initial sp: 3 words pushed by kernel(may not use in x86) */

//#define TASK_STACK_BYTES 256	/* how many bytes for each task stack (may not use in x86)*/
//#define K_STACK_BYTES    256	/* how many bytes for the kernel stack (may not use in x86)*/

/* The following items pertain to the 3 scheduling queues. */
#define NQ                 3	/* # of scheduling queues */
#define TASK_Q             0	/* ready tasks are scheduled via queue 0 */
#define SERVER_Q           1	/* ready servers are scheduled via queue 1 */
#define USER_Q             2	/* ready users are scheduled via queue 2 */



