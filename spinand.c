#include <spinand.h>

/*
enum {
	OPCODE_RDID			= 0x9f,
};

static inline int spinand_read_id(
    struct xfel_ctx_t * ctx,
    uint32_t swapbuf,
    uint32_t swaplen,
    uint32_t cmdlen,
    uint32_t * id
) {
	uint8_t tx[1];
	uint8_t rx[3];

	tx[0] = OPCODE_RDID;
	if(!fel_spi_xfer(ctx, swapbuf, swaplen, cmdlen, tx, 1, rx, 3))
		return 0;
	*id = (rx[0] << 16) | (rx[1] << 8) | (rx[2] << 0);
	return 1;
}
*/

uint64_t spinand_detect(struct xfel_ctx_t * ctx)
{
	return 0;
}

int spinand_read(struct xfel_ctx_t * ctx, uint64_t addr, void * buf, uint64_t len)
{
	return 0;
}

int spinand_write(struct xfel_ctx_t * ctx, uint64_t addr, void * buf, uint64_t len)
{
	return 0;
}
