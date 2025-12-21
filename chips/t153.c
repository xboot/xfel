#include <fel.h>


static int chip_detect(struct xfel_ctx_t * ctx, uint32_t id)
{
	if(id == 0x00192200)
	{
		return 1;
	}
	return 0;
}

static uint32_t payload_read32(struct xfel_ctx_t * ctx, uint32_t addr)
{
	static const uint8_t payload[] = {
		0x00, 0x00, 0xa0, 0xe3, 0x17, 0x0f, 0x08, 0xee, 0x15, 0x0f, 0x07, 0xee,
		0xd5, 0x0f, 0x07, 0xee, 0x9a, 0x0f, 0x07, 0xee, 0x95, 0x0f, 0x07, 0xee,
		0xff, 0xff, 0xff, 0xea, 0x0c, 0x00, 0x9f, 0xe5, 0x0c, 0x10, 0x8f, 0xe2,
		0x00, 0x20, 0x90, 0xe5, 0x00, 0x20, 0x81, 0xe5, 0x1e, 0xff, 0x2f, 0xe1,
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
		0x00, 0x00, 0xa0, 0xe3, 0x17, 0x0f, 0x08, 0xee, 0x15, 0x0f, 0x07, 0xee,
		0xd5, 0x0f, 0x07, 0xee, 0x9a, 0x0f, 0x07, 0xee, 0x95, 0x0f, 0x07, 0xee,
		0xff, 0xff, 0xff, 0xea, 0x08, 0x00, 0x9f, 0xe5, 0x08, 0x10, 0x9f, 0xe5,
		0x00, 0x10, 0x80, 0xe5, 0x1e, 0xff, 0x2f, 0xe1,
	};
	uint32_t params[2] = {
		cpu_to_le32(addr),
		cpu_to_le32(val),
	};

	fel_write(ctx, ctx->version.scratchpad, (void *)payload, sizeof(payload));
	fel_write(ctx, ctx->version.scratchpad + sizeof(payload), (void *)params, sizeof(params));
	fel_exec(ctx, ctx->version.scratchpad);
}

static int chip_reset(struct xfel_ctx_t * ctx)
{
	return 0;
}

static int chip_sid(struct xfel_ctx_t * ctx, char * sid)
{
	return 0;
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
	return 0;
}

static int chip_spi_run(struct xfel_ctx_t * ctx, uint8_t * cbuf, uint32_t clen)
{
	return 0;
}

static int chip_extra(struct xfel_ctx_t * ctx, int argc, char * argv[])
{
	return 0;
}

struct chip_t t153 = {
	.name = "T153",
	.detect = chip_detect,
	.reset = chip_reset,
	.sid = chip_sid,
	.jtag = chip_jtag,
	.ddr = chip_ddr,
	.spi_init = chip_spi_init,
	.spi_run = chip_spi_run,
	.extra = chip_extra,
};
