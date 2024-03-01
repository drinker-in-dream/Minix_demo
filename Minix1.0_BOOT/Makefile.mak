
##################################
# Makefile for Minix             #
# make -f Makefile.mak           #
##################################

# Programs, flags, etc.
ASM		= nasm
ASMBFLAGS	= -I ./BOOT/INCLUDE

# This Program
DOXBOOT	= ./BUILD/BOOT.bin 

# All Phony Targets
.PHONY : everything clean

# Default starting position
everything :clean $(DOXBOOT)

clean :
	rm -f $(DOXBOOT)

./BUILD/BOOT.bin : ./BOOT/BOOT.asm
	$(ASM) $(ASMBFLAGS) -o $@ $<

