#include <sdcard.h>

enum {
	/* Class 1 */
	MMC_GO_IDLE_STATE			= 0,
	MMC_SEND_OP_COND			= 1,
	MMC_ALL_SEND_CID			= 2,
	MMC_SET_RELATIVE_ADDR 		= 3,
	MMC_SET_DSR 				= 4,
	MMC_SWITCH 					= 6,
	MMC_SELECT_CARD 			= 7,
	MMC_SEND_EXT_CSD 			= 8,
	MMC_SEND_CSD 				= 9,
	MMC_SEND_CID				= 10,
	MMC_READ_DAT_UNTIL_STOP		= 11,
	MMC_STOP_TRANSMISSION 		= 12,
	MMC_SEND_STATUS 			= 13,
	MMC_GO_INACTIVE_STATE 		= 15,
	MMC_SPI_READ_OCR 			= 58,
	MMC_SPI_CRC_ON_OFF 			= 59,

	/* Class 2 */
	MMC_SET_BLOCKLEN 			= 16,
	MMC_READ_SINGLE_BLOCK 		= 17,
	MMC_READ_MULTIPLE_BLOCK		= 18,

	/* Class 3 */
	MMC_WRITE_DAT_UNTIL_STOP	= 20,

	/* Class 4 */
	MMC_SET_BLOCK_COUNT 		= 23,
	MMC_WRITE_SINGLE_BLOCK 		= 24,
	MMC_WRITE_MULTIPLE_BLOCK	= 25,
	MMC_PROGRAM_CID 			= 26,
	MMC_PROGRAM_CSD 			= 27,

	/* Class 5 */
	MMC_ERASE_GROUP_START 		= 35,
	MMC_ERASE_GROUP_END			= 36,
	MMC_ERASE					= 38,

	/* Class 6 */
	MMC_SET_WRITE_PROT			= 28,
	MMC_CLR_WRITE_PROT			= 29,
	MMC_SEND_WRITE_PROT			= 30,

	/* Class 7 */
	MMC_LOCK_UNLOCK				= 42,

	/* Class 8 */
	MMC_APP_CMD					= 55,
	MMC_GEN_CMD					= 56,

	/* Class 9 */
	MMC_FAST_IO					= 39,
	MMC_GO_IRQ_STATE			= 40,
};

enum {
	SD_CMD_SEND_RELATIVE_ADDR	= 3,
	SD_CMD_SWITCH_FUNC			= 6,
	SD_CMD_SEND_IF_COND			= 8,
	SD_CMD_APP_SET_BUS_WIDTH	= 6,
	SD_CMD_ERASE_WR_BLK_START	= 32,
	SD_CMD_ERASE_WR_BLK_END		= 33,
	SD_CMD_APP_SEND_OP_COND		= 41,
	SD_CMD_APP_SEND_SCR			= 51,
};

enum {
	MMC_RSP_PRESENT		= (1 << 0),
	MMC_RSP_136			= (1 << 1),
	MMC_RSP_CRC			= (1 << 2),
	MMC_RSP_BUSY		= (1 << 3),
	MMC_RSP_OPCODE		= (1 << 4),
};

enum {
	MMC_RSP_NONE		= (0 << 24),
	MMC_RSP_R1 			= (1 << 24) | (MMC_RSP_PRESENT | MMC_RSP_CRC | MMC_RSP_OPCODE),
	MMC_RSP_R1B 		= (1 << 24) | (MMC_RSP_PRESENT | MMC_RSP_CRC | MMC_RSP_OPCODE | MMC_RSP_BUSY),
	MMC_RSP_R2 			= (2 << 24) | (MMC_RSP_PRESENT | MMC_RSP_136 | MMC_RSP_CRC),
	MMC_RSP_R3 			= (3 << 24) | (MMC_RSP_PRESENT),
	MMC_RSP_R4 			= (4 << 24) | (MMC_RSP_PRESENT),
	MMC_RSP_R5 			= (5 << 24) | (MMC_RSP_PRESENT | MMC_RSP_CRC | MMC_RSP_OPCODE),
	MMC_RSP_R6 			= (6 << 24) | (MMC_RSP_PRESENT | MMC_RSP_CRC | MMC_RSP_OPCODE),
	MMC_RSP_R7 			= (7 << 24) | (MMC_RSP_PRESENT | MMC_RSP_CRC | MMC_RSP_OPCODE),
};

