#include <fel.h>

static int chip_detect(struct xfel_ctx_t * ctx, uint32_t id)
{
	if(id == 0x00188200)
	{
		return 1;
	}
	return 0;
}

static uint32_t payload_read32(struct xfel_ctx_t * ctx, uint32_t addr)
{
	static const uint8_t payload[] = {
		0x37, 0x03, 0x40, 0x00, 0x73, 0x20, 0x43, 0x7c, 0x0f, 0x10, 0x00, 0x00,
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
		0x37, 0x03, 0x40, 0x00, 0x73, 0x20, 0x43, 0x7c, 0x0f, 0x10, 0x00, 0x00,
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

static int chip_reset(struct xfel_ctx_t * ctx)
{
	return 0;
}

static int chip_sid(struct xfel_ctx_t * ctx, char * sid)
{
	uint32_t id[4];

	id[0] = payload_read32(ctx, 0x43006200 + 0x0);
	id[1] = payload_read32(ctx, 0x43006200 + 0x4);
	id[2] = payload_read32(ctx, 0x43006200 + 0x8);
	id[3] = payload_read32(ctx, 0x43006200 + 0xc);
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

struct chip_t v821 = {
	.name = "V821",
	.detect = chip_detect,
	.reset = chip_reset,
	.sid = chip_sid,
	.jtag = chip_jtag,
	.ddr = chip_ddr,
	.spi_init = chip_spi_init,
	.spi_run = chip_spi_run,
	.extra = chip_extra,
};
