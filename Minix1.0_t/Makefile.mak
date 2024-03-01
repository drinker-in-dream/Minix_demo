
##################################
# Makefile for Minix             #
# make -f Makefile.mak           #
##################################

# Entry point of Minix
# It must be as same as 'KernelEntryPointPhyAddr' !!!
ENTRYPOINT	= 0x30400
MMENTRYPOINT    = 0x60400
FSENTRYPOINT    = 0x70400
INITENTRYPOINT  = 0x80400

# Offset of entry point in kernel file
# It depends on ENTRYPOINT
ENTRYOFFSET	=   0x400

# Programs, flags, etc.
ASM		= nasm
CC		= gcc
LD		= ld
ASMBFLAGS	= -I ./BOOT/INCLUDE 
ASMLFLAGS	= -I  ./LOADER/INCLUDE
ASMKFLAGS	= -I ./KERNEL -f elf
CFLAGS		= -I ./INCLUDE -c -fno-builtin
LDFLAGS		= -s -Ttext $(ENTRYPOINT)
INITLDFLAGS	= -s -Ttext $(INITENTRYPOINT)
MMLDFLAGS	= -s -Ttext $(MMENTRYPOINT)
FSLDFLAGS	= -s -Ttext $(FSENTRYPOINT)

# This Program
MinixBOOT	= ./BUILD/BOOT.bin ./BUILD/LOADER.bin
MinixKERNEL	= ./BUILD/KERNEL.bin
MINIXINIT       = ./BUILD/INIT.bin
MINIXMM         = ./BUILD/MM.bin
MINIXFS         = ./BUILD/FS.bin
LIB             = ./BUILD/libc.a

OBJS		=  ./BUILD/KERNEL.o ./BUILD/main.o ./BUILD/klibx86.o ./BUILD/table.o \
  ./BUILD/protect.o ./BUILD/i8259.o ./BUILD/start.o ./BUILD/proc.o ./BUILD/clock.o \
  ./BUILD/system.o  ./BUILD/tty.o  ./BUILD/printk.o ./BUILD/win.o  ./BUILD/fs_main.o \
  ./BUILD/memory.o ./BUILD/mm_main.o ./BUILD/table_mm.o ./BUILD/put_mm.o ./BUILD/getset.o \
  ./BUILD/inode.o ./BUILD/tablefs.o ./BUILD/cache.o ./BUILD/device.o  ./BUILD/pipe.o \
  ./BUILD/read.o ./BUILD/super.o ./BUILD/filedes.o  ./BUILD/fsput.o ./BUILD/time.o \
  ./BUILD/path.o ./BUILD/protectfs.o ./BUILD/utility.o ./BUILD/write.o ./BUILD/link.o \
  ./BUILD/misc.o ./BUILD/mount.o ./BUILD/open_fs.o ./BUILD/stadir.o ./BUILD/utility_mm.o \
  ./BUILD/alloc.o ./BUILD/break.o ./BUILD/exec.o ./BUILD/forkexit.o ./BUILD/signal_mm.o 
   
  
INITOBJS =  ./INIT/BUILD/init.o ./INIT/BUILD/initc.o  
        