enum {
	MMC_STATUS_IDLE		= 0,
	MMC_STATUS_READY	= 1,
	MMC_STATUS_IDENT	= 2,
	MMC_STATUS_STBY		= 3,
	MMC_STATUS_TRAN		= 4,
	MMC_STATUS_DATA		= 5,
	MMC_STATUS_RCV		= 6,
	MMC_STATUS_PRG		= 7,
	MMC_STATUS_DIS		= 8,
	MMC_STATUS_BTST		= 9,
	MMC_STATUS_SLP		= 10,
};

enum {
	OCR_BUSY 			= 0x80000000,
	OCR_HCS 			= 0x40000000,
	OCR_VOLTAGE_MASK	= 0x00ffff80,
	OCR_ACCESS_MODE		= 0x60000000,
};

enum {
	MMC_DATA_READ		= (1 << 0),
	MMC_DATA_WRITE		= (1 << 1),
};

enum {
	MMC_VDD_27_36		= (1 << 0),
	MMC_VDD_165_195		= (1 << 1),
};

enum {
	MMC_BUS_WIDTH_1 	= (1 << 0),
	MMC_BUS_WIDTH_4 	= (1 << 2),
	MMC_BUS_WIDTH_8 	= (1 << 3),
};

enum {
	SD_VERSION_SD 		= 0x20000,
	SD_VERSION_3 		= (SD_VERSION_SD | 0x300),
	SD_VERSION_2 		= (SD_VERSION_SD | 0x200),
	SD_VERSION_1_0 		= (SD_VERSION_SD | 0x100),
	SD_VERSION_1_10 	= (SD_VERSION_SD | 0x10a),
	MMC_VERSION_MMC 	= 0x10000,
	MMC_VERSION_UNKNOWN	= (MMC_VERSION_MMC),
	MMC_VERSION_1_2 	= (MMC_VERSION_MMC | 0x102),
	MMC_VERSION_1_4 	= (MMC_VERSION_MMC | 0x104),
	MMC_VERSION_2_2 	= (MMC_VERSION_MMC | 0x202),
	MMC_VERSION_3 		= (MMC_VERSION_MMC | 0x300),
	MMC_VERSION_4 		= (MMC_VERSION_MMC | 0x400),
	MMC_VERSION_4_1 	= (MMC_VERSION_MMC | 0x401),
	MMC_VERSION_4_2 	= (MMC_VERSION_MMC | 0x402),
	MMC_VERSION_4_3 	= (MMC_VERSION_MMC | 0x403),
	MMC_VERSION_4_41 	= (MMC_VERSION_MMC | 0x429),
	MMC_VERSION_4_5 	= (MMC_VERSION_MMC | 0x405),
	MMC_VERSION_5_0 	= (MMC_VERSION_MMC | 0x500),
	MMC_VERSION_5_1 	= (MMC_VERSION_MMC | 0x501),
};

struct sdcard_info_t {
	uint32_t version;
	uint32_t ocr;
	uint32_t rca;
	uint32_t cid[4];
	uint32_t csd[4];
	uint8_t extcsd[512];

	uint32_t high_capacity;
	uint32_t tran_speed;
	uint32_t dsr_imp;
	uint32_t read_bl_len;
	uint32_t write_bl_len;
	uint64_t capacity;
};

struct sdcard_pdata_t {
	struct sdcard_info_t info;
	uint32_t swapbuf;
	uint32_t swaplen;
	uint32_t cmdlen;
	uint8_t buf[4096];
};

#define UNSTUFF_BITS(resp, start, size)									\
	({																	\
		const int __size = size;										\
		const uint32_t __mask = (__size < 32 ? 1 << __size : 0) - 1;	\
		const int __off = 3 - ((start) / 32);							\
		const int __shft = (start) & 31;								\
		uint32_t __res;													\
																		\
		__res = resp[__off] >> __shft;									\
		if(__size + __shft > 32)										\
			__res |= resp[__off-1] << ((32 - __shft) % 32);				\
		__res & __mask;													\
	})

static const unsigned tran_speed_unit[] = {
	[0] = 10000,
	[1] = 100000,
	[2] = 1000000,
	[3] = 10000000,
};

