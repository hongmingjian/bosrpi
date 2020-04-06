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
LDFLAGS+=-T$(PROG).ld -nostdlib

COBJS=machdep.o uudecode.o xmodem.o softfloat.o string.o sdcard.o dosfs.o

OBJS=entry.o $(COBJS)

all: $(PROG).img

$(PROG).img: $(PROG).elf
	$(OBJCOPY) -S $^ -O binary $@

$(PROG).elf: $(PROG).ld $(OBJS)
	$(CC) $(LDFLAGS) $(OBJS) -o $@

$(PROG).ld: $(PROG).ld.in
	$(CC) -E -P -x c-header -o $@ $^

clean:
	-$(RM) $(OBJS) $(PROG).{ld,img,elf}
