
#CC65_DIR := cc65-nes-2.13.3-1
CC65_DIR := cc65

CC := $(CC65_DIR)/bin/cc65
CA := $(CC65_DIR)/bin/ca65
LD := $(CC65_DIR)/bin/ld65

TARGET_PLATFORM := nes

TARGETS := kokobatoru.nes cocobattleroyale.nes
EMULATOR ?= higan

EXAMPLES := $(TARGETS:..nes=)
OBJECTS := $(TARGETS:.nes=.o)
ASSEMBLY_SOURCES := $(TARGETS:.nes=.s)
C_SOURCES := $(TARGETS:.nes=.c)

.PHONY: all

# Disable builtin rules (for .c, .o) by providing an empty .SUFFIXES rule
# Yes. GNU make is a rat's nest
.SUFFIXES:

# Make sure intermediate files are *NOT* deleted
# Yes. GNU make is a rat's nest
.PRECIOUS: %.s %.o

all: $(CC) $(TARGETS)

# submodule build
$(CC):
	cd $(CC65_DIR) && $(MAKE)

%: %.nes
	$(EMULATOR) $<

clean:
	@rm -f $(TARGETS)
	@rm -f $(OBJECTS)
	@rm -f $(ASSEMBLY_SOURCES)
	@rm -f *.zip
	@rm -f asm/crt0.o

%.s: %.c
	$(CC) -Oi $< --target $(TARGET_PLATFORM) -I$(CC65_DIR)/include/ --add-source

%.o: %.s
	$(CA) $<

%.nes: %.o asm/crt0.o
	$(LD) -C nrom_128_horz.cfg -o $@ $^ $(TARGET_PLATFORM).lib