static const unsigned char tran_speed_time[] = {
	 0, 10, 12, 13, 15, 20, 25, 30,
	35, 40, 45, 50, 55, 60, 70, 80,
};

static int sdhci_reset(struct xfel_ctx_t * ctx)
{
	uint8_t cbuf[256];
	uint32_t clen;

	clen = 0;
	cbuf[clen++] = SDHCI_CMD_RESET;
	cbuf[clen++] = SDHCI_CMD_END;
	if(!fel_chip_sdhci_run(ctx, cbuf, clen))
		return 0;
	return 1;
}

static int sdhci_set_clock(struct xfel_ctx_t * ctx, uint32_t clock)
{
	uint8_t cbuf[256];
	uint32_t clen;

	clen = 0;
	cbuf[clen++] = SDHCI_CMD_SET_CLOCK;
	cbuf[clen++] = (clock >>  0) & 0xff;
	cbuf[clen++] = (clock >>  8) & 0xff;
	cbuf[clen++] = (clock >> 16) & 0xff;
	cbuf[clen++] = (clock >> 24) & 0xff;
	cbuf[clen++] = SDHCI_CMD_END;
	if(!fel_chip_sdhci_run(ctx, cbuf, clen))
		return 0;
	return 1;
}

static int sdhci_set_width(struct xfel_ctx_t * ctx, uint32_t width)
{
	uint8_t cbuf[256];
	uint32_t clen;

	clen = 0;
	cbuf[clen++] = SDHCI_CMD_SET_CLOCK;
	cbuf[clen++] = (width >>  0) & 0xff;
	cbuf[clen++] = (width >>  8) & 0xff;
	cbuf[clen++] = (width >> 16) & 0xff;
	cbuf[clen++] = (width >> 24) & 0xff;
	cbuf[clen++] = SDHCI_CMD_END;
	if(!fel_chip_sdhci_run(ctx, cbuf, clen))
		return 0;
	return 1;
}

static int go_idle_state(struct xfel_ctx_t * ctx, struct sdcard_pdata_t * pdat)
{
	struct sdhci_cmd_t cmd = { 0 };

	cmd.cmdidx = MMC_GO_IDLE_STATE;
	cmd.cmdarg = 0;
	cmd.resptype = MMC_RSP_NONE;
	if(fel_sdhci_xfer(ctx, pdat->swapbuf, pdat->swaplen, pdat->cmdlen, &cmd, NULL))
		return 1;
	return fel_sdhci_xfer(ctx, pdat->swapbuf, pdat->swaplen, pdat->cmdlen, &cmd, NULL);
}

static int sd_send_if_cond(struct xfel_ctx_t * ctx, struct sdcard_pdata_t * pdat)
{
	struct sdhci_cmd_t cmd = { 0 };

	cmd.cmdidx = SD_CMD_SEND_IF_COND;
	cmd.cmdarg = (0x1 << 8);
	cmd.cmdarg |= 0xaa;
	cmd.resptype = MMC_RSP_R7;
	if(!fel_sdhci_xfer(ctx, pdat->swapbuf, pdat->swaplen, pdat->cmdlen, &cmd, NULL))
		return 0;
	if((cmd.response[0] & 0xff) != 0xaa)
		return 0;
	pdat->info.version = SD_VERSION_2;
	return 1;
}

static int sd_send_op_cond(struct xfel_ctx_t * ctx, struct sdcard_pdata_t * pdat)
{
	struct sdhci_cmd_t cmd = { 0 };
	int retries = 10;

	do {
		cmd.cmdidx = MMC_APP_CMD;
		cmd.cmdarg = 0;
		cmd.resptype = MMC_RSP_R1;
		if(!fel_sdhci_xfer(ctx, pdat->swapbuf, pdat->swaplen, pdat->cmdlen, &cmd, NULL))
			continue;
		cmd.cmdidx = SD_CMD_APP_SEND_OP_COND;
		cmd.cmdarg = 0x00ff8000;
		if(pdat->info.version == SD_VERSION_2)
			cmd.cmdarg |= OCR_HCS;
		cmd.resptype = MMC_RSP_R3;
		if(!fel_sdhci_xfer(ctx, pdat->swapbuf, pdat->swaplen, pdat->cmdlen, &cmd, NULL) || (cmd.response[0] & OCR_BUSY))
			break;
	} while(retries--);
	if(retries <= 0)
		return 0;
	if(pdat->info.version != SD_VERSION_2)
		pdat->info.version = SD_VERSION_1_0;
	pdat->info.ocr = cmd.response[0];
	pdat->info.high_capacity = ((pdat->info.ocr & OCR_HCS) == OCR_HCS);
	pdat->info.rca = 0;
	return 1;
}

