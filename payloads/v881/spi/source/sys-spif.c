/* SPDX-License-Identifier: MIT */
/* Copyright (c) 2026 Han Gao <gaohan@iscas.ac.cn> */

typedef unsigned char u8;
typedef unsigned int u32;
typedef unsigned long uintptr_t;

#define BIT(n)			(1U << (n))

#define PIO_BASE		0x02000000U
#define PIO_BANK_SIZE		0x30U
#define PIO_CFG(pin)		(((pin) / 8U) * 4U)
#define PIO_CFG_SHIFT(pin)	(((pin) % 8U) * 4U)
#define PIO_PULL(pin)		(0x24U + (((pin) / 16U) * 4U))
#define PIO_PULL_SHIFT(pin)	(((pin) % 16U) * 2U)
#define GPIO_C			2U
#define SPIF_MUX		2U

#define CCU_BASE		0x02001000U
#define SPIF_CLK		(CCU_BASE + 0x950U)
#define SPI_BGR			(CCU_BASE + 0x96cU)

#define SPIF_BASE		0x04f00000U
#define SPIF_VERSION		0x00U
#define SPIF_GCR		0x04U
#define SPIF_GAR		0x08U
#define SPIF_INT_STATUS		0x18U
#define SPIF_CSD		0x1cU
#define SPIF_PHASE		0x20U
#define SPIF_FLASH_ADDR		0x24U
#define SPIF_BUS_WIDTH		0x28U
#define SPIF_TRANSFER_NUM	0x2cU
#define SPIF_DMA_CTL		0x40U
#define SPIF_DESC_ADDR		0x44U

#define SPIF_DMA_RESET		BIT(4)
#define SPIF_SOFT_RESET		BIT(3)
#define SPIF_FIFO_RESET		(BIT(0) | BIT(1))
#define SPIF_CS_ACTIVE_LOW	BIT(8)
#define SPIF_NORMAL_MODE	BIT(2)
#define SPIF_DMA_MODE		BIT(0)
#define SPIF_DMA_DONE		BIT(24)
#define SPIF_ERROR_MASK		(BIT(8) | BIT(9) | BIT(10))

#define SPIF_PHASE_RX		BIT(8)
#define SPIF_PHASE_TX		BIT(12)
#define SPIF_PHASE_DUMMY	BIT(16)
#define SPIF_PHASE_ADDR		BIT(24)
#define SPIF_PHASE_CMD		BIT(28)

#define SPIF_DESC_LAST		BIT(0)
#define SPIF_DESC_DMA_WRITE	BIT(1)
#define SPIF_DESC_BURST16	(0x7U << 4)
#define SPIF_DESC_BLOCK64	(0x3U << 24)
#define SPIF_DESC_NORMAL	BIT(28)
#define SPIF_ADDR_24BIT_V2	(0x2U << 24)
#define SPIF_DESC_LEN		(32U << 4)

#define SPIF_TIMEOUT		0x01000000U
#define SPIF_MAX_DATA		65536U
#define SPIF_MIN_RX		8U

#define COMMAND_END		0x00101100U
#define STATUS_ADDR		0x00101ffcU
#define DIAGNOSTIC_ADDR		0x00101f80U
#define SWAP_START		0x00102000U
#define SWAP_END		0x00112000U

enum {
	SPI_CMD_END = 0x00,
	SPI_CMD_INIT = 0x01,
	SPI_CMD_SELECT = 0x02,
	SPI_CMD_DESELECT = 0x03,
	SPI_CMD_FAST = 0x04,
	SPI_CMD_TXBUF = 0x05,
	SPI_CMD_RXBUF = 0x06,
	SPI_CMD_SPINOR_WAIT = 0x07,
	SPI_CMD_SPINAND_WAIT = 0x08,
};

struct spif_descriptor {
	u32 burst;
	u32 block_len;
	u32 data_addr;
	u32 next_desc;
	u32 phase;
	u32 flash_addr;
	u32 bus_width;
	u32 transfer_len;
} __attribute__((aligned(64)));

static struct spif_descriptor descriptor __attribute__((aligned(64)));
static u8 bounce[320] __attribute__((aligned(64)));

static inline u32 read32(uintptr_t address)
{
	return *(volatile u32 *)address;
}

static inline void write32(uintptr_t address, u32 value)
{
	*(volatile u32 *)address = value;
}

static void copy_bytes(u8 *dest, const u8 *source, u32 length)
{
	while (length--)
		*dest++ = *source++;
}

