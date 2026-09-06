# SPDX-License-Identifier: MIT

CROSS ?= riscv32-unknown-linux-gnu-

CC := $(CROSS)gcc
OBJCOPY := $(CROSS)objcopy
XXD ?= xxd

CFLAGS := -Os -g -Wall -Wextra -ffreestanding -fno-builtin
CFLAGS += -fno-stack-protector -ffunction-sections -fdata-sections
CFLAGS += -march=rv32imac_zicsr_zifencei -mabi=ilp32
CFLAGS += -mcmodel=medany -msmall-data-limit=0
LDFLAGS := -nostdlib -static -Wl,--build-id=none
LDFLAGS += -Wl,--gc-sections -Wl,-T,link.ld

OUTPUT ?= output
OBJECTS := $(OUTPUT)/start.o $(OUTPUT)/sys-spif.o

.PHONY: all check update clean

all: $(OUTPUT)/spi.bin

$(OUTPUT):
	mkdir -p $@

$(OUTPUT)/start.o: source/start.S | $(OUTPUT)
	$(CC) $(CFLAGS) -c $< -o $@

$(OUTPUT)/sys-spif.o: source/sys-spif.c | $(OUTPUT)
	$(CC) $(CFLAGS) -c $< -o $@

$(OUTPUT)/spi.elf: $(OBJECTS) link.ld
	$(CC) $(CFLAGS) $(LDFLAGS) $(OBJECTS) -o $@

$(OUTPUT)/spi.bin: $(OUTPUT)/spi.elf
	$(OBJCOPY) -O binary $< $@

$(OUTPUT)/spi.inc: $(OUTPUT)/spi.bin
	$(XXD) -i < $< > $@

check: $(OUTPUT)/spi.inc
	cmp $(OUTPUT)/spi.inc ../../../chips/v881-spi.inc

update: $(OUTPUT)/spi.inc
	cp $(OUTPUT)/spi.inc ../../../chips/v881-spi.inc

clean:
	rm -f $(OBJECTS) $(OUTPUT)/spi.elf $(OUTPUT)/spi.bin $(OUTPUT)/spi.inc
