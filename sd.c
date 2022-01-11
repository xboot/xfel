#include <sd.h>

struct sd_info_t {
	char * name;
	uint64_t capacity;
	uint32_t blksz;
};

struct sd_pdata_t {
	struct sd_info_t info;
	uint32_t swapbuf;
	uint32_t swaplen;
	uint32_t cmdlen;
};

static struct sd_pdata_t pdat_sd;

static inline int sd_info(struct xfel_ctx_t * ctx, struct sd_pdata_t * pdat)
{
	uint8_t cbuf[1+4*2+1];
	uint32_t clen = 0;

	uint32_t swapbuf = pdat->swapbuf;
	uint32_t rxlen = 8;

	cbuf[clen++] = SD_CMD_INFO;

	cbuf[clen++] = (swapbuf >> 0) & 0xff;
	cbuf[clen++] = (swapbuf >> 8) & 0xff;
	cbuf[clen++] = (swapbuf >> 16) & 0xff;
	cbuf[clen++] = (swapbuf >> 24) & 0xff;

	cbuf[clen++] = SD_CMD_END;
	if(!fel_chip_sd_run(ctx, cbuf, clen))
	{
		return 0;
	}

	pdat->info.capacity = 0;
	fel_read(ctx, swapbuf, &pdat->info.capacity, 8);

	return 1;
}

static int sd_helper_init(struct xfel_ctx_t * ctx, struct sd_pdata_t * pdat)
{
	if(fel_sd_init(ctx, &pdat->swapbuf, &pdat->swaplen, &pdat->cmdlen))
	{
		return 1;
	}
	return 0;
}

int sd_detect(struct xfel_ctx_t * ctx, char * name, uint64_t * capacity)
{
	if (sd_helper_init(ctx, &pdat_sd))
	{
		if (name)
			strcpy(name, "SD");

		if (capacity && (sd_info(ctx, &pdat_sd)))
		{
			*capacity = pdat_sd.info.capacity;
		}

		return 1;
	}

	return 0;
}

static void sd_helper_read(struct xfel_ctx_t *ctx, struct sd_pdata_t *pdat, uint64_t addr, uint8_t *buf, uint32_t len)
{
	uint8_t cbuf[1 + 8 + 4 * 2 + 1];
	uint32_t clen = 0;
	uint32_t swapbuf = pdat->swapbuf;

	// printf("func=%s L=%d. addr=%lld, len=%d\r\n", __FUNCTION__, __LINE__, addr, len);

	cbuf[clen++] = SD_CMD_READ; // u64:sd-addr, u32:len, u32:buf. ret_buf=[len]

	cbuf[clen++] = (addr >> 0) & 0xff;
	cbuf[clen++] = (addr >> 8) & 0xff;
	cbuf[clen++] = (addr >> 16) & 0xff;
	cbuf[clen++] = (addr >> 24) & 0xff;
	cbuf[clen++] = (addr >> 32) & 0xff;
	cbuf[clen++] = (addr >> 40) & 0xff;
	cbuf[clen++] = (addr >> 48) & 0xff;
	cbuf[clen++] = (addr >> 56) & 0xff;

	cbuf[clen++] = (swapbuf >> 0) & 0xff;
	cbuf[clen++] = (swapbuf >> 8) & 0xff;
	cbuf[clen++] = (swapbuf >> 16) & 0xff;
	cbuf[clen++] = (swapbuf >> 24) & 0xff;

	cbuf[clen++] = SD_CMD_END;
	if (!fel_chip_sd_run(ctx, cbuf, clen))
	{
		printf("func=%s L=%d. addr=%I64d, len=%d. read error\r\n", __FUNCTION__, __LINE__, addr, len);
		return;
	}

	// printf("fel_read 0x%x\r\n", swapbuf);
	fel_read(ctx, swapbuf, buf, len);
}

