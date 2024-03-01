
/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
                            main.c
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
                                                  Toby Du, 2014/6/30
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
#include "../H/const.h"
#include "../H/type.h"
#include "const.h"
#include "type.h"
#include "glo.h"
#include "proc.h"
#include "protect.h"

//------------->> P R O G R A M    S E T U P <<-------------
EXTERN u8		gdt_ptr[6];	// 0~15:Limit  16~47:Base
EXTERN DESCRIPTOR	gdt[GDT_SIZE];
EXTERN u8		idt_ptr[6];	// 0~15:Limit  16~47:Base
EXTERN GATE		idt[IDT_SIZE];
EXTERN disp_pos;
EXTERN	char	task_stack[0x8000];
EXTERN	u32		k_reenter;
/*======================================================================*
                            main
 *======================================================================*/
PUBLIC int dox_main()
{
	
disp_str("A");

}


