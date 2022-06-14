#include <fel.h>

static int chip_detect(struct xfel_ctx_t * ctx, uint32_t id)
{
	if(id == 0x00182100)
		return 1;
	return 0;
}

static int chip_reset(struct xfel_ctx_t * ctx)
{
	return 0;
}

static uint32_t payload_arm_read32(struct xfel_ctx_t * ctx, uint32_t addr)
{
	uint32_t payload[] = {
		cpu_to_le32(0xe59f000c), /* ldr r0, [pc, #12] */
		cpu_to_le32(0xe28f100c), /* add r1, pc, #12   */
		cpu_to_le32(0xe4902000), /* ldr r2, [r0], #0  */
		cpu_to_le32(0xe4812000), /* str r2, [r1], #0  */
		cpu_to_le32(0xe12fff1e), /* bx lr             */
		cpu_to_le32(addr),
	};
	uint32_t val;

	fel_write(ctx, ctx->version.scratchpad, (void *)payload, sizeof(payload));
	fel_exec(ctx, ctx->version.scratchpad);
	fel_read(ctx, ctx->version.scratchpad + sizeof(payload), (void *)&val, sizeof(val));
	return le32_to_cpu(val);
}

static int chip_sid(struct xfel_ctx_t * ctx, char * sid)
{
	uint32_t id[4];

	id[0] = payload_arm_read32(ctx, 0x03006200 + 0x0);
	id[1] = payload_arm_read32(ctx, 0x03006200 + 0x4);
	id[2] = payload_arm_read32(ctx, 0x03006200 + 0x8);
	id[3] = payload_arm_read32(ctx, 0x03006200 + 0xc);
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

struct chip_t r328 = {
	.name = "R328",
	.detect = chip_detect,
	.reset = chip_reset,
	.sid = chip_sid,
	.jtag = chip_jtag,
	.ddr = chip_ddr,
	.spi_init = chip_spi_init,
	.spi_run = chip_spi_run,
};
