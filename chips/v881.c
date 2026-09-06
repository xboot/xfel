#include <fel.h>

#define V881_SPI_PAYLOAD_ADDR	0x00100000
#define V881_SPI_COMMAND_ADDR	0x00101000
#define V881_SPI_STATUS_ADDR	0x00101ffc
#define V881_SPI_SWAP_ADDR	0x00102000
#define V881_SPI_SWAP_SIZE	65536
#define V881_SPI_COMMAND_SIZE	256

static int chip_detect(struct xfel_ctx_t * ctx, uint32_t id)
{
	if(id == 0x00191800)
		return 1;
	return 0;
}

static uint32_t payload_read32(struct xfel_ctx_t * ctx, uint32_t addr)
{
	static const uint8_t payload[] = {
		0x37, 0x03, 0x40, 0x00, 0x73, 0x20, 0x03, 0x7c, 0x0f, 0x10, 0x00, 0x00,
		0x09, 0xa0, 0x97, 0x02, 0x00, 0x00, 0x93, 0x82, 0xe2, 0x01, 0x83, 0xa2,
		0x02, 0x00, 0x83, 0xa2, 0x02, 0x00, 0x17, 0x03, 0x00, 0x00, 0x13, 0x03,
		0x23, 0x01, 0x23, 0x20, 0x53, 0x00, 0x82, 0x80,
	};
	uint32_t adr = cpu_to_le32(addr);
	uint32_t val;

	fel_write(ctx, ctx->version.scratchpad, (void *)payload, sizeof(payload));
	fel_write(ctx, ctx->version.scratchpad + sizeof(payload), (void *)&adr, sizeof(adr));
	fel_exec(ctx, ctx->version.scratchpad);
	fel_read(ctx, ctx->version.scratchpad + sizeof(payload) + sizeof(adr), (void *)&val, sizeof(val));
	return le32_to_cpu(val);
}

static void payload_write32(struct xfel_ctx_t * ctx, uint32_t addr, uint32_t val)
{
	static const uint8_t payload[] = {
		0x37, 0x03, 0x40, 0x00, 0x73, 0x20, 0x03, 0x7c, 0x0f, 0x10, 0x00, 0x00,
		0x09, 0xa0, 0x97, 0x02, 0x00, 0x00, 0x93, 0x82, 0xe2, 0x01, 0x83, 0xa2,
		0x02, 0x00, 0x17, 0x03, 0x00, 0x00, 0x13, 0x03, 0x63, 0x01, 0x03, 0x23,
		0x03, 0x00, 0x23, 0xa0, 0x62, 0x00, 0x82, 0x80,
	};
	uint32_t params[2] = {
		cpu_to_le32(addr),
		cpu_to_le32(val),
	};

	fel_write(ctx, ctx->version.scratchpad, (void *)payload, sizeof(payload));
	fel_write(ctx, ctx->version.scratchpad + sizeof(payload), (void *)params, sizeof(params));
	fel_exec(ctx, ctx->version.scratchpad);
}

static int chip_read32(struct xfel_ctx_t * ctx, uint32_t addr, uint32_t * val)
{
	*val = payload_read32(ctx, addr);
	return 1;
}

static int chip_write32(struct xfel_ctx_t * ctx, uint32_t addr, uint32_t val)
{
	payload_write32(ctx, addr, val);
	return 1;
}

static int chip_reset(struct xfel_ctx_t * ctx)
{
	uint32_t val = payload_read32(ctx, 0x07090000 + 0x2a0);
	val |= 1 << 1;
	payload_write32(ctx, 0x07090000 + 0x2a0, val | (0x429b << 16));

	payload_write32(ctx, 0x08009000 + 0x18, (0x16aa << 16) | (0 << 0));
	payload_write32(ctx, 0x08009000 + 0x18, (0x16aa << 16) | 0x20 | (1 << 0));
	return 1;
}

static int chip_sid(struct xfel_ctx_t * ctx, char * sid)
{
	uint32_t id[4];

	id[0] = payload_read32(ctx, 0x07091200 + 0x0);
	id[1] = payload_read32(ctx, 0x07091200 + 0x4);
	id[2] = payload_read32(ctx, 0x07091200 + 0x8);
	id[3] = payload_read32(ctx, 0x07091200 + 0xc);
	sprintf(sid, "%08x%08x%08x%08x", id[0], id[1], id[2], id[3]);
	return 1;
}

static int chip_jtag(struct xfel_ctx_t * ctx)
{
	return 0;
}

static int chip_ddr(struct xfel_ctx_t * ctx, const char * type)
{
	return 0;
}

static int chip_spi_init(struct xfel_ctx_t * ctx, uint32_t * swapbuf, uint32_t * swaplen, uint32_t * cmdlen)
{
	static const uint8_t payload[] = {
#include "v881-spi.inc"
	};

	fel_write(ctx, V881_SPI_PAYLOAD_ADDR, (void *)payload, sizeof(payload));
	if(swapbuf)
		*swapbuf = V881_SPI_SWAP_ADDR;
	if(swaplen)
		*swaplen = V881_SPI_SWAP_SIZE;
	if(cmdlen)
		*cmdlen = V881_SPI_COMMAND_SIZE;
	return 1;
}

static int chip_spi_run(struct xfel_ctx_t * ctx, uint8_t * cbuf, uint32_t clen)
{
	uint32_t status = cpu_to_le32(-1);

	if(clen > V881_SPI_COMMAND_SIZE)
		return 0;
	fel_write(ctx, V881_SPI_COMMAND_ADDR, cbuf, clen);
	fel_write(ctx, V881_SPI_STATUS_ADDR, &status, sizeof(status));
	fel_exec(ctx, V881_SPI_PAYLOAD_ADDR);
	fel_read(ctx, V881_SPI_STATUS_ADDR, &status, sizeof(status));
	if(le32_to_cpu(status) != 0)
	{
		uint32_t regs[4];

		fel_read(ctx, 0x00101f80, regs, sizeof(regs));
		fprintf(stderr, "V881 SPIF error %d: ver=%08x gcr=%08x "
			"gar=%08x irq=%08x\n", (int32_t)le32_to_cpu(status),
			le32_to_cpu(regs[0]), le32_to_cpu(regs[1]),
			le32_to_cpu(regs[2]), le32_to_cpu(regs[3]));
	}
	return le32_to_cpu(status) == 0;
}

static int chip_extra(struct xfel_ctx_t * ctx, int argc, char * argv[])
{
	return 0;
}

struct chip_t v881 = {
	.name = "V881",
	.detect = chip_detect,
	.read32 = chip_read32,
	.write32 = chip_write32,
	.reset = chip_reset,
	.sid = chip_sid,
	.jtag = chip_jtag,
	.ddr = chip_ddr,
	.spi_init = chip_spi_init,
	.spi_run = chip_spi_run,
	.extra = chip_extra,
};