static int mmc_send_op_cond(struct xfel_ctx_t * ctx, struct sdcard_pdata_t * pdat)
{
	struct sdhci_cmd_t cmd = { 0 };
	int retries = 10;

	if(!go_idle_state(ctx, pdat))
		return 0;
	cmd.cmdidx = MMC_SEND_OP_COND;
	cmd.cmdarg = 0;
	cmd.resptype = MMC_RSP_R3;
	if(!fel_sdhci_xfer(ctx, pdat->swapbuf, pdat->swaplen, pdat->cmdlen, &cmd, NULL))
		return 0;
	do {
		cmd.cmdidx = MMC_SEND_OP_COND;
		cmd.cmdarg = (pdat->info.ocr & OCR_VOLTAGE_MASK) | (pdat->info.ocr & OCR_ACCESS_MODE);
		cmd.cmdarg |= OCR_HCS;
		cmd.resptype = MMC_RSP_R3;
		if(!fel_sdhci_xfer(ctx, pdat->swapbuf, pdat->swaplen, pdat->cmdlen, &cmd, NULL))
			return 0;
	} while (!(cmd.response[0] & OCR_BUSY) && retries--);
	if(retries <= 0)
		return 0;
	pdat->info.version = MMC_VERSION_UNKNOWN;
	pdat->info.ocr = cmd.response[0];
	pdat->info.high_capacity = ((pdat->info.ocr & OCR_HCS) == OCR_HCS);
	pdat->info.rca = 0;
	return 1;
}

static int mmc_status(struct xfel_ctx_t * ctx, struct sdcard_pdata_t * pdat)
{
	struct sdhci_cmd_t cmd = { 0 };
	int retries = 10;

	cmd.cmdidx = MMC_SEND_STATUS;
	cmd.resptype = MMC_RSP_R1;
	cmd.cmdarg = pdat->info.rca << 16;
	do
	{
		if(!fel_sdhci_xfer(ctx, pdat->swapbuf, pdat->swaplen, pdat->cmdlen, &cmd, NULL))
			continue;
		if(cmd.response[0] & (1 << 8))
			break;
		usleep(1);
	} while(retries-- > 0);
	if(retries > 0)
		return ((cmd.response[0] >> 9) & 0xf);
	return -1;
}

static uint64_t mmc_read_blocks(struct xfel_ctx_t * ctx, struct sdcard_pdata_t * pdat, uint8_t * buf, uint64_t start, uint64_t blkcnt)
{
	struct sdhci_cmd_t cmd = { 0 };
	struct sdhci_data_t dat = { 0 };
	int status;

	if(blkcnt > 1)
		cmd.cmdidx = MMC_READ_MULTIPLE_BLOCK;
	else
		cmd.cmdidx = MMC_READ_SINGLE_BLOCK;
	if(pdat->info.high_capacity)
		cmd.cmdarg = start;
	else
		cmd.cmdarg = start * pdat->info.read_bl_len;
	cmd.resptype = MMC_RSP_R1;
	dat.buf = buf;
	dat.flag = MMC_DATA_READ;
	dat.blksz = pdat->info.read_bl_len;
	dat.blkcnt = blkcnt;
	if(!fel_sdhci_xfer(ctx, pdat->swapbuf, pdat->swaplen, pdat->cmdlen, &cmd, &dat))
		return 0;
	do {
		status = mmc_status(ctx, pdat);
		if(status < 0)
			return 0;
	} while((status != MMC_STATUS_TRAN) && (status != MMC_STATUS_DATA));
	if(blkcnt > 1)
	{
		cmd.cmdidx = MMC_STOP_TRANSMISSION;
		cmd.cmdarg = 0;
		cmd.resptype = MMC_RSP_R1B;
		if(!fel_sdhci_xfer(ctx, pdat->swapbuf, pdat->swaplen, pdat->cmdlen, &cmd, NULL))
			return 0;
	}
	return blkcnt;
}

