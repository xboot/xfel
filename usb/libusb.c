#include <x.h>
#include <fel.h>
#include <usb.h>

#if !defined(USE_WINUSB_DRV)
#include <libusb.h>

static void xfel_libusb_bulk_send(void * hdl, int ep, const char * buf, size_t len)
{
	libusb_device_handle *handle = (libusb_device_handle *) hdl;
	size_t max_chunk = 128 * 1024;
	size_t chunk;
	int r, bytes;

	while(len > 0)
	{
		chunk = len < max_chunk ? len : max_chunk;
		r = libusb_bulk_transfer(handle, ep, (void *)buf, chunk, &bytes, 10000);
		if(r != 0)
		{
			printf("usb bulk send error\r\n");
			exit(-1);
		}
		len -= bytes;
		buf += bytes;
	}
}

static void xfel_libusb_bulk_recv(void * hdl, int ep, char * buf, size_t len)
{
	libusb_device_handle *handle = (libusb_device_handle *) hdl;
	int r, bytes;

	while(len > 0)
	{
		r = libusb_bulk_transfer(handle, ep, (void *)buf, len, &bytes, 10000);
		if(r != 0)
		{
			printf("usb bulk recv error\r\n");
			exit(-1);
		}
		len -= bytes;
		buf += bytes;
	}
}

static int xfel_libusb_open(struct xfel_ctx_t * ctx)
{
	libusb_device ** list = NULL;
	libusb_context * context = NULL;
	libusb_init(&context);
	int count = libusb_get_device_list(context, &list);
	for(int i = 0; i < count; i++)
	{
		libusb_device * device = list[i];
		struct libusb_device_descriptor desc;
		if(libusb_get_device_descriptor(device, &desc) != 0)
			printf("ERROR: Can't get device list\r\n");
		if((desc.idVendor == 0x1f3a) && (desc.idProduct == 0xefe8))
		{
			libusb_device_handle *libusb_hdl = NULL;
			if(libusb_open(device, &libusb_hdl) != 0)
				printf("ERROR: Can't connect to device\r\n");
			ctx->hdl = (void *) libusb_hdl;
			break;
		}
	}
	libusb_free_device_list(list, 1);
	return ctx->hdl ? 1 : 0;
}

static void xfel_libusb_close(struct xfel_ctx_t * ctx)
{
	if(ctx->hdl)
		libusb_close(ctx->hdl);
	libusb_exit(NULL);
}

static int xfel_libusb_init(struct xfel_ctx_t * ctx)
{
	if(ctx && ctx->hdl)
	{
		struct libusb_config_descriptor * config;
		int if_idx, set_idx, ep_idx;
		const struct libusb_interface * iface;
		const struct libusb_interface_descriptor * setting;
		const struct libusb_endpoint_descriptor * ep;

		if(libusb_kernel_driver_active(ctx->hdl, 0))
			libusb_detach_kernel_driver(ctx->hdl, 0);

		if(libusb_claim_interface(ctx->hdl, 0) == 0)
		{
			if(libusb_get_active_config_descriptor(libusb_get_device(ctx->hdl), &config) == 0)
			{
				for(if_idx = 0; if_idx < config->bNumInterfaces; if_idx++)
				{
					iface = config->interface + if_idx;
					for(set_idx = 0; set_idx < iface->num_altsetting; set_idx++)
					{
						setting = iface->altsetting + set_idx;
						for(ep_idx = 0; ep_idx < setting->bNumEndpoints; ep_idx++)
						{
							ep = setting->endpoint + ep_idx;
							if((ep->bmAttributes & LIBUSB_TRANSFER_TYPE_MASK) != LIBUSB_TRANSFER_TYPE_BULK)
								continue;
							if((ep->bEndpointAddress & LIBUSB_ENDPOINT_DIR_MASK) == LIBUSB_ENDPOINT_IN)
								ctx->epin = ep->bEndpointAddress;
							else
								ctx->epout = ep->bEndpointAddress;
						}
					}
				}
				libusb_free_config_descriptor(config);
				return 1;
			}
		}
	}
	return 0;
}

struct usb_ops_t xfel_libusb_ops = {
	.bulk_send = xfel_libusb_bulk_send,
	.bulk_recv = xfel_libusb_bulk_recv,
	.open = xfel_libusb_open,
	.close = xfel_libusb_close,
	.init = xfel_libusb_init,
};

#endif /* !USE_WINUSB_DRV */