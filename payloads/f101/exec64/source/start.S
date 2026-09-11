/*
 * start.S
 *
 * Copyright(c) 2007-2021 Jianjun Jiang <8192542@qq.com>
 * Official site: http://xboot.org
 * Mobile phone: +86-18665388956
 * QQ: 8192542
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 *
 */

#include <linkage.h>
#include <riscv32.h>

	.global _start
_start:
	li t1, 0x1 << 22
	csrs mxstatus, t1
	fence.i
	j reset

reset:
	la t0, _lo
	lw a0, 0(t0)
	lw a1, 4(t0)

	/* Write back all dirty d-cache lines to memory */
	dcache.call
	sync.is

	/* Enable RISCV CFG bus gate, CCU + 0x0d0c, RISCV_CFG_BGR_REG */
	li t0, 0x02001d0c
	li t2, 0x1
	sw t2, 0(t0)

	/* Set RV64 start address, low 32 bits, RISCV_CFG + 0x100, BA_L_REG */
	li t0, 0x06010100
	sw a0, 0(t0)

	/* Set RV64 start address, high 32 bits, RISCV_CFG + 0x104, BA_H_REG */
	sw a1, 4(t0)

	/* Core config, RISCV_CFG + 0x108, CORE_CFG_REG, [9:8]=2: 64-bit core after reset */
	li t2, 0x200
	sw t2, 8(t0)

	/* Disable irq, RISCV_CFG + 0x6c, IRQ_ENABLE_REG */
	sw zero, 0x6c(t0)

	/* Enable RISC-V watchdog, RISCV_WDG + 0x0 */
	li t0, 0x06011000
	li t2, 0x1
	sw t2, 0(t0)

	/* Watchdog config, RISCV_WDG + 0x14, RWDOG_CFG_REG, key 0x16aa, reset cpu core only */
	li t2, 0x16aa0000
	sw t2, 0x14(t0)

	/* Make all writes visible before arming the watchdog */
	fence rw, rw
	fence.i

	/* Watchdog mode, RISCV_WDG + 0x18, RWDOG_MODE_REG, key 0x16aa, interval 0x1, enable */
	li t2, 0x16aa0011
	sw t2, 0x18(t0)

	/* wait for the reset */
1:	wfi
	j 1b

	.align 2
_lo:
	.word 0x11223344
_hi:
	.word 0x55667788
