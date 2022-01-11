#include <xboot.h>

extern int sys_uart_printf(const char * fmt, ...);

// #define logout(...)
#define logout  sys_uart_printf("[sdio] ");sys_uart_printf

#define SUNXI_SMHC0_BASE				(0x04020000U)
#define SUNXI_MMC0_BASE				    (SUNXI_SMHC0_BASE)
#define MMC_REG_BASE                    SUNXI_MMC0_BASE

struct tina_mmc
{
    volatile uint32_t gctl_reg;            /* (0x000) */
    volatile uint32_t ckcr_reg;            /* (0x004) */
    volatile uint32_t tmor_reg;            /* (0x008) */
    volatile uint32_t bwdr_reg;            /* (0x00C) */
    volatile uint32_t bksr_reg;            /* (0x010) */
    volatile uint32_t bycr_reg;            /* (0x014) */
    volatile uint32_t cmdr_reg;            /* (0x018) */
    volatile uint32_t cagr_reg;            /* (0x01C) */
    volatile uint32_t resp0_reg;           /* (0x020) */
    volatile uint32_t resp1_reg;           /* (0x024) */
    volatile uint32_t resp2_reg;           /* (0x028) */
    volatile uint32_t resp3_reg;           /* (0x02C) */
    volatile uint32_t imkr_reg;            /* (0x030) */
    volatile uint32_t misr_reg;            /* (0x034) */
    volatile uint32_t risr_reg;            /* (0x038) */
    volatile uint32_t star_reg;            /* (0x03C) */
    volatile uint32_t fwlr_reg;            /* (0x040) */
    volatile uint32_t funs_reg;            /* (0x044) */
    volatile uint32_t cbcr_reg;            /* (0x048) */
    volatile uint32_t bbcr_reg;            /* (0x04C) */
    volatile uint32_t dbgc_reg;            /* (0x050) */
    volatile uint32_t reserved0;
    volatile uint32_t a12a_reg;            /* (0x058) */
    volatile uint32_t reserved1[7];
    volatile uint32_t hwrst_reg;           /* (0x078) */
    volatile uint32_t reserved2;
    volatile uint32_t dmac_reg;            /* (0x080) */
    volatile uint32_t dlba_reg;            /* (0x084) */
    volatile uint32_t idst_reg;            /* (0x088) */
    volatile uint32_t idie_reg;            /* (0x08C) */
    volatile uint32_t chda_reg;            /* (0x090) */
    volatile uint32_t cbda_reg;            /* (0x094) */
    volatile uint32_t reserved3[26];
    volatile uint32_t card_thldc_reg;      /* (0x100) */
    volatile uint32_t reserved4[2];
    volatile uint32_t emmc_dsbd_reg;       /* (0x10c) */
    volatile uint32_t reserved5[12];
    volatile uint32_t reserved6[48];
    volatile uint32_t fifo_reg;            /* (0x200) */
};

typedef struct tina_mmc *tina_mmc_t;
volatile tina_mmc_t mmc=(tina_mmc_t)SUNXI_SMHC0_BASE;
static uint32_t csd_structure;
static uint32_t sd_rca;

#define GO_IDLE_STATE         0   /* bc                          */
#define SEND_OP_COND          1   /* bcr  [31:0] OCR         R3  */
#define ALL_SEND_CID          2   /* bcr                     R2  */
#define SET_RELATIVE_ADDR     3   /* ac   [31:16] RCA        R1  */
#define SET_DSR               4   /* bc   [31:16] RCA            */
#define SWITCH                6   /* ac   [31:0] See below   R1b */
#define SELECT_CARD           7   /* ac   [31:16] RCA        R1  */
#define SEND_EXT_CSD          8   /* adtc                    R1  */
#define SEND_CSD              9   /* ac   [31:16] RCA        R2  */
#define SEND_CID             10   /* ac   [31:16] RCA        R2  */
#define READ_DAT_UNTIL_STOP  11   /* adtc [31:0] dadr        R1  */
#define STOP_TRANSMISSION    12   /* ac                      R1b */
#define SEND_STATUS          13   /* ac   [31:16] RCA        R1  */
#define GO_INACTIVE_STATE    15   /* ac   [31:16] RCA            */
#define SPI_READ_OCR         58   /* spi                  spi_R3 */
#define SPI_CRC_ON_OFF       59   /* spi  [0:0] flag      spi_R1 */

  /* class 2 */
