#include <fel.h>
#include <usb.h>

extern struct chip_t a10;
extern struct chip_t a13_a10s_r8;
extern struct chip_t a20;
extern struct chip_t a23;
extern struct chip_t a31;
extern struct chip_t a33_r16;
extern struct chip_t a40i_r40;
extern struct chip_t a50;
extern struct chip_t a523_a527_t527;
extern struct chip_t a64;
extern struct chip_t a80;
extern struct chip_t a83t;
extern struct chip_t d1_f133;
extern struct chip_t f1c100s_f1c200s_f1c500s;
extern struct chip_t h2_h3;
extern struct chip_t h5;
extern struct chip_t h6;
extern struct chip_t h616;
extern struct chip_t r128;
extern struct chip_t r328;
extern struct chip_t r329;
extern struct chip_t r528_t113;
extern struct chip_t r818;
extern struct chip_t v3s_s3;
extern struct chip_t v536;
extern struct chip_t v831;
extern struct chip_t v851_v853;
extern struct chip_t v821;
extern struct chip_t a733;
extern struct chip_t t536;
extern struct chip_t a537_a333;
extern struct chip_t h135;
extern struct chip_t t153;


static struct chip_t * chips[] = {
	&a10,
	&a13_a10s_r8,
	&a20,
	&a23,
	&a31,
	&a33_r16,
	&a40i_r40,
	&a50,
	&a523_a527_t527,
	&a64,
	&a80,
	&a83t,
	&d1_f133,
	&f1c100s_f1c200s_f1c500s,
	&h2_h3,
	&h5,
	&h6,
	&h616,
	&r128,
	&r328,
	&r329,
	&r528_t113,
	&r818,
	&v3s_s3,
	&v536,
	&v831,
	&v851_v853,
	&v821,
	&a733,
	&t536,
	&a537_a333,
	&h135,
	&t153,
};

struct fel_request_t {
	uint32_t request;
	uint32_t address;
	uint32_t length;
	uint32_t pad;
} __attribute__((packed));

static inline void send_fel_request(struct xfel_ctx_t * ctx, int type, uint32_t addr, uint32_t length)
{
	struct fel_request_t req = {
		.request = cpu_to_le32(type),
		.address = cpu_to_le32(addr),
		.length = cpu_to_le32(length)
	};
	usb_write(ctx, &req, sizeof(struct fel_request_t));
}

static inline void read_fel_status(struct xfel_ctx_t * ctx)
{
	char buf[8];
	usb_read(ctx, buf, sizeof(buf));
}

static inline int fel_version(struct xfel_ctx_t * ctx)
{
	int i;

	send_fel_request(ctx, 0x001, 0, 0);
	usb_read(ctx, &ctx->version, sizeof(ctx->version));
	read_fel_status(ctx);
	ctx->version.id = le32_to_cpu(ctx->version.id);
	ctx->version.firmware = le32_to_cpu(ctx->version.firmware);
	ctx->version.protocol = le16_to_cpu(ctx->version.protocol);
	ctx->version.scratchpad = le32_to_cpu(ctx->version.scratchpad);
	for(i = 0; i < ARRAY_SIZE(chips); i++)
	{
		ctx->chip = chips[i];
		if(fel_chip_detect(ctx, ctx->version.id))
			return 1;
	}
	printf("WARNING: Not yet support this device ID 0x%08x\r\n", ctx->version.id);
	return 0;
}

int fel_init(struct xfel_ctx_t * ctx)
{
	if(usb_init(ctx))
	{
		return fel_version(ctx);
	}
	return 0;
}

void fel_exec(struct xfel_ctx_t * ctx, uint32_t addr)
{
	send_fel_request(ctx, 0x102, addr, 0);
	read_fel_status(ctx);
}

static inline void fel_read_raw(struct xfel_ctx_t * ctx, uint32_t addr, void * buf, size_t len)
{
	send_fel_request(ctx, 0x103, addr, len);
	usb_read(ctx, buf, len);
	read_fel_status(ctx);
}

static inline void fel_write_raw(struct xfel_ctx_t * ctx, uint32_t addr, void * buf, size_t len)
{
	send_fel_request(ctx, 0x101, addr, len);
	usb_write(ctx, buf, len);
	read_fel_status(ctx);
}

uint32_t fel_read32(struct xfel_ctx_t * ctx, uint32_t addr)
{
	uint32_t val;
	fel_read_raw(ctx, addr, &val, sizeof(uint32_t));
	return val;
}

void fel_write32(struct xfel_ctx_t * ctx, uint32_t addr, uint32_t val)
{
	fel_write_raw(ctx, addr, &val, sizeof(uint32_t));
}

void fel_read(struct xfel_ctx_t * ctx, uint32_t addr, void * buf, size_t len)
{
	size_t n;

	while(len > 0)
	{
		n = len > 65536 ? 65536 : len;
		fel_read_raw(ctx, addr, buf, n);
		addr += n;
		buf += n;
		len -= n;
	}
}

void fel_write(struct xfel_ctx_t * ctx, uint32_t addr, void * buf, size_t len)
{
	size_t n;

	while(len > 0)
	{
		n = len > 65536 ? 65536 : len;
		fel_write_raw(ctx, addr, buf, n);
		addr += n;
		buf += n;
		len -= n;
	}
}

void fel_read_progress(struct xfel_ctx_t * ctx, uint32_t addr, void * buf, size_t len)
{
	struct progress_t p;
	size_t n;

	progress_start(&p, len);
	while(len > 0)
	{
		n = len > 65536 ? 65536 : len;
		fel_read_raw(ctx, addr, buf, n);
		addr += n;
		buf += n;
		len -= n;
		progress_update(&p, n);
	}
	progress_stop(&p);
}

