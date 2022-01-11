#ifndef __SD_H__
#define __SD_H__

#ifdef __cplusplus
extern "C" {
#endif

#include <fel.h>

int sd_detect(struct xfel_ctx_t * ctx, char * name, uint64_t * capacity);
int sd_read(struct xfel_ctx_t * ctx, uint64_t addr, void * buf, uint64_t len);
int sd_write(struct xfel_ctx_t * ctx, uint64_t addr, void * buf, uint64_t len);

#ifdef __cplusplus
}
#endif

#endif /* __SD_H__ */