LIBOBJS	  =  ./BUILD/sendrec.o ./BUILD/message.o ./BUILD/call.o ./BUILD/fork.o ./BUILD/wait.o \
	     ./BUILD/abs.o ./BUILD/atoi.o ./BUILD/ctype.o ./BUILD/atol.o ./BUILD/bcopy.o \
	     ./BUILD/itoa.o ./BUILD/strcat.o ./BUILD/strcmp.o ./BUILD/strcpy.o ./BUILD/strlen.o \
	     ./BUILD/strncat.o ./BUILD/strncmp.o ./BUILD/strncpy.o ./BUILD/syslib.o ./BUILD/open.o \
	     ./BUILD/close.o ./BUILD/fstat.o ./BUILD/exit.o ./BUILD/abort.o ./BUILD/access.o \
	     ./BUILD/alarm.o ./BUILD/brk.o ./BUILD/brk2.o ./BUILD/chdir.o ./BUILD/chmod.o \
	     ./BUILD/chown.o ./BUILD/chroot.o ./BUILD/cleanup.o ./BUILD/creat.o ./BUILD/crypt.o \
             ./BUILD/dup.o ./BUILD/dup2.o ./BUILD/execlib.o ./BUILD/fclose.o ./BUILD/fflush.o \
             ./BUILD/writelib.o ./BUILD/printdat.o ./BUILD/putc.o ./BUILD/doprintf.o ./BUILD/getc.o \
             ./BUILD/readlib.o ./BUILD/fgets.o ./BUILD/fopen.o ./BUILD/lseek.o ./BUILD/malloc.o \
	     ./BUILD/fseek.o ./BUILD/getegid.o ./BUILD/getpid.o ./BUILD/isatty.o ./BUILD/perror.o \
	     ./BUILD/fprintf.o ./BUILD/fputs.o ./BUILD/fread.o ./BUILD/freopen.o ./BUILD/fwrite.o \
             ./BUILD/getenv.o ./BUILD/geteuid.o ./BUILD/getgid.o ./BUILD/getgrent.o ./BUILD/getpass.o \
             ./BUILD/getpwent.o ./BUILD/gets.o ./BUILD/getuid.o ./BUILD/index.o ./BUILD/ioctl.o \
             ./BUILD/kill.o ./BUILD/linklib.o ./BUILD/mknod.o ./BUILD/mountlib.o ./BUILD/pause.o \
             ./BUILD/pipelib.o ./BUILD/prints.o ./BUILD/rand.o ./BUILD/rindex.o ./BUILD/setbuf.o \
             ./BUILD/setuid.o ./BUILD/sleep.o ./BUILD/signal.o ./BUILD/sprintf.o ./BUILD/stat.o \
             ./BUILD/stderr.o ./BUILD/stime.o ./BUILD/sync.o ./BUILD/timelib.o ./BUILD/times.o \
             ./BUILD/umount.o ./BUILD/ungetc.o ./BUILD/unlink.o ./BUILD/utime.o  ./BUILD/getutil.o \
	     ./BUILD/setgid.o ./BUILD/stb.o ./BUILD/umask.o 


# All Phony Targets
.PHONY : everything clean

# Default starting position
everything :cleanAll $(MinixBOOT) $(MinixKERNEL) $(MINIXINIT) $(LIB) clean

cleanAll :
	rm -f $(MinixBOOT)
	rm -f $(MinixKERNEL)
	rm -f $(OBJS)
	rm -f $(INITOBJS)
	rm -f $(LIB)

clean:
	rm -f $(OBJS)
	rm -f $(INITOBJS)
	rm -f $(LIBOBJS)

./BUILD/BOOT.bin : ./BOOT/BOOT.asm
	$(ASM) $(ASMBFLAGS) -o $@ $<

./BUILD/LOADER.bin : ./LOADER/LOADER.asm
	$(ASM) $(ASMLFLAGS) -o $@ $<

./BUILD/KERNEL.o : ./KERNEL/x86.asm
	$(ASM) $(ASMKFLAGS) -o $@ $<

./INIT/BUILD/init.o : ./INIT/init.asm
	$(ASM) $(ASMKFLAGS) -o $@ $<

./BUILD/klibx86.o : ./KERNEL/klibx86.asm
	$(ASM) $(ASMKFLAGS) -o $@ $<

./BUILD/sendrec.o : ./LIB/sendrec.asm
	$(ASM) $(ASMKFLAGS) -o $@ $<

./BUILD/main.o : ./KERNEL/main.c ./H/const.h ./H/type.h ./KERNEL/const.h ./KERNEL/type.h \
  ./KERNEL/glo.h
	$(CC) $(CFLAGS) -o $@ $<

./BUILD/table.o : ./KERNEL/table.c ./H/const.h ./H/type.h ./KERNEL/const.h ./KERNEL/type.h \
  ./KERNEL/glo.h ./KERNEL/proc.h
	$(CC) $(CFLAGS) -o $@ $<

./BUILD/protect.o: ./KERNEL/protect.c ./H/const.h ./H/type.h ./KERNEL/const.h \
  ./KERNEL/type.h ./KERNEL/glo.h ./KERNEL/proc.h ./KERNEL/protect.h
	$(CC) $(CFLAGS) -o $@ $<

./BUILD/i8259.o: ./KERNEL/i8259.c ./KERNEL/const.h ./KERNEL/type.h ./KERNEL/glo.h \
  ./KERNEL/protect.h
	$(CC) $(CFLAGS) -o $@ $<

./BUILD/start.o: ./KERNEL/start.c ./H/const.h ./H/type.h ./KERNEL/const.h ./KERNEL/type.h \
  ./KERNEL/glo.h ./KERNEL/proc.h
	$(CC) $(CFLAGS) -o $@ $<