#define SET_BLOCKLEN         16   /* ac   [31:0] block len   R1  */
#define READ_SINGLE_BLOCK    17   /* adtc [31:0] data addr   R1  */
#define READ_MULTIPLE_BLOCK  18   /* adtc [31:0] data addr   R1  */

  /* class 3 */
#define WRITE_DAT_UNTIL_STOP 20   /* adtc [31:0] data addr   R1  */

  /* class 4 */
#define SET_BLOCK_COUNT      23   /* adtc [31:0] data addr   R1  */
#define WRITE_BLOCK          24   /* adtc [31:0] data addr   R1  */
#define WRITE_MULTIPLE_BLOCK 25   /* adtc                    R1  */
#define PROGRAM_CID          26   /* adtc                    R1  */
#define PROGRAM_CSD          27   /* adtc                    R1  */

  /* class 6 */
#define SET_WRITE_PROT       28   /* ac   [31:0] data addr   R1b */
#define CLR_WRITE_PROT       29   /* ac   [31:0] data addr   R1b */
#define SEND_WRITE_PROT      30   /* adtc [31:0] wpdata addr R1  */

  /* class 5 */
#define ERASE_GROUP_START    35   /* ac   [31:0] data addr   R1  */
#define ERASE_GROUP_END      36   /* ac   [31:0] data addr   R1  */
#define ERASE                38   /* ac                      R1b */

  /* class 9 */
#define FAST_IO              39   /* ac   <Complex>          R4  */
#define GO_IRQ_STATE         40   /* bcr                     R5  */

  /* class 7 */
#define LOCK_UNLOCK          42   /* adtc                    R1b */

  /* class 8 */
#define APP_CMD              55   /* ac   [31:16] RCA        R1  */
#define GEN_CMD              56   /* adtc [0] RD/WR          R1  */


/* SD commands                           type  argument     response */
  /* class 0 */
/* This is basically the same command as for MMC with some quirks. */
#define SD_SEND_RELATIVE_ADDR     3   /* bcr                     R6  */
#define SD_SEND_IF_COND           8   /* bcr  [11:0] See below   R7  */

  /* class 10 */
#define SD_SWITCH                 6   /* adtc [31:0] See below   R1  */

  /* Application commands */
#define SD_APP_SET_BUS_WIDTH      6   /* ac   [1:0] bus width    R1  */
#define SD_APP_SEND_NUM_WR_BLKS  22   /* adtc                    R1  */
#define SD_APP_OP_COND           41   /* bcr  [31:0] OCR         R3  */
#define SD_APP_SEND_SCR          51   /* adtc                    R1  */

#define SCR_SPEC_VER_0		0	/* Implements system specification 1.0 - 1.01 */
#define SCR_SPEC_VER_1		1	/* Implements system specification 1.10 */
#define SCR_SPEC_VER_2		2	/* Implements system specification 2.00 */


/* SDIO commands                                          type  argument     response */
#define SD_IO_SEND_OP_COND          5 /* bcr  [23:0] OCR         R4  */
#define SD_IO_RW_DIRECT            52 /* ac   [31:0] See below   R5  */
#define SD_IO_RW_EXTENDED          53 /* adtc [31:0] See below   R5  */


/* CMD52 arguments */
#define SDIO_ARG_CMD52_READ             (0<<31)
#define SDIO_ARG_CMD52_WRITE            (1u<<31)
#define SDIO_ARG_CMD52_FUNC_SHIFT       28
#define SDIO_ARG_CMD52_FUNC_MASK        0x7
#define SDIO_ARG_CMD52_RAW_FLAG         (1u<<27)
#define SDIO_ARG_CMD52_REG_SHIFT        9
#define SDIO_ARG_CMD52_REG_MASK         0x1ffff
#define SDIO_ARG_CMD52_DATA_SHIFT       0
#define SDIO_ARG_CMD52_DATA_MASK        0xff
#define SDIO_R5_DATA(resp)            ((resp)[0] & 0xff)