static void clear_bytes(void *buffer, u32 length)
{
	u8 *dest = buffer;

	while (length--)
		*dest++ = 0;
}

static void cache_clean(const void *buffer, u32 length)
{
	register uintptr_t line asm("a0") = (uintptr_t)buffer & ~31U;
	uintptr_t end = (uintptr_t)buffer + length;

	for (; line < end; line += 32U)
		__asm__ volatile (".word 0x0295000b" : : "r"(line) : "memory");
	__asm__ volatile (".word 0x0190000b" : : : "memory");
}

static void cache_invalidate(void *buffer, u32 length)
{
	register uintptr_t line asm("a0") = (uintptr_t)buffer & ~31U;
	uintptr_t end = (uintptr_t)buffer + length;

	for (; line < end; line += 32U)
		__asm__ volatile (".word 0x02a5000b" : : "r"(line) : "memory");
	__asm__ volatile (".word 0x0190000b" : : : "memory");
}

static void set_pin_mux(u32 pin, u32 mux)
{
	uintptr_t reg = PIO_BASE + GPIO_C * PIO_BANK_SIZE + PIO_CFG(pin);
	u32 shift = PIO_CFG_SHIFT(pin);
	u32 value = read32(reg);

	value &= ~(0xfU << shift);
	value |= mux << shift;
	write32(reg, value);
}

static void set_pin_pull(u32 pin, u32 pull)
{
	uintptr_t reg = PIO_BASE + GPIO_C * PIO_BANK_SIZE + PIO_PULL(pin);
	u32 shift = PIO_PULL_SHIFT(pin);
	u32 value = read32(reg);

	value &= ~(0x3U << shift);
	value |= pull << shift;
	write32(reg, value);
}

static int wait_clear(uintptr_t reg, u32 mask)
{
	u32 timeout = SPIF_TIMEOUT;

	while (read32(reg) & mask) {
		if (!--timeout)
			return -1;
	}
	return 0;
}

static int spif_init(void)
{
	u32 value;
	u32 pin;

	for (pin = 0; pin <= 5; pin++)
		set_pin_mux(pin, SPIF_MUX);
	set_pin_pull(1, 1);
	set_pin_pull(4, 1);
	set_pin_pull(5, 1);

	/* Use HOSC directly so the payload does not depend on a PLL state. */
	write32(SPIF_CLK, BIT(31));
	value = read32(SPI_BGR);
	write32(SPI_BGR, value & ~BIT(20));
	for (pin = 0; pin < 100; pin++)
		__asm__ volatile ("nop");
	write32(SPI_BGR, value | BIT(20) | BIT(4));

	value = read32(SPIF_BASE + SPIF_GAR);
	write32(SPIF_BASE + SPIF_GAR, value | SPIF_DMA_RESET);
	if (wait_clear(SPIF_BASE + SPIF_GAR, SPIF_DMA_RESET))
		return -2;

	value = read32(SPIF_BASE + SPIF_GAR);
	write32(SPIF_BASE + SPIF_GAR, value | SPIF_SOFT_RESET);
	if (wait_clear(SPIF_BASE + SPIF_GAR, SPIF_SOFT_RESET))
		return -3;

	write32(SPIF_BASE + SPIF_GAR,
		read32(SPIF_BASE + SPIF_GAR) | SPIF_FIFO_RESET);
	value = read32(SPIF_BASE + SPIF_GCR);
	value &= ~(BIT(17) | BIT(18) | BIT(16) | BIT(15) | BIT(13) |
		   BIT(5) | BIT(4) | SPIF_NORMAL_MODE | SPIF_DMA_MODE);
	value |= SPIF_CS_ACTIVE_LOW;
	write32(SPIF_BASE + SPIF_GCR, value);
	write32(SPIF_BASE + SPIF_CSD, (5U << 16) | (6U << 8) | 6U);
	write32(SPIF_BASE + SPIF_INT_STATUS,
		SPIF_DMA_DONE | SPIF_ERROR_MASK);
	return 0;
}

