#include <x.h>
#include <fel.h>
#include <usb.h>

static inline void send_usb_request(struct xfel_ctx_t * ctx, int type, size_t length)
{
	struct usb_request_t req = {
		.magic = "AWUC",
		.request = cpu_to_le16(type),
		.length = cpu_to_le32(length),
		.unknown1 = cpu_to_le32(0x0c000000)
	};
	req.length2 = req.length;
	usb_ops->bulk_send(ctx->hdl, ctx->epout, (const char *)&req, sizeof(struct usb_request_t));
}

static inline void read_usb_response(struct xfel_ctx_t * ctx)
{
	char buf[13];

	usb_ops->bulk_recv(ctx->hdl, ctx->epin, (char *)buf, sizeof(buf));
	assert(strcmp(buf, "AWUS") == 0);
}

void usb_write(struct xfel_ctx_t * ctx, const void * buf, size_t len)
{
	send_usb_request(ctx, 0x12, len);
	usb_ops->bulk_send(ctx->hdl, ctx->epout, (const char *)buf, len);
	read_usb_response(ctx);
}

void usb_read(struct xfel_ctx_t * ctx, const void * data, size_t len)
{
	send_usb_request(ctx, 0x11, len);
	usb_ops->bulk_recv(ctx->hdl, ctx->epin, (char *)data, len);
	read_usb_response(ctx);
}

int usb_open(struct xfel_ctx_t * ctx)
{
	return usb_ops->open(ctx);
}

void usb_close(struct xfel_ctx_t * ctx)
{
	usb_ops->close(ctx);
}

int usb_init(struct xfel_ctx_t * ctx)
{
	return usb_ops->init(ctx);
}
