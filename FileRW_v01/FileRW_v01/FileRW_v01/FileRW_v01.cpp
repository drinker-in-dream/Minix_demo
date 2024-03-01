// FileRW_v01.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "const.h"
#include "fsconst.h"
#include "type.h"
#include "fstype.h"

#define BITSHIFT	  5	/* = 2log(#bits(int)) */
#define BITMAPSHIFT	 13	/* = 2log(#bits(block)); 13 means 1K blocks */
#define BLOCKSHIFT	 10	/* = 2log(#bytes(block)); 10 means 1K blocks */
#define SECT_SHIFT        9	/* sectors are 512 bytes */
#define SECTOR_SIZE (1<<SECT_SHIFT)	/* bytes in a sector */
#define BITMASK		((1 << BITSHIFT) - 1)
#define MAXDIRSIZE     5000	/* max. size of a reasonable directory */
#define CINDIR		128	/* number of indirect zno's read at a time */
#define CDIRECT		 16	/* number of dir entries read at a time */

/* ztob gives the block address of a zone
 * btoa gives the byte address of a block
 */
#define ztob(z)		((block_nr) (z) << sb.s_log_zone_size)
#define btoa(b)		((long) (b) * BLOCK_SIZE)
#define SCALE		((int) ztob(1))		/* # blocks in a zone */
#define FIRST		sb.s_firstdatazone	/* as the name says */

/* # blocks of each type */
#define N_SUPER		1
#define N_IMAP		(sb.s_imap_blocks)
#define N_ZMAP		(sb.s_zmap_blocks)
#define N_ILIST		((sb.s_ninodes+INODES_PER_BLOCK-1) / INODES_PER_BLOCK)
#define N_DATA		(sb.s_nzones - FIRST + 1)

/* block address of each type */
#define BLK_SUPER	(SUPER_BLOCK)
#define BLK_IMAP	(BLK_SUPER + N_SUPER)
#define BLK_ZMAP	(BLK_IMAP  + N_IMAP)
#define BLK_ILIST	(BLK_ZMAP  + N_ZMAP)
#define BLK_FIRST	ztob(FIRST)
#define ZONE_SIZE	((int) ztob(BLOCK_SIZE))
#define NLEVEL		(NR_ZONE_NUMS - NR_DZONE_NUM + 1)

#define setbit(w, b)	(w[(b) >> BITSHIFT] |= 1 << ((b) & BITMASK))
#define clrbit(w, b)	(w[(b) >> BITSHIFT] &= ~(1 << ((b) & BITMASK)))
#define bitset(w, b)	(w[(b) >> BITSHIFT] & (1 << ((b) & BITMASK)))

/* byte address of a zone/of an inode */
#define zaddr(z)	btoa(ztob(z))
#define inoaddr(i)	((long) (i - 1) * INODE_SIZE + btoa(BLK_ILIST))
#define INDCHUNK	(CINDIR * ZONE_NUM_SIZE)
#define DIRCHUNK	(CDIRECT * DIR_ENTRY_SIZE)

/* Supper Block */
struct dsb {
	inode_nr s_ninodes;		/* # inodes on the minor device (total inode number)*/ 
	zone_nr s_nzones;		/* total dev size, incl. bit maps etc (total block number)*/
	unsigned short s_imap_blocks;	/* # of blocks used by inode bit map */
	unsigned short s_zmap_blocks;	/* # of blocks used by zone bit map */
	zone_nr s_firstdatazone;	/* number of first data zone */
	short s_log_zone_size;		/* log2 of blocks/zone */
	file_pos s_maxsize;		/* maximum file size on this device */
	u16 s_magic;			/* magic number for super blocks */
} sb;