static uint64_t mmc_write_blocks(struct xfel_ctx_t * ctx, struct sdcard_pdata_t * pdat, uint8_t * buf, uint64_t start, uint64_t blkcnt)
{
	struct sdhci_cmd_t cmd = { 0 };
	struct sdhci_data_t dat = { 0 };
	int status;

	if(blkcnt > 1)
		cmd.cmdidx = MMC_WRITE_MULTIPLE_BLOCK;
	else
		cmd.cmdidx = MMC_WRITE_SINGLE_BLOCK;
	if(pdat->info.high_capacity)
		cmd.cmdarg = start;
	else
		cmd.cmdarg = start * pdat->info.write_bl_len;
	cmd.resptype = MMC_RSP_R1;
	dat.buf = buf;
	dat.flag = MMC_DATA_WRITE;
	dat.blksz = pdat->info.write_bl_len;
	dat.blkcnt = blkcnt;
	if(!fel_sdhci_xfer(ctx, pdat->swapbuf, pdat->swaplen, pdat->cmdlen, &cmd, &dat))
		return 0;
	do {
		status = mmc_status(ctx, pdat);
		if(status < 0)
			return 0;
	} while((status != MMC_STATUS_TRAN) && (status != MMC_STATUS_RCV));
	if(blkcnt > 1)
	{
		cmd.cmdidx = MMC_STOP_TRANSMISSION;
		cmd.cmdarg = 0;
		cmd.resptype = MMC_RSP_R1B;
		if(!fel_sdhci_xfer(ctx, pdat->swapbuf, pdat->swaplen, pdat->cmdlen, &cmd, NULL))
			return 0;
	}
	return blkcnt;
}

static uint64_t sdcard_blk_read(struct xfel_ctx_t * ctx, struct sdcard_pdata_t * pdat, uint8_t * buf, uint64_t blkno, uint64_t blkcnt)
{
	uint64_t cnt, blks = blkcnt;
	uint32_t bmax = MIN((uint32_t)127, (pdat->swaplen - 16) / pdat->info.read_bl_len);

	while(blks > 0)
	{
		cnt = (blks > bmax) ? bmax : blks;
		if(mmc_read_blocks(ctx, pdat, buf, blkno, cnt) != cnt)
			return 0;
		blks -= cnt;
		blkno += cnt;
		buf += cnt * pdat->info.read_bl_len;
	}
	return blkcnt;
}

static uint64_t sdcard_blk_write(struct xfel_ctx_t * ctx, struct sdcard_pdata_t * pdat, uint8_t * buf, uint64_t blkno, uint64_t blkcnt)
{
	uint64_t cnt, blks = blkcnt;
	uint32_t bmax = MIN((uint32_t)127, (pdat->swaplen - 16) / pdat->info.write_bl_len);

	while(blks > 0)
	{
		cnt = (blks > bmax) ? bmax : blks;
		if(mmc_write_blocks(ctx, pdat, buf, blkno, cnt) != cnt)
			return 0;
		blks -= cnt;
		blkno += cnt;
		buf += cnt * pdat->info.write_bl_len;
	}
	return blkcnt;
}

static uint64_t sdcard_helper_read(struct xfel_ctx_t * ctx, struct sdcard_pdata_t * pdat, uint64_t offset, uint8_t * buf, uint64_t count)
{
	uint64_t blksz = pdat->info.read_bl_len;
	uint64_t blkno, len, tmp;
	uint64_t ret = 0;

	blkno = offset / blksz;
	tmp = offset % blksz;
	if(tmp > 0)
	{
		len = blksz - tmp;
		if(count < len)
			len = count;
		if(sdcard_blk_read(ctx, pdat, &pdat->buf[0], blkno, 1) != 1)
			return ret;
		memcpy((void *)buf, (const void *)(&pdat->buf[tmp]), len);
		buf += len;
		count -= len;
		ret += len;
		blkno += 1;
	}
	tmp = count / blksz;
	if(tmp > 0)
	{
		len = tmp * blksz;
		if(sdcard_blk_read(ctx, pdat, buf, blkno, tmp) != tmp)
			return ret;
		buf += len;
		count -= len;
		ret += len;
		blkno += tmp;
	}
	if(count > 0)
	{
		len = count;
		if(sdcard_blk_read(ctx, pdat, &pdat->buf[0], blkno, 1) != 1)
			return ret;
		memcpy((void *)buf, (const void *)(&pdat->buf[0]), len);
		ret += len;
	}
	return ret;
}

