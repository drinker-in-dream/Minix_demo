
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

//------------->> P R O G R A M    S E T U P <<-------------
EXTERN u8		gdt_ptr[6];	// 0~15:Limit  16~47:Base
EXTERN DESCRIPTOR	gdt[GDT_SIZE];
EXTERN u8		idt_ptr[6];	// 0~15:Limit  16~47:Base
EXTERN GATE		idt[IDT_SIZE];
EXTERN disp_pos;
/*======================================================================*
                            main
 *======================================================================*/
PUBLIC void cstart(){
   // copy the GDT in LOADER to new GDT table here
	memcpy(	&gdt,				    // New GDT
		(void*)(*((u32*)(&gdt_ptr[2]))),   // Base  of Old GDT
		*((u16*)(&gdt_ptr[0])) + 1	    // Limit of Old GDT
		);

	// gdt_ptr[6]  6 byte total£º0~15:Limit  16~47:Base
	u16* p_gdt_limit = (u16*)(&gdt_ptr[0]);
	u32* p_gdt_base  = (u32*)(&gdt_ptr[2]);
	*p_gdt_limit = GDT_SIZE * sizeof(DESCRIPTOR) - 1;
	*p_gdt_base  = (u32)&gdt;

	// idt_ptr[6] 6 byte total£º0~15:Limit  16~47:Base.
	u16* p_idt_limit = (u16*)(&idt_ptr[0]);
	u32* p_idt_base  = (u32*)(&idt_ptr[2]);
    *p_idt_limit = IDT_SIZE * sizeof(GATE) - 1;
    *p_idt_base  = (u32)&idt;
	
    prot_init();

}