static int spif_cpu_transfer(const struct spif_descriptor *desc)
{
	u32 value;
	u32 timeout = SPIF_TIMEOUT;

	write32(SPIF_BASE + SPIF_PHASE, desc->phase);
	write32(SPIF_BASE + SPIF_FLASH_ADDR, desc->flash_addr);
	write32(SPIF_BASE + SPIF_BUS_WIDTH, desc->bus_width);
	write32(SPIF_BASE + SPIF_TRANSFER_NUM, desc->transfer_len);

	value = read32(SPIF_BASE + SPIF_GCR) & ~SPIF_DMA_MODE;
	write32(SPIF_BASE + SPIF_GCR, value | SPIF_NORMAL_MODE);
	while (read32(SPIF_BASE + SPIF_GCR) & SPIF_NORMAL_MODE) {
		if (!--timeout)
			return -4;
	}
	return 0;
}

static int spif_dma_transfer(struct spif_descriptor *desc, void *buffer,
			     u32 length, int receive)
{
	u32 status;
	u32 timeout = SPIF_TIMEOUT;

	cache_clean(desc, sizeof(*desc));
	cache_clean(buffer, length);
	__asm__ volatile ("fence iorw, iorw" : : : "memory");

	write32(SPIF_BASE + SPIF_GAR,
		read32(SPIF_BASE + SPIF_GAR) | SPIF_FIFO_RESET);
	write32(SPIF_BASE + SPIF_GCR,
		(read32(SPIF_BASE + SPIF_GCR) & ~SPIF_NORMAL_MODE) |
		SPIF_DMA_MODE);
	write32(SPIF_BASE + SPIF_INT_STATUS,
		SPIF_DMA_DONE | SPIF_ERROR_MASK);
	write32(SPIF_BASE + SPIF_DESC_ADDR, (uintptr_t)desc >> 2);
	write32(SPIF_BASE + SPIF_DMA_CTL,
		read32(SPIF_BASE + SPIF_DMA_CTL) | SPIF_DESC_LEN | BIT(0));

	do {
		status = read32(SPIF_BASE + SPIF_INT_STATUS);
		if (status & SPIF_ERROR_MASK)
			return -5;
		if (!--timeout)
			return -6;
	} while (!(status & SPIF_DMA_DONE));

	write32(SPIF_BASE + SPIF_INT_STATUS,
		SPIF_DMA_DONE | SPIF_ERROR_MASK);
	if (receive)
		cache_invalidate(buffer, length);
	return 0;
}

static u32 data_count(u32 length)
{
	return length == SPIF_MAX_DATA ? BIT(31) : length;
}

static int opcode_has_address(u8 opcode)
{
	return opcode == 0x02 || opcode == 0x03 || opcode == 0x20 ||
	       opcode == 0x52 || opcode == 0xd8 || opcode == 0x5a;
}

static int spif_transfer(const u8 *tx, u32 tx_length, u8 *rx,
			 u32 rx_length)
{
	const u8 *data = 0;
	u8 *dma_buffer = 0;
	u32 address = 0;
	u32 data_length = 0;
	u32 dma_length = 0;
	u32 header_length = 1;
	u32 phase = SPIF_PHASE_CMD;
	u32 dummy_cycles = 0;
	int receive = 0;
	int result;

	if (!tx || !tx_length || tx_length > SPIF_MAX_DATA ||
	    rx_length > SPIF_MAX_DATA)
		return -1;

	clear_bytes(&descriptor, sizeof(descriptor));
	descriptor.burst = SPIF_DESC_LAST | SPIF_DESC_BURST16;
	descriptor.bus_width = (u32)tx[0] << 24;
	descriptor.transfer_len = SPIF_DESC_NORMAL;

	if (opcode_has_address(tx[0])) {
		if (tx_length < 4)
			return -1;
		address = ((u32)tx[1] << 16) | ((u32)tx[2] << 8) | tx[3];
		header_length = 4;
		phase |= SPIF_PHASE_ADDR;
		if (read32(SPIF_BASE + SPIF_VERSION) >= 0x10002U)
			descriptor.transfer_len |= SPIF_ADDR_24BIT_V2;
	}

	if (tx[0] == 0x5a) {
		if (tx_length < 5)
			return -1;
		header_length = 5;
		dummy_cycles = 8;
		phase |= SPIF_PHASE_DUMMY;
	}

	if (rx_length) {
		receive = 1;
		data_length = rx_length;
		dma_length = data_length < SPIF_MIN_RX ? SPIF_MIN_RX : data_length;
		dma_buffer = data_length < SPIF_MIN_RX ? bounce : rx;
		phase |= SPIF_PHASE_RX;
		descriptor.burst |= SPIF_DESC_DMA_WRITE;
	} else if (tx_length > header_length) {
		data = tx + header_length;
		data_length = tx_length - header_length;
		dma_length = data_length;
		if (data_length > sizeof(bounce))
			return -1;
		copy_bytes(bounce, data, data_length);
		dma_buffer = bounce;
		phase |= SPIF_PHASE_TX;
	}

	descriptor.phase = phase;
	descriptor.flash_addr = address;
	descriptor.transfer_len |= dummy_cycles << 16;

	if (!dma_length)
		return spif_cpu_transfer(&descriptor);

	descriptor.block_len = SPIF_DESC_BLOCK64 | dma_length;
	descriptor.data_addr = (uintptr_t)dma_buffer >> 2;
	descriptor.transfer_len |= data_count(dma_length);

	result = spif_dma_transfer(&descriptor, dma_buffer, dma_length, receive);
	if (result)
		return result;
	if (receive && data_length < SPIF_MIN_RX)
		copy_bytes(rx, bounce, data_length);
	if (receive)
		cache_clean(rx, data_length);
	return 0;
}