//----------->> V A R I A B L E    S E T U P <<-----------//
FILE *fpIn ; /* Point to image file*/
char *rwbuf;			/* one block buffer cache */
char  rwbuf1[BLOCK_SIZE]={0};	/* in case of a DMA-overrun under DOS .. */
char  rwbuf2[BLOCK_SIZE]={0};	/* .. an other buffer can be used */
char nullbuf[BLOCK_SIZE]={0};	/* null buffer */
unsigned int *imap;	/* inode bit maps */
unsigned int *zmap;	/* zone bit maps */
d_inode rootinode;
d_inode fileinode;
dir_struct filedir;		
char name[128]={0};

/* Open the image*/
int devopen(const char *f){
	if( (fpIn = fopen(f,"rb+") ) == NULL ){
		printf("\n can not open file Minix.bin!\n");
		return -1;
	}
	return 0;
}
/* Close the image*/
int devclose(){
	fclose(fpIn);
	return 0;
}

/* Copy n bytes.
 */
void copy(char *p, char *q, u64 n){
	do
		*q++ = *p++;
	while (--n);
}

/* Handle disk Read & Write
 */
int diskio(int dir, u64 sector_number, char *buffer, u64 sector_count){
	u64 startoffset=0;
	startoffset = sector_number * SECTOR_SIZE;		
	fseek(fpIn,startoffset,0);
	if(dir == READING){
		if(fread(buffer,SECTOR_SIZE,sector_count,fpIn) < 0){
				printf("Read file Error");
				return -1;
		}
	}else if(dir == WRITING){
		if(fwrite(buffer,SECTOR_SIZE,sector_count,fpIn) < 0){
				printf("Write file Error");
				return -1;
		}
	}else{
		printf("parameter error!");
		return -1;
	}
	return 0;
}

/* Read or write a block.
 */
int devio(block_nr bno, int dir){
	u64 lastone;
	u64 offset = btoa(bno);
	int sector = offset >> SECT_SHIFT;
	int error;

	lastone = sector  + (BLOCK_SIZE>>SECT_SHIFT);
	if (lastone > 0xffffffffffffffff) {
		printf("Fsck cannot read beyond sector 0xffffffffffffffff\n");
		exit(1);
	}
	error = diskio(dir, sector, rwbuf, BLOCK_SIZE>>SECT_SHIFT);
	if ((error & 0xFF00) == 0){
		return 0;
	}
	else{
		printf("devio error!!! \n");
		return -1;
	}
}


/* Read `size' bytes from the disk starting at byte `offset'.
 */
void devread(u64 offset, char *buf, int size){
	int i;
	int pos;
	//devio((block_nr) (offset / BLOCK_SIZE), READING);
	//copy(&rwbuf[offset % BLOCK_SIZE], buf, size);
	i = (offset % BLOCK_SIZE + size + (BLOCK_SIZE -1))/BLOCK_SIZE;
	pos=0;
	do{
		devio((block_nr) (offset / BLOCK_SIZE), READING);
		copy(&rwbuf[offset % BLOCK_SIZE], &buf[pos], 
			(size <= (BLOCK_SIZE - offset % BLOCK_SIZE) ? size:(BLOCK_SIZE - offset % BLOCK_SIZE)) );
		if(i > 1){
			pos += (size <= (BLOCK_SIZE - offset % BLOCK_SIZE) ? size:(BLOCK_SIZE - offset % BLOCK_SIZE));
			offset += (BLOCK_SIZE - (offset % BLOCK_SIZE));
			size -= (size <= (BLOCK_SIZE - offset % BLOCK_SIZE) ? size:(BLOCK_SIZE - offset % BLOCK_SIZE));
		}
	}while(--i > 0);
}

/* Write `size' bytes to the disk starting at byte `offset'.
 */
