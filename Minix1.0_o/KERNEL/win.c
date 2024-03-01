
/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
                               system.c
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
                                                    Toby Du, 2014/8/22
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/


#include "../H/const.h"
#include "../H/type.h"
#include "../H/com.h"
#include "../H/error.h"
#include "../H/callnr.h"
#include "const.h"
#include "type.h"
#include "protect.h"
#include "proc.h"
#include "glo.h"

/* Command Block Registers */
/*	MACRO		PORT			DESCRIPTION			INPUT/OUTPUT	*/
/*	-----		----			-----------			------------	*/
#define REG_DATA	0x1F0		/*	Data				I/O		*/
#define REG_FEATURES	0x1F1		/*	Features			O		*/
#define REG_ERROR	REG_FEATURES	/*	Error				I		*/
#define REG_NSECTOR	0x1F2		/*	Sector Count			I/O		*/
#define REG_LBA_LOW	0x1F3		/*	Sector Number / LBA Bits 0-7	I/O		*/
#define REG_LBA_MID	0x1F4		/*	Cylinder Low / LBA Bits 8-15	I/O		*/
#define REG_LBA_HIGH	0x1F5		/*	Cylinder High / LBA Bits 16-23	I/O		*/
#define REG_DEVICE	0x1F6		/*	Drive | Head | LBA bits 24-27	I/O		*/
#define REG_STATUS	0x1F7		/*	Status				I		*/
#define REG_CMD		REG_STATUS	/*	Command				O		*/
#define REG_DEV_CTRL	0x3F6		/*	Device Control			O		*/
#define REG_ALT_STATUS	REG_DEV_CTRL	/*	Alternate Status		I		*/
#define REG_DRV_ADDR	0x3F7		/*	Drive Address			I		*/

/* Parameters for the disk drive. */
#define SECTOR_SIZE      512	/* physical sector size in bytes */

/* Error codes */
#define ERR		  -1	/* general error */

/* Miscellaneous. */
#define MAX_ERRORS         4	/* how often to try rd/wt before quitting */
#define NR_DEVICES        10	/* maximum number of drives */
#define MAX_WIN_RETRY  10000	/* max # times to try to output to WIN */
#define PART_TABLE     0x1C6	/* IBM partition table starts here in sect 0 */
#define DEV_PER_DRIVE      5	/* hd0 + hd1 + hd2 + hd3 + hd4 = 5 */


/***************/
/* DEFINITIONS */
/***************/
#define	HD_TIMEOUT		10000	/* in millisec */
#define	PARTITION_TABLE_OFFSET	0x1BE
#define ATA_IDENTIFY		0xEC
#define ATA_READ		0x20
#define ATA_WRITE		0x30
/* for DEVICE register. */
#define	MAKE_DEVICE_REG(lba,drv,lba_highest) (((lba) << 6) |		\
					      ((drv) << 4) |		\
					      (lba_highest & 0xF) | 0xA0)



PRIVATE message w_mess;		/* message buffer for in and out */

struct hd_cmd {
	u8	features;
	u8	count;
	u8	lba_low;
	u8	lba_mid;
	u8	lba_high;
	u8	device;
	u8	command;
};

/* Variables. */
PRIVATE struct wini {		/* main drive struct, one entry per drive */
  int wn_opcode;		/* DISK_READ or DISK_WRITE */
  int wn_procnr;		/* which proc wanted this operation? */
  int wn_drive;			/* drive number addressed */
  u8  lba_low;		/* cylinder number addressed */
  u8  lba_mid;		/* sector addressed */
  u8  lba_high;			/* head number addressed */
  int wn_heads;			/* maximum number of heads */
  int wn_maxsec;		/* maximum number of sectors per track */
  int wn_ctlbyte;		/* control byte (steprate) */
  int wn_precomp;		/* write precompensation cylinder / 4 */
  long wn_low;			/* lowest cylinder of partition */
  long wn_size;			/* size of partition in blocks */
  int wn_count;			/* byte count */
  vir_bytes wn_address;		/* user virtual address */
} wini[NR_DEVICES];

