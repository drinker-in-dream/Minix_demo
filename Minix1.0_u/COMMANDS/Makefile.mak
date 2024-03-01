
##################################
# Makefile for Minix Tools       #
# make -f Makefile.mak           #
##################################

ENTRYPOINT  = 0x80400

# Programs, flags, etc.
ASM		= nasm
ASMKFLAGS	= -I ./KERNEL -f elf
CC		= gcc
LD		= ld
CFLAGS		= -I ./INCLUDE -c -fno-builtin 
LDFLAGS		= -s 


# Programs
CRTSO = ./bin/crtso.o
COMMANDS         = ./bin/sh ./bin/crtso.o

OBJS		= ./bin/sh.o

# All Phony Targets
.PHONY : everything clean

# Default starting position
everything :cleanAll $(CRTSO) $(OBJS) $(COMMANDS) clean

cleanAll :
	rm -f $(OBJS)
	

clean:
	rm -f $(OBJS)


./bin/crtso.o : ../LIB/crtso.asm
	$(ASM) $(ASMKFLAGS) -o $@ $< 

#sh
SH = ./bin/sh
SHOBJS = ./bin/sh.o
./bin/sh.o: sh1.c 
	$(CC) $(CFLAGS) -o $@ $< 

./bin/sh : ./bin/sh.o
	$(LD) $(LDFLAGS) -o $(SH) $(CRTSO) $(SHOBJS) -L../BUILD -lc 



	






