/* SPDX-License-Identifier: MIT */

OUTPUT_FORMAT("elf32-littleriscv")
OUTPUT_ARCH(riscv)
ENTRY(_start)

MEMORY
{
	ram : ORIGIN = 0x00100000, LENGTH = 0x1000
}

SECTIONS
{
	.text :
	{
		__image_start = .;
		*(.text.start)
		*(.text*)
	} > ram

	.rodata :
	{
		*(.rodata*)
		*(.srodata*)
	} > ram

	.data :
	{
		*(.data*)
		*(.sdata*)
	} > ram

	.bss (NOLOAD) :
	{
		__bss_start = .;
		*(.bss*)
		*(.sbss*)
		*(COMMON)
		__bss_end = .;
	} > ram

	/DISCARD/ :
	{
		*(.comment)
		*(.eh_frame*)
		*(.riscv.attributes)
	}

	ASSERT(__bss_end <= ORIGIN(ram) + LENGTH(ram),
	       "V881 SPI payload exceeds its 4 KiB code region")
}
