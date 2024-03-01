
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

#define Elf32_Addr u32
#define Elf32_Half u16
#define Elf32_Off  u32
#define Elf32_Sword  s32
#define Elf32_Word  u32

#define EI_NIDENT 16

typedef struct{
	unsigned char		e_ident[EI_NIDENT];
	Elf32_Half			e_type;
	Elf32_Half			e_machine;
	Elf32_Word			e_version;
	Elf32_Addr			e_entry;
	Elf32_Off			e_phoff;
	Elf32_Off			e_shoff;
	Elf32_Word			e_flags;
	Elf32_Half			e_ehsize;
	Elf32_Half			e_phentsize;
	Elf32_Half			e_phnum;
	Elf32_Half			e_shentsize;
	Elf32_Half			e_shnum;
	Elf32_Half			e_shstrndx;
}Elf32_Ehdr;


typedef struct{
	Elf32_Word		p_type;
	Elf32_Off		p_offset;
	Elf32_Addr		p_vaddr;
	Elf32_Addr		p_paddr;
	Elf32_Word		p_filesz;
	Elf32_Word		p_memsz;
	Elf32_Word		p_flags;
	Elf32_Word		p_align;
}Elf32_Phdr;





typedef struct {
	u32 BaseAddrLow;
	u32 BaseAddrHigh;
	u32 LengthLow;
	u32 LengthHigh;
	u32 type;
  
} MemoInfo;

/* Macros */
#define MAX(a,b)	(a > b ? a : b)
#define MIN(a,b)	(a < b ? a : b)

/* Type definitions */
typedef unsigned short unshort;	/* must be 16-bit unsigned */
typedef unshort block_nr;	/* block number */
#define NO_BLOCK (block_nr) 0	/* indicates the absence of a block number */
#define MAX_BLOCK_NR (block_nr) 0177777

typedef unshort inode_nr;	/* inode number */
#define NO_ENTRY (inode_nr) 0	/* indicates the absence of a dir entry */
#define MAX_INODE_NR (inode_nr) 0177777

typedef unshort zone_nr;	/* zone number */
#define NO_ZONE   (zone_nr) 0	/* indicates the absence of a zone number */
#define HIGHEST_ZONE (zone_nr)  0177777

typedef long zone_type;		/* zone size */
typedef unshort mask_bits;	/* mode bits */

typedef unshort bit_nr;		/* if inode_nr & zone_nr both unshort, then also unshort, else long */

typedef unshort dev_nr;		/* major | minor device number */
#define NO_DEV    (dev_nr) ~0	/* indicates absence of a device number */

typedef char links;		/* number of links to an inode */
#define MAX_LINKS 	0177

/* File property*/
typedef long real_time;		/* real time in seconds since Jan 1, 1980 */
typedef long file_pos;		/* position in, or length of, a file */
#define MAX_FILE_POS 017777777777L
typedef short int uid;		/* user id */
typedef char gid;		/* group id */

typedef unsigned vir_bytes;	/* virtual addresses and lengths in bytes */
typedef unsigned vir_clicks;	/* virtual addresses and lengths in clicks */
typedef unsigned phys_bytes;	/* physical addresses and lengths in bytes */
typedef unsigned phys_clicks;	/* physical addresses and lengths in clicks */
typedef int signed_clicks;	/* same length as phys_clicks, but signed */

/* Types relating to messages. */
#define M1                 1
#define M3                 3
#define M4                 4
#define M3_STRING         14

typedef struct {int m1i1, m1i2, m1i3; char *m1p1, *m1p2, *m1p3;} mess_1;
typedef struct {int m2i1, m2i2, m2i3; long m2l1, m2l2; char *m2p1;} mess_2;
typedef struct {int m3i1, m3i2; char *m3p1; char m3ca1[M3_STRING];} mess_3;
typedef struct {long m4l1, m4l2, m4l3, m4l4;} mess_4;
typedef struct {char m5c1, m5c2; int m5i1, m5i2; long m5l1, m5l2, m5l3;} mess_5;
typedef struct {int m6i1, m6i2, m6i3; long m6l1; int (*m6f1)();} mess_6;

typedef struct {
  int m_source;			/* who sent the message */
  int m_type;			/* what kind of message is it */
  union {
	mess_1 m_m1;
	mess_2 m_m2;
	mess_3 m_m3;
	mess_4 m_m4;
	mess_5 m_m5;
	mess_6 m_m6;
  } m_u;
} message;

#define MESS_SIZE (sizeof(message))
#define NIL_MESS (message *) 0

/* The following defines provide names for useful members. */
#define m1_i1  m_u.m_m1.m1i1
#define m1_i2  m_u.m_m1.m1i2
#define m1_i3  m_u.m_m1.m1i3
#define m1_p1  m_u.m_m1.m1p1
#define m1_p2  m_u.m_m1.m1p2
#define m1_p3  m_u.m_m1.m1p3

#define m2_i1  m_u.m_m2.m2i1
#define m2_i2  m_u.m_m2.m2i2
#define m2_i3  m_u.m_m2.m2i3
#define m2_l1  m_u.m_m2.m2l1
#define m2_l2  m_u.m_m2.m2l2
#define m2_p1  m_u.m_m2.m2p1

#define m3_i1  m_u.m_m3.m3i1
#define m3_i2  m_u.m_m3.m3i2
#define m3_p1  m_u.m_m3.m3p1
#define m3_ca1 m_u.m_m3.m3ca1


#define m4_l1  m_u.m_m4.m4l1
#define m4_l2  m_u.m_m4.m4l2
#define m4_l3  m_u.m_m4.m4l3
#define m4_l4  m_u.m_m4.m4l4

#define m5_c1  m_u.m_m5.m5c1
#define m5_c2  m_u.m_m5.m5c2
#define m5_i1  m_u.m_m5.m5i1
#define m5_i2  m_u.m_m5.m5i2
#define m5_l1  m_u.m_m5.m5l1
#define m5_l2  m_u.m_m5.m5l2
#define m5_l3  m_u.m_m5.m5l3

#define m6_i1  m_u.m_m6.m6i1
#define m6_i2  m_u.m_m6.m6i2
#define m6_i3  m_u.m_m6.m6i3
#define m6_l1  m_u.m_m6.m6l1
#define m6_f1  m_u.m_m6.m6f1

struct mem_map {
  vir_clicks mem_vir;		/* virtual address */
  phys_clicks mem_phys;		/* physical address */
  vir_clicks mem_len;		/* length */
};

