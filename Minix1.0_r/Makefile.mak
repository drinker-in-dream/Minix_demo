
##################################
# Makefile for Minix             #
# make -f Makefile.mak           #
##################################

# Entry point of Minix
# It must be as same as 'KernelEntryPointPhyAddr' !!!
ENTRYPOINT	= 0x30400

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

# This Program
MinixBOOT	= ./BUILD/BOOT.bin ./BUILD/LOADER.bin
MinixKERNEL	= ./BUILD/KERNEL.bin
OBJS		=  ./BUILD/KERNEL.o ./BUILD/main.o ./BUILD/klibx86.o ./BUILD/table.o \
  ./BUILD/protect.o ./BUILD/i8259.o ./BUILD/start.o ./BUILD/proc.o ./BUILD/clock.o \
  ./BUILD/sendrec.o ./BUILD/system.o  ./BUILD/tty.o  ./BUILD/printk.o  ./BUILD/abs.o \
  ./BUILD/atoi.o ./BUILD/ctype.o ./BUILD/atol.o ./BUILD/bcopy.o ./BUILD/itoa.o \
  ./BUILD/strcat.o ./BUILD/strcmp.o ./BUILD/strcpy.o ./BUILD/strlen.o ./BUILD/strncat.o \
  ./BUILD/strncmp.o ./BUILD/strncpy.o ./BUILD/memory.o ./BUILD/win.o  ./BUILD/fs_main.o \
  ./BUILD/inode.o ./BUILD/tablefs.o ./BUILD/cache.o ./BUILD/device.o  ./BUILD/pipe.o \
  ./BUILD/read.o ./BUILD/super.o ./BUILD/filedes.o  ./BUILD/fsput.o ./BUILD/time.o \
  ./BUILD/path.o ./BUILD/protectfs.o ./BUILD/utility.o ./BUILD/write.o ./BUILD/link.o \
  ./BUILD/misc.o ./BUILD/mount.o ./BUILD/open.o ./BUILD/stadir.o ./BUILD/syslib.o \
  ./BUILD/call.o ./BUILD/message.o 

# All Phony Targets
.PHONY : everything clean

# Default starting position
everything :cleanAll $(MinixBOOT) $(MinixKERNEL) clean

cleanAll :
	rm -f $(MinixBOOT)
	rm -f $(MinixKERNEL)
	rm -f $(OBJS)

clean:
	rm -f $(OBJS)

./BUILD/BOOT.bin : ./BOOT/BOOT.asm
	$(ASM) $(ASMBFLAGS) -o $@ $<

./BUILD/LOADER.bin : ./LOADER/LOADER.asm
	$(ASM) $(ASMLFLAGS) -o $@ $<

./BUILD/KERNEL.o : ./KERNEL/x86.asm
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

./BUILD/open.o: FS/open.c H/const.h H/type.h H/callnr.h H/error.h FS/const.h \
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

$(MinixKERNEL) : $(OBJS)
	$(LD) $(LDFLAGS) -o $(MinixKERNEL) $(OBJS)
