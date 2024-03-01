
/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
                            const.h
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
                                                    Toby Du, 2014/7/12
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

/* EXTERN */
#define	EXTERN	extern	/* EXTERN is defined as extern except in global.c */

/* function type */
#define	PUBLIC		/* PUBLIC is the opposite of PRIVATE */
#define	PRIVATE	static	/* PRIVATE x limits the scope of x */

/* Boolean */
#define	TRUE	1
#define	FALSE	0

/* amount of GDT & IDT */
#define	GDT_SIZE	128
#define	IDT_SIZE	256

/* privilege defininition */
#define	PRIVILEGE_KRNL	0
#define	PRIVILEGE_TASK	1
#define	PRIVILEGE_SERVICE 1 
#define	PRIVILEGE_USER	3

/* RPL */
#define	RPL_KRNL	SA_RPL0
#define	RPL_TASK	SA_RPL1
#define	RPL_SERVICE	SA_RPL1
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

#define INIT_ESP (int*)0x0010	/* initial sp: 3 words pushed by kernel */

#define IDLE            -999	/* 'cur_proc' = IDLE means nobody is running */

