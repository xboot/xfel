#include <fel.h>

static int chip_detect(struct xfel_ctx_t * ctx, uint32_t id)
{
	if(id == 0x00190300)
		return 1;
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
	uint32_t id[4];

	id[0] = payload_read32(ctx, 0x03006200 + 0x0);
	id[1] = payload_read32(ctx, 0x03006200 + 0x4);
	id[2] = payload_read32(ctx, 0x03006200 + 0x8);
	id[3] = payload_read32(ctx, 0x03006200 + 0xc);
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

enum {
	SID_PRCTL		= 0x03006000 + 0x040,
	SID_PRKEY		= 0x03006000 + 0x050,
	SID_RDKEY 		= 0x03006000 + 0x060,
	EFUSE_HV_SWITCH	= 0x07090000 + 0x204,
};

static uint32_t efuse_read(struct xfel_ctx_t * ctx, uint32_t offset)
{
	uint32_t val;

	val = payload_read32(ctx, SID_PRCTL);
	val &= ~((0x1ff << 16) | 0x3);
	val |= offset << 16;
	payload_write32(ctx, SID_PRCTL, val);
	val &= ~((0xff << 8) | 0x3);
	val |= (0xac << 8) | 0x2;
	payload_write32(ctx, SID_PRCTL, val);
	while(payload_read32(ctx, SID_PRCTL) & 0x2);
	val &= ~((0x1ff << 16) | (0xff << 8) | 0x3);
	payload_write32(ctx, SID_PRCTL, val);
	val = payload_read32(ctx, SID_RDKEY);

	return val;
}

static void efuse_write(struct xfel_ctx_t * ctx, uint32_t offset, uint32_t value)
{
	uint32_t val;

	payload_write32(ctx, EFUSE_HV_SWITCH, 0x1);
	payload_write32(ctx, SID_PRKEY, value);
	val = payload_read32(ctx, SID_PRCTL);
	val &= ~((0x1ff << 16) | 0x3);
	val |= offset << 16;
	payload_write32(ctx, SID_PRCTL, val);
	val &= ~((0xff << 8) | 0x3);
	val |= (0xac << 8) | 0x1;
	payload_write32(ctx, SID_PRCTL, val);
	while(payload_read32(ctx, SID_PRCTL) & 0x1);
	val &= ~((0x1ff << 16) | (0xff << 8) | 0x3);
	payload_write32(ctx, SID_PRCTL, val);
	payload_write32(ctx, EFUSE_HV_SWITCH, 0x0);
}

static const struct sid_section_t {
	char * name;
	uint32_t offset;
	uint32_t size_bits;
} sids[] = {
	{ "chipid",         0x00,  128 },
	{ "brom-config",    0x10,   32 },
	{ "tf-zone",        0x28,  128 },
	{ "thermal-sensor", 0x30,   64 },
	{ "oem-program",    0x3c,  160 },
	{ "write-protect",  0x80,   32 },
	{ "read-protect",   0x84,   32 },
	{ "lcjs",           0x88,   32 },
	{ "attr",           0x90,   32 },
	{ "huk",            0x94,  192 },
	{ "reserved1",      0xac,   64 },
	{ "rotpk",          0xb4,  256 },
	{ "ssk",            0xd4,  128 },
	{ "rssk",           0xf4,  256 },
	{ "sn",             0xb0,  192 },
	{ "nv1",            0x124,  32 },
	{ "nv2",            0x128, 224 },
	{ "hdcp-hash",      0x114, 128 },
	{ "backup-key",     0x164, 192 },
	{ "backup-key2",    0x1a4,  72 }
};

static int chip_extra(struct xfel_ctx_t * ctx, int argc, char * argv[])
{
	if(argc > 0)
	{
		if(!strcmp(argv[0], "efuse"))
		{
			argc -= 1;
			argv += 1;
			if(argc > 0)
			{
				if(!strcmp(argv[0], "dump") && (argc == 1))
				{
					uint32_t buffer[2048 / 4];
					for(int n = 0; n < ARRAY_SIZE(sids); n++)
					{
						uint32_t count = sids[n].size_bits / 32;
						for(int i = 0; i < count; i++)
							buffer[i] = efuse_read(ctx, sids[n].offset + i * 4);
						printf("%s:(0x%04x %d-bits)", sids[n].name, sids[n].offset, sids[n].size_bits);
						for(int i = 0; i < count; i++)
						{
							if(i >= 0 && ((i % 8) == 0))
								printf("\r\n%-4s", "");
							printf("%08x ", buffer[i]);
						}
						printf("\r\n");
					}
					return 1;
				}
				else if(!strcmp(argv[0], "read32") && (argc == 2))
				{
					uint32_t offset = strtoul(argv[1], NULL, 0);
					printf("0x%08x\r\n", efuse_read(ctx, offset));
					return 1;
				}
				else if(!strcmp(argv[0], "write32") && (argc == 3))
				{
					uint32_t offset = strtoul(argv[1], NULL, 0);
					size_t value = strtoul(argv[2], NULL, 0);
					efuse_write(ctx, offset, value);
					return 1;
				}
				else if(!strcmp(argv[0], "write") && (argc == 3))
				{
					uint32_t offset = strtoul(argv[1], NULL, 0);
					uint64_t len;
					void * buf = file_load(argv[2], &len);
					if(buf && (len > 0))
					{
						uint8_t * p = buf;
						uint32_t l = len;
						uint32_t o = 0;
						uint32_t v;
						while(l >= 4)
						{
							v = *((uint32_t *)p);
							efuse_write(ctx, offset + o, v);
							l -= 4;
							o += 4;
							p += 4;
						}
						if(l > 0)
						{
							uint32_t v = 0;
							for(int i = 0; i < l; i++)
								v = ((v << 8) & 0xffffff00) | p[i];
							efuse_write(ctx, offset + o, v);
						}
						free(buf);
						return 1;
					}
				}
			}
		}
		else if(!strcmp(argv[0], "checksboot"))
		{
			uint32_t secure_status = payload_read32(ctx, 0x03006000 + 0xA0);
			if(secure_status)
				printf("The device enabled the security boot.\r\n");
			else
				printf("The device does not enabled the security boot.\r\n");
			return 1;
		}
	}
	printf("usage:\r\n");
	printf("    xfel extra efuse dump                     - Dump all of the efuse information\r\n");
	printf("    xfel extra efuse read32 <offset>          - Read 32-bits value from efuse\r\n");
	printf("    xfel extra efuse write32 <offset> <value> - Write 32-bits value to efuse\r\n");
	printf("    xfel extra efuse write <offset> <file>    - Write file to efuse\r\n");
	printf("    xfel extra checksboot                     - Check whether security boot is enabled on the device\r\n");
	return 0;
}

struct chip_t a733 = {
	.name = "A733",
	.detect = chip_detect,
	.reset = chip_reset,
	.sid = chip_sid,
	.jtag = chip_jtag,
	.ddr = chip_ddr,
	.spi_init = chip_spi_init,
	.spi_run = chip_spi_run,
	.extra = chip_extra,
};