static uint64_t sdcard_helper_write(struct xfel_ctx_t * ctx, struct sdcard_pdata_t * pdat, uint64_t offset, uint8_t * buf, uint64_t count)
{
	uint64_t blksz = pdat->info.write_bl_len;
	uint64_t blkno, len, tmp;
	uint64_t ret = 0;

	blkno = offset / blksz;
	tmp = offset % blksz;
	if(tmp > 0)
	{
		len = blksz - tmp;
		if(count < len)
			len = count;
		if(sdcard_blk_read(ctx, pdat, &pdat->buf[0], blkno, 1) != 1)
			return ret;
		memcpy((void *)(&pdat->buf[tmp]), (const void *)buf, len);
		if(sdcard_blk_write(ctx, pdat, &pdat->buf[0], blkno, 1) != 1)
			return ret;
		buf += len;
		count -= len;
		ret += len;
		blkno += 1;
	}
	tmp = count / blksz;
	if(tmp > 0)
	{
		len = tmp * blksz;
		if(sdcard_blk_write(ctx, pdat, buf, blkno, tmp) != tmp)
			return ret;
		buf += len;
		count -= len;
		ret += len;
		blkno += tmp;
	}
	if(count > 0)
	{
		len = count;
		if(sdcard_blk_read(ctx, pdat, &pdat->buf[0], blkno, 1) != 1)
			return ret;
		memcpy((void *)(&pdat->buf[0]), (const void *)buf, len);
		if(sdcard_blk_write(ctx, pdat, &pdat->buf[0], blkno, 1) != 1)
			return ret;
		ret += len;
	}
	return ret;
}

