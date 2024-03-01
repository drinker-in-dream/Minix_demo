
/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
                               i8259.c
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
                                                    Toby Du, 2014/7/3
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
#include "../H/const.h"
#include "../H/type.h"
#include "const.h"
#include "type.h"
#include "glo.h"
#include "protect.h"


EXTERN t_pf_irq_handler	irq_table[NR_IRQ];
PUBLIC void spurious_irq(int irq);
/*======================================================================*
                            init_8259A
 *======================================================================*/
PUBLIC void init_8259A()
{
	port_out(INT_M_CTL,	0x11);			// Master 8259, ICW1.
	port_out(INT_S_CTL,	0x11);			// Slave  8259, ICW1.
	port_out(INT_M_CTLMASK,	INT_VECTOR_IRQ0);	// Master 8259, ICW2. set 'master 8259' interrupt entry to 0x20.
	port_out(INT_S_CTLMASK,	INT_VECTOR_IRQ8);	// Slave  8259, ICW2. set 'slave 8259' interrupt entry to 0x28
	port_out(INT_M_CTLMASK,	0x4);			// Master 8259, ICW3. IR2 refer 'slave8259'.
	port_out(INT_S_CTLMASK,	0x2);			// Slave  8259, ICW3. refer 'master8259' IR2.
	port_out(INT_M_CTLMASK,	0x1);			// Master 8259, ICW4.
	port_out(INT_S_CTLMASK,	0x1);			// Slave  8259, ICW4.

	port_out(INT_M_CTLMASK,	0xFF);	// Master 8259, OCW1. 
	port_out(INT_S_CTLMASK,	0xFF);	// Slave  8259, OCW1. 

	int i;
	for(i=0;i<NR_IRQ;i++){
		irq_table[i] = spurious_irq;
	}
}

/*======================================================================*
                            put_irq_handler
 *======================================================================*/
PUBLIC void put_irq_handler(int irq, t_pf_irq_handler handler)
{
	disable_irq(irq);
	irq_table[irq] = handler;
}

/*======================================================================*
                           spurious_irq
 *======================================================================*/
PUBLIC void spurious_irq(int irq)
{
	;
}