/* CMD53 arguments */
#define SDIO_ARG_CMD53_READ             (0<<31)
#define SDIO_ARG_CMD53_WRITE            (1u<<31)
#define SDIO_ARG_CMD53_FUNC_SHIFT       28
#define SDIO_ARG_CMD53_FUNC_MASK        0x7
#define SDIO_ARG_CMD53_BLOCK_MODE       (1u<<27)
#define SDIO_ARG_CMD53_INCREMENT        (1u<<26)
#define SDIO_ARG_CMD53_REG_SHIFT        9
#define SDIO_ARG_CMD53_REG_MASK         0x1ffff
#define SDIO_ARG_CMD53_LENGTH_SHIFT     0
#define SDIO_ARG_CMD53_LENGTH_MASK      0x1ff
#define SDIO_ARG_CMD53_LENGTH_MAX       511

/*the following is response bit*/
#define R1_CURRENT_STATE(x)	((x & 0x00001E00) >> 9)	/* sx, b (4 bits) */
#define R1_READY_FOR_DATA	(1 << 8)	/* sx, a */

/*cmd reg*/

#define VAL_CMD_LOAD             (1u<<31)
#define VAL_USE_HOLDREG         (1u<<29)
#define VAL_VOL_SW_CMD11        (1u<<28)
#define VAL_BOOT_ABT            (1u<<27)
#define VAL_EXP_BOOT_ACK        (1u<<26)
#define VAL_BOOT_MOD(mode)      ((mode & 0x03)<<24)
#define VAL_PRG_CLK             (1u<<21)
#define VAL_SEND_INIT_SEQ        (1u<<15)
#define VAL_STOP_ABT_CMD        (1u<<14)

#define VAL_WAIT_PRE_OVER       (1u<<13)
#define VAL_STOP_CMD_FLAG       (1u<<12)
#define VAL_TRANS_MODE_STREAM   (1u<<11)
#define VAL_TRANS_DIR_WRITE     (1u<<10)
#define VAL_WITH_DATA_TRANS     (1u<<9)

#define VAL_CHK_RESP_CRC        (1u<<8)
#define VAL_LONG_RESP           (1u<<7)
#define VAL_CMD_WITH_RESP       (1u<<6)

#define BIT(x)              (1<<(x))
#define SDXC_RESPONSE_CMD        BIT(6)
#define SDXC_LONG_RESPONSE_CMD   BIT(7)
#define SDXC_CHECK_CRC_CMD       BIT(8)
#define SDXC_HAS_DATA_CMD        BIT(9)
#define SDXC_WRITE_CMD           BIT(10)
#define SDXC_STEAM_CMD           BIT(11)
#define SDXC_AUTO_STOP_CMD       BIT(12)
#define SDXC_WAIT_OVER_CMD       BIT(13)
#define SDXC_ABOUT_CMD           BIT(14)
#define SDXC_SEND_INIT_CMD       BIT(15)
#define SDXC_UPDATE_CLOCK_CMD    BIT(21)
#define SDXC_LOAD_CMD            BIT(31)

static inline void sdelay(int loops)
{
	  volatile uint32_t i = loops*10;

    while (i--);
}

static inline uint32_t GET_BITS(uint32_t *resp,
                                uint32_t start,
                                uint32_t size)
{
    const int32_t __size = size;
    const uint32_t __mask = (__size < 32 ? 1 << __size : 0) - 1;
    const int32_t __off = 3 - ((start) / 32);
    const int32_t __shft = (start)&31;
    uint32_t __res;

    __res = resp[__off] >> __shft;
    if (__size + __shft > 32)
        __res |= resp[__off - 1] << ((32 - __shft) % 32);

    return __res & __mask;
}