static int spinor_wait_ready(void)
{
	u8 command = 0x05;
	u8 status;
	u32 timeout = SPIF_TIMEOUT;

	do {
		if (spif_transfer(&command, 1, &status, 1))
			return -1;
		if (!(status & BIT(0)))
			return 0;
	} while (--timeout);
	return -1;
}

static u32 get_u32(const u8 *bytes)
{
	return (u32)bytes[0] | ((u32)bytes[1] << 8) |
	       ((u32)bytes[2] << 16) | ((u32)bytes[3] << 24);
}

static int valid_swap_buffer(uintptr_t address, u32 length)
{
	if (address < SWAP_START || address > SWAP_END)
		return 0;
	return length <= SWAP_END - address;
}

void sys_spif_run(void *command_buffer)
{
	u8 *command = command_buffer;
	const u8 *tx = 0;
	u8 *rx = 0;
	u32 tx_length = 0;
	u32 rx_length = 0;
	int result = 0;

	while ((uintptr_t)command < COMMAND_END) {
		u8 opcode = *command++;

		if (opcode == SPI_CMD_END)
			break;
		if (opcode == SPI_CMD_INIT) {
			result = spif_init();
		} else if (opcode == SPI_CMD_SELECT) {
			tx = 0;
			rx = 0;
			tx_length = 0;
			rx_length = 0;
		} else if (opcode == SPI_CMD_DESELECT) {
			if (tx_length || rx_length)
				result = spif_transfer(tx, tx_length, rx,
						       rx_length);
			tx = 0;
			rx = 0;
			tx_length = 0;
			rx_length = 0;
		} else if (opcode == SPI_CMD_FAST) {
			tx_length = *command++;
			if (!tx_length || command + tx_length >
			    (u8 *)COMMAND_END) {
				result = -1;
				break;
			}
			tx = command;
			command += tx_length;
		} else if (opcode == SPI_CMD_TXBUF ||
			   opcode == SPI_CMD_RXBUF) {
			uintptr_t address;
			u32 length;

			if (command + 8 > (u8 *)COMMAND_END) {
				result = -1;
				break;
			}
			address = get_u32(command);
			length = get_u32(command + 4);
			command += 8;
			if (!valid_swap_buffer(address, length)) {
				result = -1;
				break;
			}
			if (opcode == SPI_CMD_TXBUF) {
				tx = (const u8 *)address;
				tx_length = length;
			} else {
				rx = (u8 *)address;
				rx_length = length;
			}
		} else if (opcode == SPI_CMD_SPINOR_WAIT) {
			result = spinor_wait_ready();
		} else if (opcode == SPI_CMD_SPINAND_WAIT) {
			result = -1;
		} else {
			result = -1;
		}

		if (result)
			break;
	}

	if (!result && (tx_length || rx_length))
		result = spif_transfer(tx, tx_length, rx, rx_length);
	write32(DIAGNOSTIC_ADDR, read32(SPIF_BASE + SPIF_VERSION));
	write32(DIAGNOSTIC_ADDR + 4, read32(SPIF_BASE + SPIF_GCR));
	write32(DIAGNOSTIC_ADDR + 8, read32(SPIF_BASE + SPIF_GAR));
	write32(DIAGNOSTIC_ADDR + 12, read32(SPIF_BASE + SPIF_INT_STATUS));
	cache_clean((const void *)DIAGNOSTIC_ADDR, 16);
	write32(STATUS_ADDR, (u32)result);
	cache_clean((const void *)STATUS_ADDR, sizeof(u32));
}
