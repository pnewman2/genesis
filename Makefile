CROSS_COMPILE ?= m68k-linux-gnu-

AS = $(CROSS_COMPILE)as
CC = $(CROSS_COMPILE)gcc
LD = $(CROSS_COMPILE)ld
OBJCOPY = $(CROSS_COMPILE)objcopy

ASFLAGS = -mcpu=68000 -g
CFLAGS = -mcpu=68000 -Og -g

name = myrom
bin = $(name).bin
elf = $(name).elf

all : $(bin)

$(bin) : $(elf)
	$(OBJCOPY) -O binary $< $@

objs = vector.o main.o

$(elf) : $(objs)
	$(LD) -o $@ $^ -Tcartridge.ld

clean:
	-rm -f *.o $(bin) $(elf)

