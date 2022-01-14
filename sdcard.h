#ifndef __SDCARD_H__
#define __SDCARD_H__

#ifdef __cplusplus
extern "C" {
#endif

#include <fel.h>

int sdcard_detect(struct xfel_ctx_t * ctx, char * name, uint64_t * capacity);
int sdcard_read(struct xfel_ctx_t * ctx, uint64_t addr, void * buf, uint64_t len);
int sdcard_write(struct xfel_ctx_t * ctx, uint64_t addr, void * buf, uint64_t len);

#ifdef __cplusplus
}
#endif

#endif /* __SDCARD_H__ */
