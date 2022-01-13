/*
 * sys-spi.c
 *
 * Copyright(c) 2007-2021 Jianjun Jiang <8192542@qq.com>
 * Official site: http://xboot.org
 * Mobile phone: +86-18665388956
 * QQ: 8192542
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 *
 */

#include <xboot.h>

enum {
	SD_CMD_END				= 0x00, // none
	SD_CMD_INIT				= 0x01, // none
	SD_CMD_INFO				= 0x02, // u32:buf. ret_buf=u64:cap
	SD_CMD_READ				= 0x03, // u64:sd-addr, u32:buf, u32:len. ret_buf=512
	SD_CMD_WRITE			= 0x04, // u64:sd-addr, u32:buf, u32:len. dat_buf=512
};

extern void sys_uart_done(void);
extern int sys_uart_printf(const char * fmt, ...);

extern uint64_t sys_sd_init(void);
extern void sdio_read_sector(uint32_t addr_byte, uint32_t len, uint8_t *buf);
extern uint32_t sdio_write_sector(uint32_t addr_byte, uint32_t len, uint8_t *buf);

void sys_spi_run(void * cbuf)
{
	u8_t c, *p = cbuf;
	u32_t addr, len; // swap buf len.
	u64_t addr64;

	// sys_uart_printf("\r\nsys sd run: 0x%x\r\n", cbuf);

	while(1)
	{
		c = *p++;
		// sys_uart_printf("\r\nCMD: %x ", c);

		if(c == SD_CMD_END)
		{
			// sys_uart_printf("SD_CMD_END\r\n");
			break;
		}
		else if(c == SD_CMD_INIT)
		{
			sys_uart_printf("SD_CMD_INIT\r\n");
		}
		else if(c == SD_CMD_INFO)
		{
			sys_uart_printf("SD_CMD_INFO\r\n");

			memcpy(&addr, p, 4);
			p += 4;

			sys_uart_printf("resp addr=%d\r\n", addr);

			uint64_t c_size = sys_sd_init();
			*(uint64_t *)((uint64_t)addr) = c_size;
		}
		else if(c == SD_CMD_READ)
		{
			// sys_uart_printf("SD_CMD_READ\r\n");

			memcpy(&addr64, p, 8);
			p += 8;
			memcpy(&addr, p, 4);
			p += 4;
			memcpy(&len, p, 4);
			p += 4;

			// sys_uart_printf("sd addr=0x%x, len=%d\r\n", (void *)addr64, len);
			// sys_uart_printf("resp addr=%d\r\n", addr);

			sdio_read_sector(addr64, len, (uint8_t *)((uint64_t)addr));
		}
		else if(c == SD_CMD_WRITE)
		{
			// sys_uart_printf("SD_CMD_WRITE\r\n");

			memcpy(&addr64, p, 8);
			p += 8;
			memcpy(&addr, p, 4);
			p += 4;
			memcpy(&len, p, 4);
			p += 4;

			// sys_uart_printf("sd addr=0x%x, len=%d\r\n", (void *)addr64, len);
			// sys_uart_printf("resp addr=%d\r\n", addr);

			sdio_write_sector(addr64, len, (uint8_t *)((uint64_t)addr));
		}
		else
		{
			sys_uart_printf("exit\r\n");
			break;
		}
	}

	sys_uart_done();
}