static inline int sdcard_info(struct xfel_ctx_t * ctx, struct sdcard_pdata_t * pdat)
{
	struct sdhci_cmd_t cmd = { 0 };
	struct sdhci_data_t dat = { 0 };
	uint64_t csize, cmult;
	uint32_t unit, time;
	int status;

	sdhci_reset(ctx);
	sdhci_set_clock(ctx, 400 * 1000);
	sdhci_set_width(ctx, MMC_BUS_WIDTH_1);

	if(!go_idle_state(ctx, pdat))
		return 0;

	sd_send_if_cond(ctx, pdat);
	if(!sd_send_op_cond(ctx, pdat))
	{
		if(!mmc_send_op_cond(ctx, pdat))
			return 0;
	}

	cmd.cmdidx = MMC_ALL_SEND_CID;
	cmd.cmdarg = 0;
	cmd.resptype = MMC_RSP_R2;
	if(!fel_sdhci_xfer(ctx, pdat->swapbuf, pdat->swaplen, pdat->cmdlen, &cmd, NULL))
		return 0;
	pdat->info.cid[0] = cmd.response[0];
	pdat->info.cid[1] = cmd.response[1];
	pdat->info.cid[2] = cmd.response[2];
	pdat->info.cid[3] = cmd.response[3];

	cmd.cmdidx = SD_CMD_SEND_RELATIVE_ADDR;
	cmd.cmdarg = pdat->info.rca << 16;
	cmd.resptype = MMC_RSP_R6;
	if(!fel_sdhci_xfer(ctx, pdat->swapbuf, pdat->swaplen, pdat->cmdlen, &cmd, NULL))
		return 0;
	if(pdat->info.version & SD_VERSION_SD)
		pdat->info.rca = (cmd.response[0] >> 16) & 0xffff;

	cmd.cmdidx = MMC_SEND_CSD;
	cmd.cmdarg = pdat->info.rca << 16;
	cmd.resptype = MMC_RSP_R2;
	if(!fel_sdhci_xfer(ctx, pdat->swapbuf, pdat->swaplen, pdat->cmdlen, &cmd, NULL))
		return 0;
	pdat->info.csd[0] = cmd.response[0];
	pdat->info.csd[1] = cmd.response[1];
	pdat->info.csd[2] = cmd.response[2];
	pdat->info.csd[3] = cmd.response[3];

	cmd.cmdidx = MMC_SELECT_CARD;
	cmd.cmdarg = pdat->info.rca << 16;
	cmd.resptype = MMC_RSP_R1;
	if(!fel_sdhci_xfer(ctx, pdat->swapbuf, pdat->swaplen, pdat->cmdlen, &cmd, NULL))
		return 0;
	do {
		status = mmc_status(ctx, pdat);
		if(status < 0)
			return 0;
	} while(status != MMC_STATUS_TRAN);

	if(pdat->info.version == MMC_VERSION_UNKNOWN)
	{
		switch((pdat->info.csd[0] >> 26) & 0xf)
		{
		case 0:
			pdat->info.version = MMC_VERSION_1_2;
			break;
		case 1:
			pdat->info.version = MMC_VERSION_1_4;
			break;
		case 2:
			pdat->info.version = MMC_VERSION_2_2;
			break;
		case 3:
			pdat->info.version = MMC_VERSION_3;
			break;
		case 4:
			pdat->info.version = MMC_VERSION_4;
			break;
		default:
			pdat->info.version = MMC_VERSION_1_2;
			break;
		};
	}

	unit = tran_speed_unit[(pdat->info.csd[0] & 0x7)];
	time = tran_speed_time[((pdat->info.csd[0] >> 3) & 0xf)];
	pdat->info.tran_speed = time * unit;
	pdat->info.dsr_imp = UNSTUFF_BITS(pdat->info.csd, 76, 1);

	pdat->info.read_bl_len = 1 << UNSTUFF_BITS(pdat->info.csd, 80, 4);
	if(pdat->info.version & SD_VERSION_SD)
		pdat->info.write_bl_len = pdat->info.read_bl_len;
	else
		pdat->info.write_bl_len = 1 << ((pdat->info.csd[3] >> 22) & 0xf);
	if(pdat->info.read_bl_len > 512)
		pdat->info.read_bl_len = 512;
	if(pdat->info.write_bl_len > 512)
		pdat->info.write_bl_len = 512;

	if((pdat->info.version & MMC_VERSION_MMC) && (pdat->info.version >= MMC_VERSION_4))
	{
		cmd.cmdidx = MMC_SEND_EXT_CSD;
		cmd.cmdarg = 0;
		cmd.resptype = MMC_RSP_R1;
		dat.buf = pdat->info.extcsd;
		dat.flag = MMC_DATA_READ;
		dat.blksz = 512;
		dat.blkcnt = 1;
		if(!fel_sdhci_xfer(ctx, pdat->swapbuf, pdat->swaplen, pdat->cmdlen, &cmd, &dat))
			return 0;
		do {
			status = mmc_status(ctx, pdat);
			if(status < 0)
				return 0;
		} while(status != MMC_STATUS_TRAN);
		switch(pdat->info.extcsd[192])
		{
		case 1:
			pdat->info.version = MMC_VERSION_4_1;
			break;
		case 2:
			pdat->info.version = MMC_VERSION_4_2;
			break;
		case 3:
			pdat->info.version = MMC_VERSION_4_3;
			break;
		case 5:
			pdat->info.version = MMC_VERSION_4_41;
			break;
		case 6:
			pdat->info.version = MMC_VERSION_4_5;
			break;
		case 7:
			pdat->info.version = MMC_VERSION_5_0;
			break;
		case 8:
			pdat->info.version = MMC_VERSION_5_1;
			break;
		default:
			break;
		}
	}

	if(pdat->info.high_capacity)
	{
		if(pdat->info.version & SD_VERSION_SD)
		{
			csize = UNSTUFF_BITS(pdat->info.csd, 48, 22);
			pdat->info.capacity = (1 + csize) << 10;
		}
		else
		{
			pdat->info.capacity = pdat->info.extcsd[212] << 0 | pdat->info.extcsd[212 + 1] << 8 | pdat->info.extcsd[212 + 2] << 16 | pdat->info.extcsd[212 + 3] << 24;
		}
	}
	else
	{
		cmult = UNSTUFF_BITS(pdat->info.csd, 47, 3);
		csize = UNSTUFF_BITS(pdat->info.csd, 62, 12);
		pdat->info.capacity = (csize + 1) << (cmult + 2);
	}
	pdat->info.capacity *= 1 << UNSTUFF_BITS(pdat->info.csd, 80, 4);

	if(pdat->info.version & SD_VERSION_SD)
	{
		cmd.cmdidx = MMC_APP_CMD;
		cmd.cmdarg = pdat->info.rca << 16;
		cmd.resptype = MMC_RSP_R5;
		if(!fel_sdhci_xfer(ctx, pdat->swapbuf, pdat->swaplen, pdat->cmdlen, &cmd, NULL))
			return 0;
		cmd.cmdidx = SD_CMD_SWITCH_FUNC;
		cmd.cmdarg = 2;
		cmd.resptype = MMC_RSP_R1;
		if(!fel_sdhci_xfer(ctx, pdat->swapbuf, pdat->swaplen, pdat->cmdlen, &cmd, NULL))
			return 0;
		sdhci_set_clock(ctx, MIN(pdat->info.tran_speed, (uint32_t)(25 * 1000 * 1000)));
		sdhci_set_width(ctx, MMC_BUS_WIDTH_4);
	}
	else if(pdat->info.version & MMC_VERSION_MMC)
	{
		cmd.cmdidx = MMC_APP_CMD;
		cmd.cmdarg = pdat->info.rca << 16;
		cmd.resptype = MMC_RSP_R5;
		if(!fel_sdhci_xfer(ctx, pdat->swapbuf, pdat->swaplen, pdat->cmdlen, &cmd, NULL))
			return 0;
		cmd.cmdidx = SD_CMD_SWITCH_FUNC;
		cmd.cmdarg = 2;
		cmd.resptype = MMC_RSP_R1;
		if(!fel_sdhci_xfer(ctx, pdat->swapbuf, pdat->swaplen, pdat->cmdlen, &cmd, NULL))
			return 0;
		sdhci_set_clock(ctx, MIN(pdat->info.tran_speed, (uint32_t)(50 * 1000 * 1000)));
		sdhci_set_width(ctx, MMC_BUS_WIDTH_8);
	}

	cmd.cmdidx = MMC_SET_BLOCKLEN;
	cmd.cmdarg = pdat->info.read_bl_len;
	cmd.resptype = MMC_RSP_R1;
	if(!fel_sdhci_xfer(ctx, pdat->swapbuf, pdat->swaplen, pdat->cmdlen, &cmd, NULL))
		return 0;
	return 1;
}

