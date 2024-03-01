
##################################
# Makefile for Minix             #
# make -f Makefile.mak           #
##################################

# Programs, flags, etc.
ASM		= nasm
ASMBFLAGS	= -I ./BOOT/INCLUDE
ASMLFLAGS	= -I  ./LOADER/INCLUDE

# This Program
MinixBOOT	= ./BUILD/BOOT.bin ./BUILD/LOADER.bin

# All Phony Targets
.PHONY : everything clean

# Default starting position
everything :clean $(MinixBOOT)

clean :
	rm -f $(MinixBOOT)

./BUILD/BOOT.bin : ./BOOT/BOOT.asm
	$(ASM) $(ASMBFLAGS) -o $@ $<

./BUILD/LOADER.bin : ./LOADER/LOADER.asm
	$(ASM) $(ASMLFLAGS) -o $@ $<