char buf[BLOCK_SIZE];  //test

/*****************************************************************************
 *                                hd_cmd_out
 *****************************************************************************/
/**
 * <Ring 1> Output a command to HD controller.
 * 
 * @param cmd  The command struct ptr.
 *****************************************************************************/
PRIVATE void hd_cmd_out(struct hd_cmd* cmd)
{
	/**
	 * For all commands, the host must first check if BSY=1,
	 * and should proceed no further unless and until BSY=0
	 */
	//if (!waitfor(STATUS_BSY, 0, HD_TIMEOUT))
	//	panic("hd error.");
	_lock();
	/* Activate the Interrupt Enable (nIEN) bit */
	out_byte(REG_DEV_CTRL, 0);
	/* Load required parameters in the Command Block Registers */
	out_byte(REG_FEATURES, cmd->features);
	out_byte(REG_NSECTOR,  cmd->count);
	out_byte(REG_LBA_LOW,  cmd->lba_low);
	out_byte(REG_LBA_MID,  cmd->lba_mid);
	out_byte(REG_LBA_HIGH, cmd->lba_high);
	out_byte(REG_DEVICE,   cmd->device);
	/* Write the command code to the Command Register */
	out_byte(REG_CMD,     cmd->command);
	_unlock();
}

/*===========================================================================*
 *				w_transfer				     * 
 *===========================================================================*/
PRIVATE int w_transfer(struct wini *wn)
//struct wini *wn;	/* pointer to the drive struct */
{
	struct hd_cmd cmd;
	//phys_bytes win_buf = umap(proc_addr(WINCHESTER), D, buf, BLOCK_SIZE);
	//phys_bytes usr_buf = umap(proc_addr(wn->wn_procnr), D, wn->wn_address, BLOCK_SIZE);
	phys_bytes win_buf = buf;
	phys_bytes usr_buf = wn->wn_address;
	int i,j;
	int r;

	cmd.features	= 0;
	cmd.count	= 2;
	cmd.lba_low	= wn->lba_low;
	cmd.lba_mid	= wn->lba_mid;
	cmd.lba_high = wn->lba_high;
	cmd.device	= MAKE_DEVICE_REG(1, 0, (0 >> 24) & 0xF);
	cmd.command	= (wn->wn_opcode == DISK_READ) ? ATA_READ : ATA_WRITE;

	if (wn->wn_opcode == DISK_WRITE)
	phys_copy(usr_buf, win_buf, (phys_bytes)BLOCK_SIZE);

	hd_cmd_out(&cmd);

	//if (com_out() != OK)
	//	return(ERR);
	receive(HARDWARE, &w_mess);
	/* Block, waiting for disk interrupt. */
	if (wn->wn_opcode == DISK_READ) {
		port_read(REG_DATA, buf, BLOCK_SIZE);
		//if (win_results() != OK) {
			//w_need_reset = TRUE;
		//	return(ERR);
		//}
		phys_copy(win_buf, usr_buf, (phys_bytes)BLOCK_SIZE);
		r = OK;
	}
	else{
		
		//port_write(REG_DATA, tmp, 1024);
		r = OK;
	}
	return r;
}

/*===========================================================================*
 *				w_do_rdwt					     * 
 *===========================================================================*/