./BUILD/proc.o: ./KERNEL/proc.c ./H/const.h ./H/type.h ./KERNEL/const.h ./KERNEL/type.h \
  ./KERNEL/protect.h ./KERNEL/proc.h ./KERNEL/glo.h
	$(CC) $(CFLAGS) -o $@ $<

./BUILD/clock.o: ./KERNEL/clock.c ./H/const.h ./H/type.h ./H/com.h ./KERNEL/const.h \
  ./KERNEL/type.h ./KERNEL/protect.h ./KERNEL/proc.h ./KERNEL/glo.h
	$(CC) $(CFLAGS) -o $@ $<

./BUILD/system.o: ./KERNEL/system.c ./H/const.h ./H/type.h ./H/com.h ./KERNEL/const.h \
  ./KERNEL/type.h ./KERNEL/protect.h ./KERNEL/proc.h ./KERNEL/glo.h
	$(CC) $(CFLAGS) -o $@ $<

./BUILD/memory.o: ./KERNEL/memory.c ./H/const.h ./H/type.h ./H/callnr.h ./H/com.h ./H/error.h \
  ./KERNEL/const.h ./KERNEL/type.h ./KERNEL/proc.h
	$(CC) $(CFLAGS) -o $@ $<

./BUILD/printk.o: ./LIB/printk.c
	$(CC) $(CFLAGS) -o $@ $<

./BUILD/abs.o: ./LIB/abs.c
	$(CC) $(CFLAGS) -o $@ $<

./BUILD/atoi.o: ./LIB/atoi.c
	$(CC) $(CFLAGS) -o $@ $<

./BUILD/ctype.o: ./LIB/ctype.c ./INCLUDE/ctype.h
	$(CC) $(CFLAGS) -o $@ $<

./BUILD/atol.o: ./LIB/atol.c ./INCLUDE/ctype.h
	$(CC) $(CFLAGS) -o $@ $< 

./BUILD/bcopy.o: ./LIB/bcopy.c
	$(CC) $(CFLAGS) -o $@ $<

./BUILD/itoa.o: ./LIB/itoa.c
	$(CC) $(CFLAGS) -o $@ $<

./BUILD/strcat.o: ./LIB/strcat.c
	$(CC) $(CFLAGS) -o $@ $<

./BUILD/strcmp.o: ./LIB/strcmp.c
	$(CC) $(CFLAGS) -o $@ $<

./BUILD/strcpy.o: ./LIB/strcpy.c
	$(CC) $(CFLAGS) -o $@ $<

./BUILD/strlen.o: ./LIB/strlen.c
	$(CC) $(CFLAGS) -o $@ $<

./BUILD/strncat.o: ./LIB/strncat.c
	$(CC) $(CFLAGS) -o $@ $<

./BUILD/strncmp.o: ./LIB/strncmp.c
	$(CC) $(CFLAGS) -o $@ $<

./BUILD/strncpy.o: ./LIB/strncpy.c
	$(CC) $(CFLAGS) -o $@ $<

./BUILD/tty.o: ./KERNEL/tty.c ./H/const.h ./H/type.h ./H/com.h ./H/signal.h ./H/error.h \
  ./H/callnr.h ./KERNEL/keymap.h ./KERNEL/keyboard.h ./KERNEL/const.h \
  ./KERNEL/type.h ./KERNEL/protect.h ./KERNEL/proc.h ./KERNEL/glo.h
	$(CC) $(CFLAGS) -o $@ $<

./BUILD/win.o: ./KERNEL/win.c ./H/const.h ./H/type.h ./H/com.h ./H/error.h ./H/callnr.h \
  ./KERNEL/const.h ./KERNEL/type.h ./KERNEL/proc.h ./KERNEL/glo.h  
	$(CC) $(CFLAGS) -o $@ $<

./BUILD/fs_main.o: ./FS/fs_main.c
	$(CC) $(CFLAGS) -o $@ $< 

./BUILD/inode.o: ./FS/inode.c ./h/const.h ./h/type.h ./h/error.h ./FS/const.h ./FS/type.h \
  ./FS/inode.h
	$(CC) $(CFLAGS) -o $@ $< 

./BUILD/tablefs.o: ./FS/tablefs.c ./h/const.h ./h/type.h ./FS/const.h ./FS/type.h ./h/callnr.h \
  ./h/com.h ./h/error.h ./FS/buf.h ./FS/glo.h ./FS/inode.h
	$(CC) $(CFLAGS) -o $@ $< 

