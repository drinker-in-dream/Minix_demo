/* This file handles the EXEC system call.  It performs the work as follows:
 *    - see if the permissions allow the file to be executed
 *    - read the header and extract the sizes
 *    - fetch the initial args and environment from the user space
 *    - allocate the memory for the new process
 *    - copy the initial stack from MM to the process
 *    - read in the text and data segments and copy to the process
 *    - take care of setuid and setgid bits
 *    - fix up 'mproc' table
 *    - tell kernel about EXEC
 *
 *   The only entry point is do_exec.
 */

#include "../H/const.h"
#include "../H/type.h"
#include "../H/callnr.h"
#include "../H/error.h"
#include "../H/stat.h"
#include "const.h"
#include "glo.h"
#include "mproc.h"
#include "param.h"

#define MAGIC    0x464C457F	/* magic number with 2 bits masked off */
#define SEP      0x00200000L	/* value for separate I & D */
#define TEXTB              2	/* location of text size in header */
#define DATAB              3	/* location of data size in header */
#define BSSB               4	/* location of bss size in header */
#define TOTB               6	/* location of total size in header */


/*===========================================================================*
 *				patch_ptr				     *
 *===========================================================================*/
PRIVATE void patch_ptr(char stack[MAX_ISTACK_BYTES], vir_bytes base)
//char stack[MAX_ISTACK_BYTES];	/* pointer to stack image within MM */
//vir_bytes base;			/* virtual address of stack base inside user */
{
/* When doing an exec(name, argv, envp) call, the user builds up a stack
 * image with arg and env pointers relative to the start of the stack.  Now
 * these pointers must be relocated, since the stack is not positioned at
 * address 0 in the user's address space.
 */

}

/*===========================================================================*
 *				new_mem					     *
 *===========================================================================*/
PRIVATE int new_mem(vir_bytes text_bytes, vir_bytes data_bytes, vir_bytes bss_bytes,vir_bytes stk_bytes,phys_bytes tot_bytes,char bf[ZEROBUF_SIZE],int zs)
//vir_bytes text_bytes;		/* text segment size in bytes */
//vir_bytes data_bytes;		/* size of initialized data in bytes */
//vir_bytes bss_bytes;		/* size of bss in bytes */
//vir_bytes stk_bytes;		/* size of initial stack segment in bytes */
//phys_bytes tot_bytes;		/* total memory to allocate, including gap */
//char bf[ZEROBUF_SIZE];		/* buffer to use for zeroing data segment */
//int zs;				/* true size of 'bf' */
{
	struct mproc *rmp;
	char *rzp;
	vir_bytes vzb;
	vir_clicks text_clicks, data_clicks, gap_clicks, stack_clicks, tot_clicks;
	phys_clicks new_base, old_clicks;
	phys_bytes bytes, base, count, bss_offset;
	extern phys_clicks alloc_mem();
	extern phys_clicks max_hole();

	/* Check to see if there is a hole big enough.  If so, we can risk first
     * releasing the old core image before allocating the new one, since we
     * know it will succeed.  If there is not enough, return failure.
     */
    //if (text_clicks + tot_clicks > max_hole()) return(EAGAIN);
	
	/* There is enough memory for the new core image.  Release the old one. */
     rmp = mp;


	printk("here in  new mem\n");
	return OK;
}

/*===========================================================================*
 *				do_exec					     *
 *===========================================================================*/