int sd_read(struct xfel_ctx_t *ctx, uint64_t addr, void *buf, uint64_t len)
{
	struct progress_t p;
	uint64_t n;
	uint32_t count;

	// printf("func=%s L=%d. addr=%I64d, len=%I64d\r\n", __FUNCTION__, __LINE__, addr, len);
	addr = addr & ~0x1FFULL;
	len = (len + 511) & ~0x1FFULL;
	// printf("func=%s L=%d. addr=%I64d, len=%I64d\r\n", __FUNCTION__, __LINE__, addr, len);

	if (!sd_helper_init(ctx, &pdat_sd))
	{
		printf("Can't detect any sd card\r\n");
		return 0;
	}

	if (!sd_info(ctx, &pdat_sd))
	{
		printf("Can't detect any sd card\r\n");
		return 0;
	}

	printf("swap buf:%08X,len=%d, cmdlen=%d\r\n", pdat_sd.swapbuf, pdat_sd.swaplen, pdat_sd.cmdlen);
	printf("Found SD card %lld bytes\r\n", (long long)pdat_sd.info.capacity);

	count = 0;
	if (1)
	{
		progress_start(&p, len);
		while (len > 0)
		{
			n = 512; // sector size
			sd_helper_read(ctx, &pdat_sd, addr, buf, n);
			addr += n;
			len -= n;
			buf += n;

			count += n;
			if(count > 1024*32 )
			{
				progress_update(&p, count);
				count = 0;
			}
		}
		progress_update(&p, count);
		progress_stop(&p);
		return 1;
	}

	return 1;
}

static void sd_helper_write(struct xfel_ctx_t *ctx, struct sd_pdata_t *pdat, uint64_t addr, uint8_t *buf, uint32_t len)
{
	uint8_t cbuf[1 + 8 + 4 * 2 + 1];
	uint32_t clen = 0;
	uint32_t swapbuf = pdat->swapbuf;

	// printf("func=%s L=%d. addr=%lld, len=%d\r\n", __FUNCTION__, __LINE__, addr, len);

	cbuf[clen++] = SD_CMD_WRITE; // u64:sd-addr, u32:len, u32:buf. ret_buf=[len]

	cbuf[clen++] = (addr >> 0) & 0xff;
	cbuf[clen++] = (addr >> 8) & 0xff;
	cbuf[clen++] = (addr >> 16) & 0xff;
	cbuf[clen++] = (addr >> 24) & 0xff;
	cbuf[clen++] = (addr >> 32) & 0xff;
	cbuf[clen++] = (addr >> 40) & 0xff;
	cbuf[clen++] = (addr >> 48) & 0xff;
	cbuf[clen++] = (addr >> 56) & 0xff;

	cbuf[clen++] = (swapbuf >> 0) & 0xff;
	cbuf[clen++] = (swapbuf >> 8) & 0xff;
	cbuf[clen++] = (swapbuf >> 16) & 0xff;
	cbuf[clen++] = (swapbuf >> 24) & 0xff;

	cbuf[clen++] = SD_CMD_END;

	fel_write(ctx, pdat->swapbuf, buf, len);
	if (!fel_chip_sd_run(ctx, cbuf, clen))
	{
		// printf("func=%s L=%d. addr=%I64d, len=%d. write error\r\n", __FUNCTION__, __LINE__, addr, len);
		return;
	}
}

int sd_write(struct xfel_ctx_t *ctx, uint64_t addr, void *buf, uint64_t len)
{
	struct progress_t p;
	uint64_t n;
	uint32_t count;

	// printf("func=%s L=%d. addr=%I64d, len=%I64d\r\n", __FUNCTION__, __LINE__, addr, len);
	addr = addr & ~0x1FFULL;
	len = (len + 511) & ~0x1FFULL;
	// printf("func=%s L=%d. addr=%I64d, len=%I64d\r\n", __FUNCTION__, __LINE__, addr, len);

	if (!sd_helper_init(ctx, &pdat_sd))
	{
		printf("Can't detect any sd card\r\n");
		return 0;
	}

	if (!sd_info(ctx, &pdat_sd))
	{
		printf("Can't detect any sd card\r\n");
		return 0;
	}

	printf("swap buf:%08X,len=%d, cmdlen=%d\r\n", pdat_sd.swapbuf, pdat_sd.swaplen, pdat_sd.cmdlen);
	printf("Found SD card %lld bytes\r\n", (long long)pdat_sd.info.capacity);

	count = 0;
	if (1)
	{
		progress_start(&p, len);
		progress_update(&p, count);
		while (len > 0)
		{
			n = 512; // sector size
			sd_helper_write(ctx, &pdat_sd, addr, buf, n);
			addr += n;
			len -= n;
			buf += n;

			count += n;
			if(count > 1024*32 )
			{
				progress_update(&p, count);
				count = 0;
			}
		}
		progress_update(&p, count);
		progress_stop(&p);
		return 1;
	}

	return 0;
}