./BUILD/cache.o: FS/cache.c h/const.h h/type.h h/error.h FS/const.h FS/type.h \
  FS/buf.h FS/glo.h FS/inode.h FS/super.h
	$(CC) $(CFLAGS) -o $@ $< 

./BUILD/device.o: FS/device.c h/const.h h/type.h h/com.h h/error.h FS/const.h \
  FS/type.h FS/dev.h FS/glo.h FS/inode.h
	$(CC) $(CFLAGS) -o $@ $< 

./BUILD/pipe.o: FS/pipe.c h/const.h h/type.h h/callnr.h h/com.h h/error.h \
  h/signal.h FS/const.h FS/type.h FS/glo.h FS/inode.h 
	$(CC) $(CFLAGS) -o $@ $< 
	
./BUILD/read.o: FS/read.c H/const.h H/type.h H/com.h H/error.h FS/const.h \
  FS/type.h FS/buf.h FS/file.h FS/fproc.h FS/glo.h FS/inode.h FS/param.h \
  FS/super.h 
	$(CC) $(CFLAGS) -o $@ $< 

./BUILD/super.o: FS/super.c H/const.h H/type.h H/error.h FS/const.h FS/type.h \
  FS/buf.h FS/inode.h FS/super.h 
	$(CC) $(CFLAGS) -o $@ $<

./BUILD/filedes.o: FS/filedes.c H/const.h H/type.h H/error.h FS/const.h FS/type.h \
  FS/file.h FS/fproc.h FS/glo.h FS/inode.h 
	$(CC) $(CFLAGS) -o $@ $<  

./BUILD/fsput.o: FS/fsput.c H/const.h H/type.h H/com.h
	$(CC) $(CFLAGS) -o $@ $< 

./BUILD/time.o: FS/time.c H/const.h H/type.h H/callnr.h H/com.h H/error.h \
  FS/const.h FS/type.h FS/file.h FS/fproc.h FS/glo.h FS/inode.h \
  FS/param.h 
	$(CC) $(CFLAGS) -o $@ $< 

./BUILD/path.o: FS/path.c H/const.h H/type.h H/error.h FS/const.h FS/type.h \
  FS/buf.h FS/file.h FS/fproc.h FS/glo.h FS/inode.h FS/super.h 
	$(CC) $(CFLAGS) -o $@ $< 

./BUILD/protectfs.o: FS/protectfs.c H/const.h H/type.h H/error.h FS/const.h \
  FS/type.h FS/buf.h FS/file.h FS/fproc.h FS/glo.h FS/inode.h FS/param.h \
  FS/super.h 
	$(CC) $(CFLAGS) -o $@ $< 

./BUILD/utility.o: FS/utility.c H/const.h H/type.h H/com.h H/error.h FS/const.h \
  FS/type.h FS/buf.h FS/file.h FS/fproc.h FS/glo.h FS/inode.h FS/param.h \
  FS/super.h 
	$(CC) $(CFLAGS) -o $@ $< 
	
./BUILD/write.o: FS/write.c H/const.h H/type.h H/error.h FS/const.h FS/type.h \
  FS/buf.h FS/file.h FS/fproc.h FS/glo.h FS/inode.h FS/super.h 
	$(CC) $(CFLAGS) -o $@ $< 

./BUILD/link.o: FS/link.c H/const.h H/type.h H/error.h FS/const.h FS/type.h \
  FS/buf.h FS/file.h FS/fproc.h FS/glo.h FS/inode.h FS/param.h 
	$(CC) $(CFLAGS) -o $@ $< 

./BUILD/misc.o: FS/misc.c H/const.h H/type.h H/callnr.h H/com.h H/error.h \
  FS/const.h FS/type.h FS/buf.h FS/dev.h FS/file.h FS/fproc.h FS/glo.h \
  FS/inode.h FS/param.h FS/super.h 
	$(CC) $(CFLAGS) -o $@ $< 

./BUILD/mount.o: FS/mount.c H/const.h H/type.h H/error.h FS/const.h FS/type.h \
  FS/buf.h FS/file.h FS/fproc.h FS/glo.h FS/inode.h FS/param.h FS/super.h 
	$(CC) $(CFLAGS) -o $@ $< 

./BUILD/open_fs.o: FS/open_fs.c H/const.h H/type.h H/callnr.h H/error.h FS/const.h \
  FS/type.h FS/buf.h FS/file.h FS/fproc.h FS/glo.h FS/inode.h FS/param.h 
	$(CC) $(CFLAGS) -o $@ $< 

