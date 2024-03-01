#include <stdio.h>
#include <stdlib.h>
#include <memory.h>

#define E_NIDENT 16
#define Elf32_Addr unsigned int 
#define Elf32_Half unsigned short 
#define Elf32_Off unsigned int 
#define Elf32_Sword int 
#define Elf32_Word unsigned int

#define MAX(a,b)	(a > b ? a : b)
#define MIN(a,b)	(a < b ? a : b)

/* Memory is allocated in clicks. */
#define CLICK_SIZE      0x20	/* unit in which memory is allocated */
#define CLICK_SHIFT        5	/* log2 of CLICK_SIZE */

typedef struct ELFHEADER{
	unsigned char e_ident[E_NIDENT];
	Elf32_Half e_type;
	Elf32_Half e_machine;
	Elf32_Word e_version;
	Elf32_Addr e_entry;
	Elf32_Off e_phoff;
	Elf32_Off e_shoff;
	Elf32_Word e_flags;
	Elf32_Half e_ehsize;
	Elf32_Half e_phentsize;
	Elf32_Half e_phnum;
	Elf32_Half e_shentsize;
	Elf32_Half e_shnum;
	Elf32_Half e_shstrndx;
}ELFHeader;

typedef struct ELF32_PHDR{
	Elf32_Word p_type;
	Elf32_Off p_offset;
	Elf32_Addr p_vaddr;
	Elf32_Addr p_paddr;
	Elf32_Word p_filesz;
	Elf32_Word p_memsz;
	Elf32_Word p_flags;
	Elf32_Word p_align;
}Elf32_Phdr;

void main(){
	FILE *fpIn ;
	ELFHeader Ehdr;
	int kernelsize=0;
	int magic=0;
	int dataoffset=0;
	unsigned int base = 0;
	unsigned int sizes[8] ={0};
	int i=0;
	Elf32_Phdr elf32_phdr[512];

	if( (fpIn = fopen("KERNEL.bin","rb+") ) == NULL ){
		printf("\n can not open file KERNEL.bin!");
		exit(0);
	}

	if(fread(&Ehdr,sizeof(ELFHeader),1,fpIn) < 0){
		printf("\n Read ELFHEADER error!");
		exit(0);
	}

	if(fread(&elf32_phdr,sizeof(Elf32_Phdr),Ehdr.e_phnum,fpIn) < 0){
		printf("\n Read Elf32_Phdr error!");
		exit(0);
	}

	for(i = 0;i < Ehdr.e_phnum; i++){
		printf("\n %d p_offset = %X",i,elf32_phdr[i].p_offset);
		printf("\n %d p_vaddr = %X",i,elf32_phdr[i].p_vaddr);
		printf("\n %d p_filesz = %X",i,elf32_phdr[i].p_filesz);
		printf("\n %d p_memsz = %X",i,elf32_phdr[i].p_memsz);
		if(base == 0) base=elf32_phdr[i].p_vaddr;
			base=MIN(base,elf32_phdr[i].p_vaddr);

		fseek(fpIn,elf32_phdr[i].p_offset,0);
		if(fread(&magic,sizeof(int),1,fpIn) < 0){
			printf("\n Read magic error!");
			exit(0);
		}
		printf("\n magic is %X",magic);
		if(magic = 0x526F){
			dataoffset = i;
		}
		kernelsize = MAX(kernelsize,elf32_phdr[i].p_vaddr + elf32_phdr[i].p_memsz);
	}
	sizes[0]=base >> CLICK_SHIFT;
	sizes[1]=(kernelsize - base + (CLICK_SIZE -1)) >> CLICK_SHIFT;
	fseek(fpIn,elf32_phdr[dataoffset].p_offset,0);

	if(fwrite(sizes,sizeof(sizes),1,fpIn) < 0){
			printf("\n write size error!");
			exit(0);
	}
	printf("\n kernelsize is %X,base is %X",kernelsize - base,base);
	printf("\n sizes[0]=%X , sizes[1]=%X, sizes[0]sh=%X, sizes[1]sh=%X",sizes[0],sizes[1],sizes[0] << CLICK_SHIFT,sizes[1] << CLICK_SHIFT );

	fclose(fpIn);
	printf("\nPress any key continue ...");
	getchar();
}