void devwrite(u64 offset, char *buf, int size){
	//if (size != BLOCK_SIZE)
	//	devio((block_nr) (offset / BLOCK_SIZE), READING);
	//copy(buf, &rwbuf[offset % BLOCK_SIZE], size);
	//devio((block_nr) (offset / BLOCK_SIZE), WRITING);
	int i;
	int pos;
	i = (offset % BLOCK_SIZE + size + (BLOCK_SIZE -1))/BLOCK_SIZE;
	pos=0;
	do{
		if ( (size <= (BLOCK_SIZE - offset % BLOCK_SIZE) ? size:(BLOCK_SIZE - offset % BLOCK_SIZE)) != BLOCK_SIZE )
			devio((block_nr) (offset / BLOCK_SIZE), READING);
		copy(&buf[pos], &rwbuf[offset % BLOCK_SIZE], 
			(size <= (BLOCK_SIZE - offset % BLOCK_SIZE) ? size:(BLOCK_SIZE - offset % BLOCK_SIZE)) );
		devio((block_nr) (offset / BLOCK_SIZE), WRITING);
		if(i > 1){
			pos += (size <= (BLOCK_SIZE - offset % BLOCK_SIZE) ? size:(BLOCK_SIZE - offset % BLOCK_SIZE));
			offset += (BLOCK_SIZE - (offset % BLOCK_SIZE));
			size -= (size <= (BLOCK_SIZE - offset % BLOCK_SIZE) ? size:(BLOCK_SIZE - offset % BLOCK_SIZE));
		}

	}while(--i > 0);


}

/* Get loader.bin size */
unsigned long get_file_size(FILE* fp){
    unsigned long size;
    fseek( fp, 0L, SEEK_END );
    size=ftell(fp);
    return size;
}

/* find next void bit number*/
bit_nr findvoidbit(unsigned int * bitmap){
	bit_nr i=1;
	do{
		i+=1;
	}while(bitset(bitmap, i));
	return i;
}

/* Get supper block */
int getsuper(){
	devread(btoa(BLK_SUPER), (char *) &sb, sizeof(sb));
	return 0;
}

int writedirzone(file_pos *pos,zone_nr zno,int level){
	return 0;
}

int writeindzone(file_pos *pos,zone_nr zno,int level){
	return 0;
}

int zonewrite(file_pos *pos, zone_nr zno, int level){
	if(level == 0){
		writedirzone(pos,zno,level);
		return 0;
	}
	else
		writeindzone(pos,zno,level);
	return 0;
}

int writezones(d_inode *ip,zone_nr *zlist,int len, int level){
	int i;
	file_pos pos;
	for (i = 0,pos = 0; i < len && pos < ip->i_size; i++){
		if (zlist[i] == NO_ZONE)
			return 0;
		else
			zonewrite(&pos, zlist[i], level);		
	}
	return 0;
}

void readzones(d_inode *ip){
	return;
}

/* Allocate `nblk' blocks worth of bitmap.
 */
unsigned int *allocbitmap(u32 nblk){
	unsigned int *bitmap;
	bitmap = (unsigned *) calloc(nblk, BLOCK_SIZE);
	return(bitmap);
}

/* Get all the bitmaps used by this program.
 */
void getbitmaps(){
	imap = allocbitmap(N_IMAP);
	zmap = allocbitmap(N_ZMAP);
	devread(btoa(BLK_IMAP), (char *)imap, N_IMAP * BLOCK_SIZE);
	devread(btoa(BLK_ZMAP), (char *)zmap, N_ZMAP * BLOCK_SIZE);
}

void syncbitmaps(){
	devwrite(btoa(BLK_IMAP), (char *)imap, N_IMAP * BLOCK_SIZE);
	devwrite(btoa(BLK_ZMAP), (char *)zmap, N_ZMAP * BLOCK_SIZE);
}

int clearzones( zone_nr *zlist, int len, int level);
void indzoneclear( zone_nr *zlist, int level){
	zone_nr indirect[BLOCK_SIZE];
	int n = NR_INDIRECTS / CINDIR;
	long offset = zaddr(((*zlist) + FIRST));

	clrbit(zmap, (*zlist));
	(*zlist) = NO_ZONE;
	devread(offset, (char *) indirect, BLOCK_SIZE);
	clearzones(indirect, NR_INDIRECTS, level - 1);
	devwrite(offset, (char *) indirect, BLOCK_SIZE);
}