./BUILD/stadir.o: FS/stadir.c H/const.h H/type.h H/error.h H/stat.h FS/const.h \
  FS/type.h FS/file.h FS/fproc.h FS/glo.h FS/inode.h FS/param.h 
	$(CC) $(CFLAGS) -o $@ $< 

./BUILD/syslib.o: LIB/syslib.c H/const.h H/type.h H/callnr.h H/com.h H/error.h
	$(CC) $(CFLAGS) -o $@ $<  

./BUILD/call.o: LIB/call.c INCLUDE/lib.h H/const.h H/type.h H/error.h H/callnr.h 
	$(CC) $(CFLAGS) -o $@ $< 

./BUILD/message.o: LIB/message.c H/const.h H/type.h 
	$(CC) $(CFLAGS) -o $@ $< 

./BUILD/mm_main.o: MM/mm_main.c H/const.h H/type.h H/callnr.h H/com.h H/error.h \
  MM/const.h MM/glo.h MM/mproc.h MM/param.h 
	$(CC) $(CFLAGS) -o $@ $< 

./BUILD/table_mm.o: MM/table_mm.c h/const.h h/type.h MM/const.h h/callnr.h \
  MM/glo.h MM/mproc.h MM/param.h 
	$(CC) $(CFLAGS) -o $@ $< 

./BUILD/put_mm.o: MM/put_mm.c h/const.h h/type.h h/com.h 
	$(CC) $(CFLAGS) -o $@ $< 

./BUILD/utility_mm.o: MM/utility_mm.c h/const.h h/type.h h/callnr.h h/com.h \
  h/error.h h/stat.h MM/const.h MM/glo.h MM/mproc.h 
	$(CC) $(CFLAGS) -o $@ $< 
	
./BUILD/open.o: LIB/open.c INCLUDE/lib.h H/const.h H/type.h H/error.h H/callnr.h 
	$(CC) $(CFLAGS) -o $@ $< 

./BUILD/close.o: LIB/close.c INCLUDE/lib.h H/const.h H/type.h H/error.h \
  H/callnr.h 
	$(CC) $(CFLAGS) -o $@ $< 

./BUILD/fstat.o: LIB/fstat.c INCLUDE/lib.h H/const.h H/type.h H/error.h \
  H/callnr.h 
	$(CC) $(CFLAGS) -o $@ $< 

./BUILD/getset.o: MM/getset.c H/const.h H/type.h H/callnr.h H/error.h MM/const.h \
  MM/glo.h MM/mproc.h MM/param.h 
	$(CC) $(CFLAGS) -o $@ $< 

./BUILD/alloc.o: MM/alloc.c H/const.h H/type.h MM/const.h 
	$(CC) $(CFLAGS) -o $@ $< 

./BUILD/break.o: MM/break.c H/const.h H/type.h H/error.h H/signal.h MM/const.h \
  MM/glo.h MM/mproc.h MM/param.h 
	$(CC) $(CFLAGS) -o $@ $< 

./BUILD/exec.o: MM/exec.c H/const.h H/type.h H/callnr.h H/error.h H/stat.h \
  MM/const.h MM/glo.h MM/mproc.h MM/param.h 
	$(CC) $(CFLAGS) -o $@ $< 

./BUILD/forkexit.o: MM/forkexit.c H/const.h H/type.h H/callnr.h H/error.h \
  MM/const.h MM/glo.h MM/mproc.h MM/param.h 
	$(CC) $(CFLAGS) -o $@ $< 

./BUILD/signal_mm.o: MM/signal_mm.c H/const.h H/type.h H/callnr.h H/com.h \
  H/error.h H/signal.h H/stat.h MM/const.h MM/glo.h MM/mproc.h MM/param.h
	$(CC) $(CFLAGS) -o $@ $< 

./BUILD/exit.o: LIB/exit.c INCLUDE/lib.h H/const.h H/type.h H/error.h H/callnr.h 
	$(CC) $(CFLAGS) -o $@ $< 

./BUILD/abort.o: LIB/abort.c 
	$(CC) $(CFLAGS) -o $@ $< 

./BUILD/access.o: LIB/access.c INCLUDE/lib.h H/const.h H/type.h H/error.h \
  H/callnr.h
	$(CC) $(CFLAGS) -o $@ $< 

./BUILD/alarm.o: LIB/alarm.c INCLUDE/lib.h H/const.h H/type.h H/error.h \
  H/callnr.h 
	$(CC) $(CFLAGS) -o $@ $< 

