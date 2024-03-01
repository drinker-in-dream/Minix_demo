
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
  ./BUILD/protect.o ./BUILD/i8259.o ./BUILD/start.o

# All Phony Targets
.PHONY : everything clean

# Default starting position
everything :cleanAll $(MinixBOOT) $(MinixKERNEL) clean

cleanAll :
	rm -f $(MinixBOOT)
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

$(MinixKERNEL) : $(OBJS)
	$(LD) $(LDFLAGS) -o $(MinixKERNEL) $(OBJS)