void zoneclear( zone_nr *zlist, int level){
	if(level == 0){
		clrbit(zmap, (*zlist));
		(*zlist) = NO_ZONE;
	}
	else
		indzoneclear(zlist, level);
}

int clearzones(zone_nr *zlist, int len, int level){
	int  i;
	for (i = 0; i < len ; i++)
		if (zlist[i] == NO_ZONE)
			return YES;
		else{
			zoneclear( &zlist[i], level);				
		}
	return NO;
}

void clearinode(inode_nr ino){
	d_inode tmpinode;
	file_pos pos =0;
	int i,level;

	devread(inoaddr(ino), (char *) &tmpinode, INODE_SIZE);
	tmpinode.i_size = 0;
	clearzones(&tmpinode.i_zone[0], NR_DZONE_NUM, 0);
	for (i = NR_DZONE_NUM, level = 1; i < NR_ZONE_NUMS; i++, level++)
		if(clearzones(&tmpinode.i_zone[i], 1, level))
			break;
	copy((char *) &tmpinode, (char *) &fileinode, INODE_SIZE);
}

int checkexistfile(){
	dir_struct dirblk[CDIRECT];
	dir_struct *dp;
	int n = SCALE * (NR_DIR_ENTRIES / CDIRECT);
	int pos = 0;
	int offset;
	offset=zaddr(rootinode.i_zone[0]);

	printf("root file size %d",rootinode.i_size);
	do{
		devread(offset, (char *) dirblk, DIRCHUNK);
		for (dp = dirblk; dp < &dirblk[CDIRECT]; dp++){
			if(!strcmp(filedir.d_name,dp->d_name)){
				filedir.d_inum = dp->d_inum;
				clearinode(dp->d_inum);
				printf("\n clear inode num:%d",dp->d_inum);
				return YES;
			}

			if ((pos += DIR_ENTRY_SIZE) >= rootinode.i_size)
				return NO;
		}
		offset += DIRCHUNK;
	}while(--n && pos < rootinode.i_size);

	return NO;
}
int getfilezones(file_pos *pos,zone_nr *zlist,int len, int level);
void indzoneget(file_pos *pos,zone_nr *zlist, int level){
	zone_nr indirect[BLOCK_SIZE] = {0};

	(*zlist) = findvoidbit(zmap);
	printf("\n ind zone: %d ",(*zlist));
	setbit(zmap, *zlist);
	getfilezones(pos,indirect,NR_INDIRECTS, level -1);
	devwrite(zaddr( ((*zlist) + FIRST) ), (char *) indirect, BLOCK_SIZE);
}

void zoneget(file_pos *pos,zone_nr *zlist, int level){
	if(level == 0){
		(*pos) -= BLOCK_SIZE;
		(*zlist) = findvoidbit(zmap);
		setbit(zmap, *zlist);
		printf("\n zone: %d",(*zlist));
	}
	else
		indzoneget(pos,zlist,level);
}

int getfilezones(file_pos *pos,zone_nr *zlist,int len, int level){
	int  i;
	for (i = 0; i < len ; i++)
		if((*pos) <= 0)
			return YES;
		else{
			zoneget(pos,&zlist[i],level);
		}
	return NO;
}

void fillinodezone(inode_nr ino,file_pos pos){
	int i,level;
	fileinode.i_size = pos;
	getfilezones(&pos,&fileinode.i_zone[0],NR_DZONE_NUM, 0);
	for (i = NR_DZONE_NUM, level = 1; i < NR_ZONE_NUMS; i++, level++)
		if (getfilezones(&pos,&fileinode.i_zone[i],1, level) )
			return;
}

