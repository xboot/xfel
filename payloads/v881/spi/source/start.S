/* SPDX-License-Identifier: MIT */
/* Copyright (c) 2026 Han Gao <gaohan@iscas.ac.cn> */

	.section .text.start
	.global _start
	.type _start, @function

_start:
	/* Enable T-Head implementation-defined instructions. */
	li t1, 1 << 22
	csrs 0x7c0, t1
	fence.i

	/* Use a private stack, but restore the FEL stack before returning. */
	mv t0, sp
	li sp, 0x00101f00
	addi sp, sp, -16
	sw t0, 0(sp)
	sw ra, 4(sp)

	li a0, 0x00101000
	call sys_spif_run

	lw ra, 4(sp)
	lw t0, 0(sp)
	mv sp, t0
	ret

	.size _start, . - _start