static void sdio_gpio_clk_init(void)
{
	  virtual_addr_t addr;
	  u32_t val;

    /* Config GPIOF1-D0, GPIOF2-CLK, GPIOF3-CMD to sdio0 */
    addr = 0x020000F0; // PF_CFG0
    val = read32(addr);
    val &= ~(0xf << ((1 & 0x7) << 2));
    val |= ((0x2 & 0xf) << ((1 & 0x7) << 2));
    write32(addr, val);

    val = read32(addr);
    val &= ~(0xf << ((2 & 0x7) << 2));
    val |= ((0x2 & 0xf) << ((2 & 0x7) << 2));
    write32(addr, val);

    val = read32(addr);
    val &= ~(0xf << ((3 & 0x7) << 2));
    val |= ((0x2 & 0xf) << ((3 & 0x7) << 2));
    write32(addr, val);

    // pull-up
    addr = 0x02000114; // PF_PULL0
    val = read32(addr);
    val &= ~((3 << 6) | (3 << 4) | (3 << 2));
    val |=  ((1 << 6) | (1 << 4) | (1 << 2));
    write32(addr, val);

    // set_drive_level
    addr = 0x02000104; // PF_PULL0
    val = read32(addr);
    val &= ~((3 << 12) | (3 << 8) | (3 << 4));
    val |=  ((2 << 12) | (2 << 8) | (2 << 4));
    write32(addr, val);

    /* Open the clock gate for sdio0 */
    addr = 0x0200184C; // SMHC_BGR_REG
    val = read32(addr);
    val |= (1UL << 0); // gating clock for SMHC0
    write32(addr, val);
    val |= (1UL << 16); // reset
    write32(addr, val);

    addr = 0x02001830;
    val = read32(addr);
    val &= ~((0x07 << 24) | (0x03 << 8) | (0x0F << 0)); // [26:24]: 000=HOSC, [9:8]:00=N=1, [3:0]:0000=M=1
    val |= (1UL << 31);                                 // enable SMHC0
    write32(addr, val);
}

static void mmc_update_clk(void)
{
    uint32_t cmd;
    uint32_t timeout = 100;

    /* cmd load */
    cmd = SDXC_LOAD_CMD | SDXC_UPDATE_CLOCK_CMD | SDXC_WAIT_OVER_CMD;
    mmc->cmdr_reg = cmd;
    /* while load success */
    while ((mmc->cmdr_reg & SDXC_LOAD_CMD) && (--timeout))
    {
      sdelay(100000);
    }
    if (!timeout)
    {
      logout("mmc update clk failed\r\n");
    }
    /* clean interrupt */
    mmc->risr_reg = mmc->risr_reg;
}

static void sdio_config_clk_width(uint32_t clk, uint32_t width)
{
    uint32_t rval = 0;

    /* disable card clock */
    // logout("dis card clock\r\n");
    rval = mmc->ckcr_reg;
    rval &= ~(1 << 16);
    mmc->ckcr_reg = rval;
    mmc_update_clk();
    sdelay(1000);
    logout("en card clock\r\n");
    // mmc_set_clk(SDMMC0, clk);

    /* Re-enable card clock */
    rval = mmc->ckcr_reg;
    rval |= (0x1 << 16); //(3 << 16);
    mmc->ckcr_reg = rval;
    mmc_update_clk();

    mmc->bwdr_reg = width / 4;

    sdelay(1000);
}

void read_resp(uint32_t *resp)
{
    resp[3] = mmc->resp0_reg;
    resp[2] = mmc->resp1_reg;
    resp[1] = mmc->resp2_reg;
    resp[0] = mmc->resp3_reg;
}

void sendcmd(int32_t cmd, int32_t arg, int32_t cmdval)
{
    uint32_t timeout = 100;
    uint32_t val;

    mmc->cagr_reg = arg;
    val = cmdval | cmd | 0x80000000;
    // logout("send %x,%x\r\n",0x7fffffff & val, 0x7fffffff & arg);

    mmc->cmdr_reg = val;

    while ((mmc->cmdr_reg & SDXC_LOAD_CMD) && (--timeout))
    {
      sdelay(100);
    }

    if (!timeout)
    {
      // logout("Run cmd failed\r\n");
    }

    /* clean interrupt */
    mmc->risr_reg = mmc->risr_reg;
}

void sdio_read_sector(uint32_t addr_sec, uint8_t *buf)
{
    uint32_t timeout;
    uint32_t *pdat=(uint32_t *)buf;
    mmc->bksr_reg = 512;
    mmc->bycr_reg = 512;
    mmc->gctl_reg = mmc->gctl_reg | 0x80000000;
    if (csd_structure == 0)
    {
      mmc->cagr_reg = addr_sec * 512;
    }
    else
    {
      mmc->cagr_reg = addr_sec;
    }

    mmc->cmdr_reg = 0x80000000|READ_SINGLE_BLOCK|VAL_CMD_WITH_RESP|VAL_CHK_RESP_CRC|VAL_WITH_DATA_TRANS|VAL_WAIT_PRE_OVER;

    timeout = 200000;
    for (int i = 0; i < 128; i++)
    {
      while ((mmc->star_reg & (1 << 2)) && --timeout);

      if (timeout <= 0)
      {
        logout("read data timeout!\r\n");
        break;
      }

      pdat[i] = mmc->fifo_reg;
      timeout = 200000;
    }
}