./BUILD/brk.o: LIB/brk.c INCLUDE/lib.h H/const.h H/type.h H/error.h H/callnr.h 
	$(CC) $(CFLAGS) -o $@ $< 

./BUILD/brk2.o: LIB/brk2.c INCLUDE/lib.h H/const.h H/type.h H/error.h H/callnr.h 
	$(CC) $(CFLAGS) -o $@ $< 

./BUILD/chdir.o: LIB/chdir.c INCLUDE/lib.h H/const.h H/type.h H/error.h \
  H/callnr.h
	$(CC) $(CFLAGS) -o $@ $< 

./BUILD/chmod.o: LIB/chmod.c INCLUDE/lib.h H/const.h H/type.h H/error.h \
  H/callnr.h
	$(CC) $(CFLAGS) -o $@ $< 

./BUILD/chown.o: LIB/chown.c INCLUDE/lib.h H/const.h H/type.h H/error.h \
  H/callnr.h
	$(CC) $(CFLAGS) -o $@ $< 

./BUILD/chroot.o: LIB/chroot.c INCLUDE/lib.h H/const.h H/type.h H/error.h \
  H/callnr.h
	$(CC) $(CFLAGS) -o $@ $< 

./BUILD/cleanup.o: LIB/cleanup.c INCLUDE/stdio.h
	$(CC) $(CFLAGS) -o $@ $< 

./BUILD/creat.o: LIB/creat.c INCLUDE/lib.h H/const.h H/type.h H/error.h \
  H/callnr.h
	$(CC) $(CFLAGS) -o $@ $< 

./BUILD/crypt.o: LIB/crypt.c
	$(CC) $(CFLAGS) -o $@ $< 

./BUILD/dup.o: LIB/dup.c INCLUDE/lib.h H/const.h H/type.h H/error.h H/callnr.h
	$(CC) $(CFLAGS) -o $@ $< 

./BUILD/dup2.o: LIB/dup2.c INCLUDE/lib.h H/const.h H/type.h H/error.h H/callnr.h
	$(CC) $(CFLAGS) -o $@ $< 

./BUILD/execlib.o: LIB/exec.c INCLUDE/lib.h H/const.h H/type.h H/error.h H/callnr.h
	$(CC) $(CFLAGS) -o $@ $< 

./BUILD/fclose.o: LIB/fclose.c INCLUDE/stdio.h
	$(CC) $(CFLAGS) -o $@ $< 

./BUILD/fflush.o: LIB/fflush.c INCLUDE/stdio.h
	$(CC) $(CFLAGS) -o $@ $< 

./BUILD/writelib.o: LIB/write.c INCLUDE/lib.h H/const.h H/type.h H/error.h \
  H/callnr.h
	$(CC) $(CFLAGS) -o $@ $< 

./BUILD/printdat.o: LIB/printdat.c INCLUDE/stdio.h
	$(CC) $(CFLAGS) -o $@ $< 

./BUILD/putc.o: LIB/putc.c INCLUDE/stdio.h
	$(CC) $(CFLAGS) -o $@ $< 

./BUILD/doprintf.o: LIB/doprintf.c INCLUDE/stdio.h
	$(CC) $(CFLAGS) -o $@ $< 

./BUILD/getc.o: LIB/getc.c INCLUDE/stdio.h
	$(CC) $(CFLAGS) -o $@ $< 

./BUILD/readlib.o: LIB/read.c INCLUDE/lib.h H/const.h H/type.h H/error.h H/callnr.h
	$(CC) $(CFLAGS) -o $@ $< 

./BUILD/fgets.o: LIB/fgets.c INCLUDE/stdio.h
	$(CC) $(CFLAGS) -o $@ $< 

./BUILD/fopen.o: LIB/fopen.c INCLUDE/stdio.h
	$(CC) $(CFLAGS) -o $@ $< 

./BUILD/lseek.o: LIB/lseek.c INCLUDE/lib.h H/const.h H/type.h H/error.h \
  H/callnr.h
	$(CC) $(CFLAGS) -o $@ $< 

./BUILD/malloc.o: LIB/malloc.c
	$(CC) $(CFLAGS) -o $@ $< 

./BUILD/fseek.o: LIB/fseek.c INCLUDE/stdio.h
	$(CC) $(CFLAGS) -o $@ $< 

./BUILD/fork.o: LIB/fork.c INCLUDE/lib.h H/const.h H/type.h H/error.h H/callnr.h
	$(CC) $(CFLAGS) -o $@ $< 

