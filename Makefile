CROSS_COMPILE ?= m68k-linux-gnu-

AS = $(CROSS_COMPILE)as
CC = $(CROSS_COMPILE)cc
LD = $(CROSS_COMPILE)ld
OBJCOPY = $(CROSS_COMPILE)objcopy

ASFLAGS = -mcpu=68000

name = myrom
bin = $(name).bin
elf = $(name).elf

all : $(bin)

$(bin) : $(elf)
	$(OBJCOPY) -O binary $< $@

$(elf) : start.o
	$(LD) -o $@ $^ -Tcartridge.ld

clean:
	-rm -f *.o $(bin) $(elf)

