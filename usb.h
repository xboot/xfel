#ifndef __USB_H__
#define __USB_H__

#ifdef __cplusplus
extern "C" {
#endif

#include <x.h>

struct xfel_ctx_t;

struct usb_request_t {
	char magic[8];
	uint32_t length;
	uint32_t unknown1;
	uint16_t request;
	uint32_t length2;
	char pad[10];
} __attribute__((packed));

struct usb_ops_t {
	void (*bulk_send)(void * hdl, int ep, const char * buf, size_t len);
	void (*bulk_recv)(void * hdl, int ep, char * buf, size_t len);
	int (*open)(struct xfel_ctx_t * ctx);
	void (*close)(struct xfel_ctx_t * ctx);
	int (*init)(struct xfel_ctx_t * ctx);
};

extern struct usb_ops_t * usb_ops;

void usb_write(struct xfel_ctx_t * ctx, const void * buf, size_t len);
void usb_read(struct xfel_ctx_t * ctx, const void * data, size_t len);
int usb_init(struct xfel_ctx_t * ctx);
int usb_open(struct xfel_ctx_t * ctx);
void usb_close(struct xfel_ctx_t * ctx);

#ifdef __cplusplus
}
#endif

#endif /* __USB_H__ */