PUBLIC int do_exec()
{
/* Perform the exece(name, argv, envp) call.  The user library builds a
 * complete stack image, including pointers, args, environ, etc.  The stack
 * is copied to a buffer inside MM, and then to the new core image.
 */
    struct mproc *rmp;
    int m, r, fd, ft;
    char mbuf[MAX_ISTACK_BYTES];	/* buffer for stack and zeroes */
    union u {
	  char name_buf[MAX_PATH];	/* the name of the file to exec */
	  char zb[ZEROBUF_SIZE];	/* used to zero bss */
    } u;
    char *new_sp;
    vir_bytes src, dst, text_bytes, data_bytes, bss_bytes, stk_bytes, vsp;
	Elf32_Ehdr hdr;
    phys_bytes tot_bytes;		/* total space for program, including gap */
    vir_clicks sc;
    struct stat s_buf;
    
	/* Do some validity checks. */
    rmp = mp;
    stk_bytes = (vir_bytes) stack_bytes;
	printk("stk_bytes:%d \n",stk_bytes);
    if (stk_bytes > MAX_ISTACK_BYTES) return(ENOMEM);	/* stack too big */
    if (exec_len <= 0 || exec_len > MAX_PATH) return(EINVAL);

	/* Get the exec file name and see if the file is executable. */
	src = (vir_bytes) exec_name;
	dst = (vir_bytes) u.name_buf;
	r = mem_copy(mm_who, D, (long) src, MM_PROC_NR, D, (long) dst, (long) exec_len);
    if (r != OK) return(r);	/* file name not in user data segment */
	tell_fs(CHDIR, mm_who, 0, 0);	/* temporarily switch to user's directory */
	fd = allowed(u.name_buf, &s_buf, X_BIT);	/* is file executable? */
	tell_fs(CHDIR, 0, 1, 0);	/* switch back to MM's own directory */
    if (fd < 0) return(fd);	/* file was not executable */
	/* Read the file header and extract the segment sizes. */
    sc = (stk_bytes + CLICK_SIZE - 1) >> CLICK_SHIFT;
    m = read_header(fd, &hdr ,sc);
	if (m < 0) {
		close(fd);		/* something wrong with header */
		return(ENOEXEC);
	}
    /* Fetch the stack from the user before destroying the old core image. */
	src = (vir_bytes) stack_ptr;
	dst = (vir_bytes) mbuf;
	r = mem_copy(mm_who, D, (long) src, MM_PROC_NR, D, (long) dst, (long) stk_bytes);
	if (r != OK) {
		close(fd);		/* can't fetch stack (e.g. bad virtual addr) */
		return(EACCES);
	}
    
    /* Allocate new memory and release old memory.  Fix map and tell kernel. */
    r = new_mem(text_bytes, data_bytes, bss_bytes, stk_bytes, tot_bytes,
							u.zb, ZEROBUF_SIZE);
    if (r != OK) {
	  close(fd);		/* insufficient core or program too big */
	  return(r);
    }

	printk("here in do_exec %d \n",fd);
	return OK;
}

/*===========================================================================*
 *				read_header				     *
 *===========================================================================*/
PRIVATE int read_header(int fd, Elf32_Ehdr *hdr,vir_clicks sc)
//int fd;				/* file descriptor for reading exec file */
//vir_clicks sc;			/* stack size in clicks */
{
/* Read the header and extract the text, data, bss and total sizes from it. */
      int m, ct;
	  vir_clicks tc, dc, s_vir, dvir;
	  phys_clicks totc;
      long buf[HDR_SIZE/sizeof(long)];

	  if (read(fd, hdr, sizeof(Elf32_Ehdr)) != sizeof(Elf32_Ehdr)) return(ENOEXEC);
      if ( *(int*)(&hdr->e_ident[0]) != MAGIC) return(ENOEXEC);

      //*ft = (buf[0] & SEP ? SEPARATE : 0);	/* separate I & D or not */
	  
	  /* Get text and data sizes. */
      //*text_bytes = (vir_bytes) buf[TEXTB];	/* text size in bytes */
      //*data_bytes = (vir_bytes) buf[DATAB];	/* data size in bytes */
      //if (*ft != SEPARATE) {
	  /* If I & D space is not separated, it is all considered data. Text=0 */
	  //  *data_bytes += *text_bytes;
	  //  *text_bytes = 0;
      //}

	  /* Get bss and total sizes. */
     // *bss_bytes = (vir_bytes) buf[BSSB];	/* bss size in bytes */
      //*tot_bytes = buf[TOTB];	/* total bytes to allocate for program */
      //if (*tot_bytes == 0) return(ENOEXEC);

	  /* Check to see if segment sizes are feasible. */
      //tc = (*text_bytes + CLICK_SHIFT - 1) >> CLICK_SHIFT;
      //dc = (*data_bytes + *bss_bytes + CLICK_SHIFT - 1) >> CLICK_SHIFT;
      //totc = (*tot_bytes + CLICK_SIZE - 1) >> CLICK_SHIFT;
      //if (dc >= totc) return(ENOEXEC);	/* stack must be at least 1 click */
      //dvir = (*ft == SEPARATE ? 0 : tc);
      //s_vir = dvir + (totc - sc);
	  //m = size_ok(*ft, tc, dc, sc, dvir, s_vir);
      //ct = buf[1] & BYTE;		/* header length */
      //if (ct > HDR_SIZE) read(fd, buf, ct - HDR_SIZE);	/* skip unused hdr */
      //return(m);
	  return OK;
}

/*===========================================================================*
 *				load_seg				     *
 *===========================================================================*/
PRIVATE void load_seg(int fd, int seg, vir_bytes seg_bytes)
//int fd;				/* file descriptor to read from */
//int seg;			/* T or D */
//vir_bytes seg_bytes;		/* how big is the segment */
{
	
}




