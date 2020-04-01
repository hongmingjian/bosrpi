CROSS=arm-none-eabi-

SHELL=   /bin/bash
CC=      $(CROSS)gcc
AS=      $(CROSS)as
LD=      $(CROSS)ld
OBJCOPY= $(CROSS)objcopy
AR=      $(CROSS)ar
STRIP=   $(CROSS)strip
SIZE=    $(CROSS)size
STRINGS= $(CROSS)strings
READELF= $(CROSS)readelf
RANLIB=  $(CROSS)ranlib
NM=      $(CROSS)nm
GDB=     $(CROSS)gdb
OBJDUMP= $(CROSS)objdump

PROG=    kernel

CPPFLAGS+=-nostdinc -I.
ASFLAGS=-Wall -O2
CFLAGS+=-Wall -O2 -ffreestanding -fleading-underscore
LDFLAGS+=-Tkernel.ld -nostdlib

COBJS=machdep.o uudecode.o xmodem.o softfloat.o

OBJS=entry.o $(COBJS)

all: $(PROG).img

$(PROG).img: $(PROG).elf
	$(OBJCOPY) $^ -O binary $@

$(PROG).elf: kernel.ld $(OBJS)
	$(CC) $(LDFLAGS) $(OBJS) -o $@

kernel.ld: kernel.ld.in
	$(CC) -E -P -x c-header -o $@ $^

clean:
	-$(RM) $(OBJS) $(PROG).{ld,img,elf}
