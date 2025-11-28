#include <x.h>
#include <fel.h>
#include <usb.h>

#if defined(USE_WINUSB_DRV) && defined(_WIN32)
extern struct usb_ops_t xfel_winusb_ops;
struct usb_ops_t * usb_ops = &xfel_winusb_ops;
#else // defined(USE_WINUSB_DRV) && defined(_WIN32)
extern struct usb_ops_t xfel_libusb_ops;
struct usb_ops_t * usb_ops = &xfel_libusb_ops;
#endif // defined(USE_WINUSB_DRV) && defined(_WIN32)