void sdio_write_sector(uint32_t addr_sec, uint8_t *buf)
{
    uint32_t timeout;
    uint32_t *pdat=(uint32_t *)buf;
    uint32_t resp[4];
    uint32_t status, count;

    mmc->bksr_reg = 512;
    mmc->bycr_reg = 512;
    mmc->gctl_reg = mmc->gctl_reg | 0x80000000;
    if (csd_structure == 0)
    {
      mmc->cagr_reg = addr_sec * 512;
    }
    else
    {
      mmc->cagr_reg = addr_sec;
    }

    mmc->risr_reg = (1<<2);//[2] CC
    mmc->cmdr_reg = 0x80000000 | WRITE_BLOCK | VAL_CMD_WITH_RESP | VAL_CHK_RESP_CRC | VAL_WITH_DATA_TRANS | VAL_WAIT_PRE_OVER | VAL_TRANS_DIR_WRITE;

    timeout = 200000;
    for (int i = 0; i < 128; i++)
    {
      while ((mmc->star_reg & (1 << 3)) && --timeout);

      if (timeout <= 0)
      {
        logout("write data timeout!\r\n");
        break;
      }

      mmc->fifo_reg = pdat[i];
      timeout = 200000;
    }
    //logout("WRITE_BLOCK, star_reg=%x, risr_reg=%x\r\n", count, status);

    count = 0;
    do
    {
        count++;
        status =  mmc->risr_reg;

        if(count > 8000000)
        {
            logout("wait CMD done timeout!\r\n");
            // ret = 1;
            break;
        }
    } while (!(status & (1<<2)));
    // logout("CMD done, risr=%x, count=%d\r\n", status, count);

    count = 0;
    do
    {
        count++;
        status =  mmc->risr_reg;

        if(count > 8000000)
        {
            logout("wait DATA done timeout!\r\n");
            // ret = 1;
            break;
        }
    } while (!(status & (1 << 3)));
    // logout("DATA done, risr=%x, count=%d\r\n", status, count);

    count = 0;
    do
    {
        sendcmd(SEND_STATUS, sd_rca, VAL_CMD_WITH_RESP | VAL_CHK_RESP_CRC); // select card into trans state
        // sdelay(10000);

        read_resp(resp);
        count++;

        // logout("SEND_STATUS resp=%x\r\n", resp[3]);
    } while (!(resp[3] & R1_READY_FOR_DATA) ||
             (R1_CURRENT_STATE(resp[3]) == 7));
    // logout("SEND_STATUS done, resp=%x, count=%d\r\n", resp[3], count);
}

