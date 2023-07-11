#ifndef __MISC_H__
#define __MISC_H__

#ifdef __cplusplus
extern "C" {
#endif

#include <x.h>

uint64_t file_save(const char * filename, void * buf, uint64_t len);
void * file_load(const char * filename, uint64_t * len);
unsigned char hex_string(const char * s, int o);
void hexdump(uint32_t addr, void * buf, size_t len);

#ifdef __cplusplus
}
#endif

#endif /* __MISC_H__ */
