#ifndef __FILE_H__
#define __FILE_H__

#ifdef __cplusplus
extern "C" {
#endif

#include <x.h>

uint64_t file_save(const char * filename, void * buf, uint64_t len);
void * file_load(const char * filename, uint64_t * len);

#ifdef __cplusplus
}
#endif

#endif /* __FILE_H__ */