static int sdcard_helper_init(struct xfel_ctx_t * ctx, struct sdcard_pdata_t * pdat)
{
	if(fel_sdhci_init(ctx, &pdat->swapbuf, &pdat->swaplen, &pdat->cmdlen) && sdcard_info(ctx, pdat))
		return 1;
	return 0;
}

int sdcard_detect(struct xfel_ctx_t * ctx, char * name, uint64_t * capacity)
{
	struct sdcard_pdata_t pdat;

	if(sdcard_helper_init(ctx, &pdat))
	{
		if(name)
			sprintf(name, "%c%c%c%c%c", (pdat.info.cid[0] >> 0) & 0xff, (pdat.info.cid[1] >> 24) & 0xff, (pdat.info.cid[1] >> 16) & 0xff, (pdat.info.cid[1] >> 8) & 0xff, (pdat.info.cid[1] >> 0) & 0xff);
		if(capacity)
			*capacity = pdat.info.capacity;
		return 1;
	}
	return 0;
}

int sdcard_read(struct xfel_ctx_t * ctx, uint64_t addr, void * buf, uint64_t len)
{
	struct sdcard_pdata_t pdat;
	struct progress_t p;
	uint64_t n;

	if(sdcard_helper_init(ctx, &pdat))
	{
		progress_start(&p, len);
		while(len > 0)
		{
			n = len > 65536 ? 65536 : len;
			sdcard_helper_read(ctx, &pdat, addr, buf, n);
			addr += n;
			len -= n;
			buf += n;
			progress_update(&p, n);
		}
		progress_stop(&p);
		return 1;
	}
	return 0;
}

int sdcard_write(struct xfel_ctx_t * ctx, uint64_t addr, void * buf, uint64_t len)
{
	struct sdcard_pdata_t pdat;
	struct progress_t p;
	uint64_t n;

	if(sdcard_helper_init(ctx, &pdat))
	{
		progress_start(&p, len);
		while(len > 0)
		{
			n = len > 65536 ? 65536 : len;
			sdcard_helper_write(ctx, &pdat, addr, buf, n);
			addr += n;
			len -= n;
			buf += n;
			progress_update(&p, n);
		}
		progress_stop(&p);
		return 1;
	}
	return 0;
}