./BUILD/fprintf.o: LIB/fprintf.c INCLUDE/stdio.h
	$(CC) $(CFLAGS) -o $@ $< 

./BUILD/fputs.o: LIB/fputs.c INCLUDE/stdio.h
	$(CC) $(CFLAGS) -o $@ $< 

./BUILD/fread.o: LIB/fread.c INCLUDE/stdio.h
	$(CC) $(CFLAGS) -o $@ $< 

./BUILD/freopen.o: LIB/freopen.c INCLUDE/stdio.h
	$(CC) $(CFLAGS) -o $@ $< 

./BUILD/fwrite.o: LIB/fwrite.c INCLUDE/stdio.h
	$(CC) $(CFLAGS) -o $@ $< 

./BUILD/getegid.o: LIB/getegid.c INCLUDE/lib.h H/const.h H/type.h H/error.h \
  H/callnr.h
	$(CC) $(CFLAGS) -o $@ $< 

./BUILD/getenv.o: LIB/getenv.c
	$(CC) $(CFLAGS) -o $@ $< 

./BUILD/geteuid.o: LIB/geteuid.c INCLUDE/lib.h H/const.h H/type.h H/error.h \
  H/callnr.h
	$(CC) $(CFLAGS) -o $@ $< 

./BUILD/getgid.o: LIB/getgid.c INCLUDE/lib.h H/const.h H/type.h H/error.h \
  H/callnr.h
	$(CC) $(CFLAGS) -o $@ $< 

./BUILD/getgrent.o: LIB/getgrent.c INCLUDE/grp.h
	$(CC) $(CFLAGS) -o $@ $< 

./BUILD/getpass.o: LIB/getpass.c H/sgtty.h
	$(CC) $(CFLAGS) -o $@ $< 

./BUILD/getpid.o: LIB/getpid.c INCLUDE/lib.h H/const.h H/type.h H/error.h \
  H/callnr.h
	$(CC) $(CFLAGS) -o $@ $< 

./BUILD/getpwent.o: LIB/getpwent.c INCLUDE/pwd.h
	$(CC) $(CFLAGS) -o $@ $< 

./BUILD/gets.o: LIB/gets.c INCLUDE/stdio.h
	$(CC) $(CFLAGS) -o $@ $< 

./BUILD/getuid.o: LIB/getuid.c INCLUDE/lib.h H/const.h H/type.h H/error.h \
  H/callnr.h
	$(CC) $(CFLAGS) -o $@ $< 

./BUILD/index.o: LIB/index.c
	$(CC) $(CFLAGS) -o $@ $< 

./BUILD/ioctl.o: LIB/ioctl.c INCLUDE/lib.h H/const.h H/type.h H/error.h \
  H/callnr.h H/com.h INCLUDE/sgtty.h
	$(CC) $(CFLAGS) -o $@ $< 

./BUILD/isatty.o: LIB/isatty.c INCLUDE/stat.h
	$(CC) $(CFLAGS) -o $@ $< 

./BUILD/kill.o: LIB/kill.c INCLUDE/lib.h H/const.h H/type.h H/error.h H/callnr.h
	$(CC) $(CFLAGS) -o $@ $< 

./BUILD/linklib.o: LIB/link.c INCLUDE/lib.h H/const.h H/type.h H/error.h H/callnr.h
	$(CC) $(CFLAGS) -o $@ $< 

./BUILD/mknod.o: LIB/mknod.c INCLUDE/lib.h H/const.h H/type.h H/error.h \
  H/callnr.h
	$(CC) $(CFLAGS) -o $@ $< 

./BUILD/mountlib.o: LIB/mount.c INCLUDE/lib.h H/const.h H/type.h H/error.h \
  H/callnr.h
	$(CC) $(CFLAGS) -o $@ $< 

./BUILD/pause.o: LIB/pause.c INCLUDE/lib.h H/const.h H/type.h H/error.h \
  H/callnr.h
	$(CC) $(CFLAGS) -o $@ $< 

./BUILD/perror.o: LIB/perror.c H/error.h
	$(CC) $(CFLAGS) -o $@ $< 

./BUILD/pipelib.o: LIB/pipe.c INCLUDE/lib.h H/const.h H/type.h H/error.h H/callnr.h
	$(CC) $(CFLAGS) -o $@ $< 

./BUILD/prints.o: LIB/prints.c
	$(CC) $(CFLAGS) -o $@ $< 

./BUILD/rand.o: LIB/rand.c
	$(CC) $(CFLAGS) -o $@ $< 

