#include <x.h>
#include <fel.h>
#include <usb.h>

#if defined(USE_WINUSB_DRV) && defined(_WIN32)
#include <setupapi.h>
#include <initguid.h>
#include <usbiodef.h>
#include <windows.h>

static int match_vid_pid(const char *device_path) {
	if (!device_path)
		return 0;
	char pattern[64];
	snprintf(pattern, sizeof(pattern), "vid_%04x&pid_%04x", 0x1f3a, 0xefe8);
	const size_t n = strlen(device_path);

	char *lower = (char *) malloc(n + 1);

	if (!lower)
		return 0;
	for (size_t i = 0; i < n; ++i)
		lower[i] = (char) tolower((unsigned char) device_path[i]);

	lower[n] = '\0';

	const int found = strstr(lower, pattern) != NULL;
	free(lower);
	return found;
}

static void xfel_winusb_bulk_send(void * hdl, int ep, const char * buf, size_t len)
{
	HANDLE const usb_handle = (HANDLE) hdl;
	const size_t max_chunk = 128 * 1024;
	DWORD bytes_sent = 0;

	char *tmp_buf = (char *)malloc(max_chunk);
	if (!tmp_buf) {
		printf("usb bulk send malloc error\r\n");
		exit(-1);
	}

	while (len > 0) {
		const size_t chunk = len <= max_chunk ? len : max_chunk;
		memcpy(tmp_buf, buf, chunk);
		const DWORD dwIoControlCode = CTL_CODE(FILE_DEVICE_UNKNOWN, 0x0807, METHOD_OUT_DIRECT, FILE_ANY_ACCESS);
		BOOL const result =
				DeviceIoControl(usb_handle, dwIoControlCode, NULL, (DWORD) 0, (LPVOID) tmp_buf, chunk, &bytes_sent, NULL);
		if (!result || bytes_sent == 0) {
			free(tmp_buf);
			printf("usb bulk send error\r\n");
			exit(-1);
		}
		len -= bytes_sent;
		buf += bytes_sent;
	}
	free(tmp_buf);
}

static void xfel_winusb_bulk_recv(void * hdl, int ep, char * buf, size_t len)
{
	HANDLE const usb_handle = (HANDLE) hdl;
	DWORD bytes_received = 0;

	char *tmp_buf = (char *)malloc(len);
	if (!tmp_buf) {
		printf("usb bulk recv malloc error\r\n");
		exit(-1);
	}

	const DWORD dwIoControlCode = CTL_CODE(FILE_DEVICE_UNKNOWN, 0x0808, METHOD_IN_DIRECT, FILE_ANY_ACCESS);

	const BOOL result =
			DeviceIoControl(usb_handle, dwIoControlCode, NULL, 0, (LPVOID) tmp_buf, (DWORD) len, &bytes_received, NULL);

	if (!result || bytes_received == 0) {
		free(tmp_buf);
		printf("usb bulk recv error\r\n");
		exit(-1);
	}

	memcpy(buf, tmp_buf, bytes_received);
	free(tmp_buf);
}

static int xfel_winusb_open(struct xfel_ctx_t * ctx)
{
	SP_DEVICE_INTERFACE_DATA interface_data;
	ULONG index = 0;
	BOOL device_found = FALSE;

	const LPGUID usb_device_guid = (LPGUID) &GUID_DEVINTERFACE_USB_DEVICE;

	const HDEVINFO device_info_set =
			SetupDiGetClassDevs(usb_device_guid, NULL, NULL, DIGCF_PRESENT | DIGCF_DEVICEINTERFACE);

	if (device_info_set == INVALID_HANDLE_VALUE) {
		printf("usb winusb open error\r\n");
		return 0;
	}

	BOOL result = TRUE;

	while (result) {
		interface_data.cbSize = sizeof(interface_data);
		result = SetupDiEnumDeviceInterfaces(device_info_set, NULL, usb_device_guid, (ULONG) index, &interface_data);

		if (result) {
			DWORD required_size = 0;
			SetupDiGetDeviceInterfaceDetail(device_info_set, &interface_data, NULL, 0, &required_size, NULL);
			if (required_size == 0) {
				index++;
				continue;
			}

			const PSP_DEVICE_INTERFACE_DETAIL_DATA detail_data =
					(PSP_DEVICE_INTERFACE_DETAIL_DATA) malloc(required_size);
			if (detail_data == NULL) {
				SetupDiDestroyDeviceInfoList(device_info_set);
				return 0;
			}
			detail_data->cbSize = sizeof(SP_DEVICE_INTERFACE_DETAIL_DATA);

			if (SetupDiGetDeviceInterfaceDetail(device_info_set, &interface_data, detail_data, required_size, NULL,
			                                    NULL)) {
				if (match_vid_pid(detail_data->DevicePath)) {
					ctx->dev_name = (char *) malloc(strlen(detail_data->DevicePath) + 1);
					if (ctx->dev_name != NULL) {
						strcpy(ctx->dev_name, detail_data->DevicePath);
						device_found = TRUE;
						free(detail_data);
						break;
					}
					free(detail_data);
					SetupDiDestroyDeviceInfoList(device_info_set);
					return 0;
				}
			}
			free(detail_data);
			index++;
		}
	}

	SetupDiDestroyDeviceInfoList(device_info_set);

	return device_found ? 1 : 0;
}

static void xfel_winusb_close(struct xfel_ctx_t * ctx)
{
	if(ctx->hdl && ctx->hdl != (void *)INVALID_HANDLE_VALUE)
		CloseHandle((HANDLE)ctx->hdl);
}

static int xfel_winusb_init(struct xfel_ctx_t * ctx)
{
	if (!ctx || !ctx->dev_name) {
		printf("usb winusb init error, ctx or dev_name is NULL\r\n");
		return 0;
	}
	ctx->hdl = CreateFile(ctx->dev_name, GENERIC_WRITE | GENERIC_READ, FILE_SHARE_WRITE | FILE_SHARE_READ, NULL,
						OPEN_EXISTING, 0, NULL);

	if (ctx->hdl == INVALID_HANDLE_VALUE) {
		printf("usb winusb init error, CreateFile failed\r\n");
		return 0;
	}

	return 1;
}

struct usb_ops_t xfel_winusb_ops = {
	.bulk_send = xfel_winusb_bulk_send,
	.bulk_recv = xfel_winusb_bulk_recv,
	.open = xfel_winusb_open,
	.close = xfel_winusb_close,
	.init = xfel_winusb_init,
};

#endif // defined(USE_WINUSB_DRV) && defined(_WIN32)