PRIVATE int w_do_rdwt(message *m_ptr)
//message *m_ptr;			/* pointer to read or write w_message */
{
/* Carry out a read or write request from the disk. */
	struct wini *wn;
	int r, device, errors = 0;
	long sector;

	/* Decode the w_message parameters. */
	device = m_ptr->DEVICE;
	if (device < 0 || device >= NR_DEVICES)
		return(EIO);
	//if (m_ptr->COUNT != BLOCK_SIZE)
	//	return(EINVAL);
	wn = &wini[device];		/* 'wn' points to entry for this drive */
	wn->wn_drive = device/DEV_PER_DRIVE;	/* save drive number */
	//if (wn->wn_drive >= nr_drives)
	//return(EIO);
	wn->wn_opcode = m_ptr->m_type;	/* DISK_READ or DISK_WRITE */
	if (m_ptr->POSITION % BLOCK_SIZE != 0)
		return(EINVAL);
	sector = m_ptr->POSITION/SECTOR_SIZE;
	if ((sector+BLOCK_SIZE/SECTOR_SIZE) > wn->wn_size)
		return(EOF);
	//sector += wn->wn_low;
	//wn->wn_cylinder = sector / (wn->wn_heads * wn->wn_maxsec);
	//wn->wn_sector =  (sector % wn->wn_maxsec) + 1;
	//wn->wn_head = (sector % (wn->wn_heads * wn->wn_maxsec) )/wn->wn_maxsec;
	//wn->wn_count = m_ptr->COUNT;
	//wn->wn_address = (vir_bytes) m_ptr->ADDRESS;
	//wn->wn_procnr = m_ptr->PROC_NR;

	/* This loop allows a failed operation to be repeated. */
  while (errors <= MAX_ERRORS) {
	errors++;		/* increment count once per loop cycle */
	if (errors > MAX_ERRORS)
		return(EIO);
	/* Perform the transfer. */
	r = w_transfer(wn);
	if (r == OK) break;	/* if successful, exit loop */

  }

  return(r == OK ? BLOCK_SIZE : EIO);

}

PUBLIC void init_win();
/*===========================================================================*
 *				winchester_task				     * 
 *===========================================================================*/
PUBLIC void winchester_task()
{
/* Main program of the winchester disk driver task. */

  int r, caller, proc_nr;

	struct hd_cmd cmd;
	//cmd.features	= 0;
	//cmd.count	= 2;
	//cmd.lba_low	= 0;
	//cmd.lba_mid	= 0;
	//cmd.lba_high	= 0;
	//cmd.device	= MAKE_DEVICE_REG(1, 0, (0 >> 24) & 0xF);
	//cmd.command	= ATA_READ;
	//hd_cmd_out(&cmd);
	//delay(1);
	//port_read(REG_DATA, tmp, 1024);

	printk(" win task  ");

  init_win();
  while (TRUE) {
	  /* First wait for a request to read or write a disk block. */
	receive(ANY, &w_mess);	/* get a request to do some work */
	if (w_mess.m_source < 0) {
		printk(" \n winchester task got message from %d ", w_mess.m_source);
		continue;
	}
	caller = w_mess.m_source;
	proc_nr = w_mess.PROC_NR;
	/* Now carry out the work. */
	switch(w_mess.m_type) {
	    case DISK_READ:
	    case DISK_WRITE:	r = w_do_rdwt(&w_mess);	break;
	    default:		r = EINVAL;		break;
	}

	/* Finally, prepare and send the reply message. */
	w_mess.m_type = TASK_REPLY;	
	w_mess.REP_PROC_NR = proc_nr;

	w_mess.REP_STATUS = r;	/* # of bytes transferred or error code */
	send(caller, &w_mess);	/* send reply to caller */
  }

}


/*======================================================================*
                           clock_handler
 *======================================================================*/
PUBLIC void win_handler(int irq)
{	printk(" wini int ");
	int_mess.m_type = DISKINT;
	interrupt(WINCHESTER,&int_mess);	
	return;
}


/*======================================================================*
                           init_clock
 *======================================================================*/
PUBLIC void init_win()
{
	put_irq_handler(AT_WINI_IRQ, win_handler);	/* 设定时钟中断处理程序 */
	enable_irq(AT_WINI_IRQ);	/* 磁盘读写 */
	return;
}