./BUILD/rindex.o: LIB/rindex.c
	$(CC) $(CFLAGS) -o $@ $< 

./BUILD/setbuf.o: LIB/setbuf.c INCLUDE/stdio.h
	$(CC) $(CFLAGS) -o $@ $< 

./BUILD/setgid.o: LIB/setgid.c INCLUDE/lib.h H/const.h H/type.h H/error.h \
  H/callnr.h
	$(CC) $(CFLAGS) -o $@ $< 

./BUILD/setuid.o: LIB/setuid.c INCLUDE/lib.h H/const.h H/type.h H/error.h \
  H/callnr.h
	$(CC) $(CFLAGS) -o $@ $< 

./BUILD/sleep.o: LIB/sleep.c INCLUDE/lib.h H/const.h H/type.h H/error.h \
  H/callnr.h INCLUDE/signal.h
	$(CC) $(CFLAGS) -o $@ $< 

./BUILD/signal.o: LIB/signal.c INCLUDE/lib.h H/const.h H/type.h H/error.h \
  H/callnr.h INCLUDE/signal.h
	$(CC) $(CFLAGS) -o $@ $< 

./BUILD/sprintf.o: LIB/sprintf.c INCLUDE/stdio.h
	$(CC) $(CFLAGS) -o $@ $< 

./BUILD/stat.o: LIB/stat.c INCLUDE/lib.h H/const.h H/type.h H/error.h H/callnr.h
	$(CC) $(CFLAGS) -o $@ $< 

./BUILD/stb.o: LIB/stb.c
	$(CC) $(CFLAGS) -o $@ $< 

./BUILD/stderr.o: LIB/stderr.c
	$(CC) $(CFLAGS) -o $@ $< 

./BUILD/stime.o: LIB/stime.c INCLUDE/lib.h H/const.h H/type.h H/error.h \
  H/callnr.h
	$(CC) $(CFLAGS) -o $@ $< 

./BUILD/sync.o: LIB/sync.c INCLUDE/lib.h H/const.h H/type.h H/error.h H/callnr.h
	$(CC) $(CFLAGS) -o $@ $< 

./BUILD/timelib.o: LIB/time.c INCLUDE/lib.h H/const.h H/type.h H/error.h H/callnr.h
	$(CC) $(CFLAGS) -o $@ $< 

./BUILD/times.o: LIB/times.c INCLUDE/lib.h H/const.h H/type.h H/error.h \
  H/callnr.h
	$(CC) $(CFLAGS) -o $@ $< 

./BUILD/umask.o: LIB/umask.c INCLUDE/lib.h H/const.h H/type.h H/error.h \
  H/callnr.h
	$(CC) $(CFLAGS) -o $@ $< 

./BUILD/umount.o: LIB/umount.c INCLUDE/lib.h H/const.h H/type.h H/error.h \
  H/callnr.h
	$(CC) $(CFLAGS) -o $@ $< 

./BUILD/ungetc.o: LIB/ungetc.c INCLUDE/stdio.h
	$(CC) $(CFLAGS) -o $@ $< 

./BUILD/unlink.o: LIB/unlink.c INCLUDE/lib.h H/const.h H/type.h H/error.h \
  H/callnr.h
	$(CC) $(CFLAGS) -o $@ $< 

./BUILD/utime.o: LIB/utime.c INCLUDE/lib.h H/const.h H/type.h H/error.h \
  H/callnr.h
	$(CC) $(CFLAGS) -o $@ $< 

./BUILD/wait.o: LIB/wait.c INCLUDE/lib.h H/const.h H/type.h H/error.h H/callnr.h
	$(CC) $(CFLAGS) -o $@ $< 

./BUILD/getutil.o : ./LIB/getutil.asm
	$(ASM) $(ASMKFLAGS) -o $@ $< 
 
./INIT/BUILD/initc.o: INIT/init.c H/signal.h H/const.h H/type.h H/com.h
	$(CC) $(CFLAGS) -o $@ $< 

$(LIB): $(LIBOBJS) 
	ar crv $(LIB) $(LIBOBJS) 

$(MinixKERNEL) : $(OBJS) $(LIB)
	$(LD) $(LDFLAGS)  -o $(MinixKERNEL) $(OBJS) -L./BUILD  -lc

$(MINIXINIT) : $(INITOBJS) $(LIB)
	$(LD) $(INITLDFLAGS) -o $(MINIXINIT) $(INITOBJS) -L./BUILD  -lc