uint64_t sys_sd_init(void)
{
    uint32_t c_size = 0; // KB
    uint32_t resp[4];

    // logout("func=sys_sd_init L=%d\r\n", __LINE__);
    sdio_gpio_clk_init();
    // logout("func=sys_sd_init L=%d\r\n", __LINE__);
    sdio_config_clk_width(400000, 1);
    
    mmc->tmor_reg = 0xFFFFFFFF;
    mmc->ckcr_reg = ((mmc->ckcr_reg) & 0xFFFFFF00) | (60 << 0);

    // logout("func=sys_sd_init L=%d\r\n", __LINE__);
    sendcmd(GO_IDLE_STATE, 0, VAL_SEND_INIT_SEQ | VAL_CHK_RESP_CRC); // IDLE+INITSEQ
    sdelay(10000);
    read_resp(resp);

    // logout("func=sys_sd_init L=%d\r\n", __LINE__);
    sendcmd(SD_SEND_IF_COND, 0x1aa, VAL_CHK_RESP_CRC | VAL_CMD_WITH_RESP); // RESP R7
    sdelay(10000);
    read_resp(resp);

    // wait OCR bit 31=1,try 10 times
    int i;
    for (i = 0; i < 10; i++)
    {
        sendcmd(APP_CMD, 0, VAL_CHK_RESP_CRC | VAL_CMD_WITH_RESP); // next CMD is ACMD
        sdelay(10000);
        read_resp(resp);
        // logout("cmd %d,resp0:%x\r\n", APP_CMD, resp[3]);

        sendcmd(SD_APP_OP_COND, 0x40018000, VAL_CMD_WITH_RESP); // get OCR
        sdelay(10000);
        read_resp(resp);
        // logout("cmd %d,resp0:%x\r\n",SD_APP_OP_COND,resp[3]);

        if (resp[3] & 0x80000000)
        {
            logout("wait OCR[31]=1 done.\r\n");
            break;
        }
    }
    if(i>=10)
    {
        logout("Not support capacity < 2G!\r\n");
    }

    sendcmd(ALL_SEND_CID, 0, VAL_CMD_WITH_RESP | VAL_LONG_RESP | VAL_CHK_RESP_CRC); // get CID
    sdelay(10000);
    read_resp(resp);
    // logout("cmd %d,resp3:%x\r\n", ALL_SEND_CID, resp[0]);
    // logout("cmd %d,resp2:%x\r\n", ALL_SEND_CID, resp[1]);
    // logout("cmd %d,resp1:%x\r\n", ALL_SEND_CID, resp[2]);
    // logout("cmd %d,resp0:%x\r\n", ALL_SEND_CID, resp[3]);

    sendcmd(SET_RELATIVE_ADDR, 0, VAL_CMD_WITH_RESP | VAL_CHK_RESP_CRC); // get RCA[31:16]
    sdelay(10000);
    read_resp(resp);
    sd_rca = resp[3] & 0xffff0000; // 这里获取RCA应该是没错的
    logout("cmd %d,sd_rca:%x\r\n", SET_RELATIVE_ADDR, sd_rca);

    sendcmd(SEND_CSD, sd_rca, VAL_CMD_WITH_RESP | VAL_LONG_RESP | VAL_CHK_RESP_CRC); // get CSD
    sdelay(10000);
    read_resp(resp);
    // logout("cmd %d,resp3:%x\r\n", SEND_CSD, resp[0]);
    // logout("cmd %d,resp2:%x\r\n", SEND_CSD, resp[1]);
    // logout("cmd %d,resp1:%x\r\n", SEND_CSD, resp[2]);
    // logout("cmd %d,resp0:%x\r\n", SEND_CSD, resp[3]);
    csd_structure = GET_BITS(resp, 126, 2);
    switch (csd_structure)
    {
    case 0: // SDSC
    {
      uint32_t rd_blk_len = GET_BITS(resp, 80, 4);
      uint32_t c_size_mult = GET_BITS(resp, 47, 3);
      c_size = GET_BITS(resp, 62, 12);
      logout("SDSC rd_blk_len %d, c_size_mult %d, c_size %d.\r\n", rd_blk_len, c_size_mult, c_size);

      rd_blk_len = 1 << rd_blk_len;
      c_size = (c_size + 1) << (c_size_mult + 2);
      c_size *= rd_blk_len; // byte
      c_size = c_size/1024; // KB
      logout("SDSC block size %d, capacity %d KB\r\n", rd_blk_len, c_size);
    }
    break;
    case 1: // SDHC
      c_size = GET_BITS(resp, 48, 22);
      c_size = (c_size + 1) * 512; // KB
      logout("SDHC capacity %d KB\r\n", c_size);
      break;
    default:
      logout("Not found SD card!\r\n");
      break;
    }

    sendcmd(SELECT_CARD, sd_rca, VAL_CMD_WITH_RESP | VAL_CHK_RESP_CRC); // select card into trans state
    sdelay(10000);

    if (csd_structure == 0)
    {
      logout("update SDSC blksize\r\n");
      sendcmd(SET_BLOCKLEN, 512, VAL_CMD_WITH_RESP | VAL_CHK_RESP_CRC); // select card into trans state
      sdelay(10000);
    }

    // sdio_config_clk_width(24 * 1000 * 1000, 1);
    mmc->ckcr_reg = ((mmc->ckcr_reg) & 0xFFFFFF00) | (0 << 0);

    return (uint64_t)c_size * 1024;
}