int contentzones(file_pos *pos,char * block,zone_nr *zlist,int len, int level,FILE *fp);
void indzonecontent(file_pos *pos,char * block,zone_nr *zlist,int level,FILE *fp){
	zone_nr indirect[BLOCK_SIZE];
	printf("\n inode contentzone %d",((*zlist)));
	devread(zaddr( ((*zlist) + FIRST) ), (char *) indirect, BLOCK_SIZE);
	contentzones(pos,block,indirect,NR_INDIRECTS, level - 1,fp);
}

void zonecontent(file_pos *pos,char * block,zone_nr *zlist,int level,FILE *fp){
	if(level == 0){
		memset((char *)block,0,BLOCK_SIZE);
		fread(block,BLOCK_SIZE,1,fp);
		devwrite(zaddr((FIRST + (*zlist))), block, BLOCK_SIZE);
		(*pos) -= BLOCK_SIZE;
	}
	else
		indzonecontent(pos,block,zlist,level,fp);
}

int contentzones(file_pos *pos,char * block,zone_nr *zlist,int len, int level,FILE *fp){
	int  i;
	for (i = 0; i < len ; i++)
		if((*pos) <= 0)
			return YES;
		else{
			zonecontent(pos,block,&zlist[i],level,fp);
			printf("\n content zone %d",zlist[i]);
		}
	return NO;
}

void writecontent(inode_nr ino,FILE *fp){
	file_pos fpos=0;
	int i,level;
	char tmpblk[BLOCK_SIZE];
	devread(inoaddr(filedir.d_inum), (char *) &fileinode, INODE_SIZE);
	fpos=fileinode.i_size;
	printf("\n filesize: %d",fpos);
	fseek(fp,0,0);
	contentzones(&fpos,tmpblk,&fileinode.i_zone[0],NR_DZONE_NUM, 0,fp);
	for (i = NR_DZONE_NUM, level = 1; i < NR_ZONE_NUMS; i++, level++)
		if (contentzones(&fpos,tmpblk,&fileinode.i_zone[i],1, level,fp))
			return;

}

/* Write file to image */
int writefile(FILE *fp,const char * filename){
	FILE *fpOut ;
	sprintf(name,"%S",filename);
	if( (fpOut = fopen(name,"rb") ) == NULL ){
		printf("\n can not open file %s!",name);
		exit(0);
	}
	filedir.d_inum = 0;
	strcpy(filedir.d_name,name);
	
	if(!checkexistfile()){
		filedir.d_inum = findvoidbit(imap);
		setbit(imap,filedir.d_inum);
		fileinode.i_mode = I_REGULAR | 0755;
		fileinode.i_uid = 0;
		fileinode.i_modtime = 0;
		fileinode.i_gid = 0;
		fileinode.i_nlinks =1;
		printf(" \n filedir.d_inum = %d",filedir.d_inum);
		devwrite(zaddr(FIRST) + rootinode.i_size, (char *)&filedir, sizeof(filedir));
		rootinode.i_size += sizeof(filedir);
		rootinode.i_nlinks++;
		devwrite(inoaddr(ROOT_INODE), (char *) &rootinode, INODE_SIZE);
	}
	printf("\nfile inode:%d",filedir.d_inum);
	fillinodezone(filedir.d_inum,get_file_size(fpOut));
	devwrite(inoaddr(filedir.d_inum), (char *) &fileinode, INODE_SIZE);
	writecontent(filedir.d_inum,fpOut);
	printf("\n kernel size %d",get_file_size(fpOut));
	fclose(fpOut);
	return 0;
}

void readrootinode(){
	devread(inoaddr(ROOT_INODE), (char *) &rootinode, INODE_SIZE);
}

int _tmain(int argc, _TCHAR* argv[])
{
	rwbuf =rwbuf1;
	devopen("Minix.bin");
	getsuper();
	readrootinode();
	getbitmaps();
	writefile(fpIn,(const char *)argv[1]);
	syncbitmaps();
	devclose();
	return 0;
}