void fel_write_progress(struct xfel_ctx_t * ctx, uint32_t addr, void * buf, size_t len)
{
	struct progress_t p;
	size_t n;

	progress_start(&p, len);
	while(len > 0)
	{
		n = len > 65536 ? 65536 : len;
		fel_write_raw(ctx, addr, buf, n);
		addr += n;
		buf += n;
		len -= n;
		progress_update(&p, n);
	}
	progress_stop(&p);
}

int fel_spi_init(struct xfel_ctx_t * ctx, uint32_t * swapbuf, uint32_t * swaplen, uint32_t * cmdlen)
{
	uint8_t cbuf[2];

	if(!fel_chip_spi_init(ctx, swapbuf, swaplen, cmdlen))
		return 0;
	cbuf[0] = SPI_CMD_INIT;
	cbuf[1] = SPI_CMD_END;
	if(!fel_chip_spi_run(ctx, cbuf, sizeof(cbuf)))
		return 0;
	return 1;
}

int fel_spi_xfer(struct xfel_ctx_t * ctx, uint32_t swapbuf, uint32_t swaplen, uint32_t cmdlen, void * txbuf, uint32_t txlen, void * rxbuf, uint32_t rxlen)
{
	uint8_t cbuf[256];
	uint32_t clen;
	uint32_t n;

	if((txlen <= swaplen) && (rxlen <= swaplen))
	{
		clen = 0;
		cbuf[clen++] = SPI_CMD_SELECT;
		if(txlen > 0)
		{
			cbuf[clen++] = SPI_CMD_TXBUF;
			cbuf[clen++] = (swapbuf >>  0) & 0xff;
			cbuf[clen++] = (swapbuf >>  8) & 0xff;
			cbuf[clen++] = (swapbuf >> 16) & 0xff;
			cbuf[clen++] = (swapbuf >> 24) & 0xff;
			cbuf[clen++] = (txlen >>  0) & 0xff;
			cbuf[clen++] = (txlen >>  8) & 0xff;
			cbuf[clen++] = (txlen >> 16) & 0xff;
			cbuf[clen++] = (txlen >> 24) & 0xff;
		}
		if(rxlen > 0)
		{
			cbuf[clen++] = SPI_CMD_RXBUF;
			cbuf[clen++] = (swapbuf >>  0) & 0xff;
			cbuf[clen++] = (swapbuf >>  8) & 0xff;
			cbuf[clen++] = (swapbuf >> 16) & 0xff;
			cbuf[clen++] = (swapbuf >> 24) & 0xff;
			cbuf[clen++] = (rxlen >>  0) & 0xff;
			cbuf[clen++] = (rxlen >>  8) & 0xff;
			cbuf[clen++] = (rxlen >> 16) & 0xff;
			cbuf[clen++] = (rxlen >> 24) & 0xff;
		}
		cbuf[clen++] = SPI_CMD_DESELECT;
		cbuf[clen++] = SPI_CMD_END;
		if(txlen > 0)
			fel_write(ctx, swapbuf, txbuf, txlen);
		if((clen > cmdlen) || !fel_chip_spi_run(ctx, cbuf, clen))
			return 0;
		if(rxlen > 0)
			fel_read(ctx, swapbuf, rxbuf, rxlen);
	}
	else
	{
		clen = 0;
		cbuf[clen++] = SPI_CMD_SELECT;
		cbuf[clen++] = SPI_CMD_END;
		if(!fel_chip_spi_run(ctx, cbuf, clen))
			return 0;
		while(txlen > 0)
		{
			n = txlen > swaplen ? swaplen : txlen;
			clen = 0;
			cbuf[clen++] = SPI_CMD_TXBUF;
			cbuf[clen++] = (swapbuf >>  0) & 0xff;
			cbuf[clen++] = (swapbuf >>  8) & 0xff;
			cbuf[clen++] = (swapbuf >> 16) & 0xff;
			cbuf[clen++] = (swapbuf >> 24) & 0xff;
			cbuf[clen++] = (n >>  0) & 0xff;
			cbuf[clen++] = (n >>  8) & 0xff;
			cbuf[clen++] = (n >> 16) & 0xff;
			cbuf[clen++] = (n >> 24) & 0xff;
			cbuf[clen++] = SPI_CMD_END;
			fel_write(ctx, swapbuf, txbuf, n);
			if(!fel_chip_spi_run(ctx, cbuf, clen))
				return 0;
			txbuf += n;
			txlen -= n;
		}
		while(rxlen > 0)
		{
			n = rxlen > swaplen ? swaplen : rxlen;
			clen = 0;
			cbuf[clen++] = SPI_CMD_RXBUF;
			cbuf[clen++] = (swapbuf >>  0) & 0xff;
			cbuf[clen++] = (swapbuf >>  8) & 0xff;
			cbuf[clen++] = (swapbuf >> 16) & 0xff;
			cbuf[clen++] = (swapbuf >> 24) & 0xff;
			cbuf[clen++] = (n >>  0) & 0xff;
			cbuf[clen++] = (n >>  8) & 0xff;
			cbuf[clen++] = (n >> 16) & 0xff;
			cbuf[clen++] = (n >> 24) & 0xff;
			cbuf[clen++] = SPI_CMD_END;
			if(!fel_chip_spi_run(ctx, cbuf, clen))
				return 0;
			fel_read(ctx, swapbuf, rxbuf, n);
			rxbuf += n;
			rxlen -= n;
		}
		clen = 0;
		cbuf[clen++] = SPI_CMD_DESELECT;
		cbuf[clen++] = SPI_CMD_END;
		if((clen > cmdlen) || !fel_chip_spi_run(ctx, cbuf, clen))
			return 0;
	}
	return 1;
}
