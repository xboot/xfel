# About XFEL

XFEL is a lightweight FEL tool for Allwinner SOCs. It communicates with the chip's BootROM FEL subroutine over USB OTG to perform initial flashing, debugging and recovery.

## What is FEL

FEL is a low-level subroutine contained in the BootROM of Allwinner SOCs. Once a chip enters FEL mode, the host can talk to it over USB to read/write memory, execute code and flash storage, which is commonly used for initial device programming and recovery.

To enter FEL mode:

1. Hold the board's FEL / BOOT button;
2. Reset or power on the board;
3. Connect the device to the host via USB (USB ID `1f3a:efe8`).

## Features

- Cross-platform: Linux, Windows and macOS;
- Built on libusb-1.0, no kernel driver required;
- Memory read/write, code execution, watchdog reset;
- SID reading, JTAG debug enable;
- DDR controller initialization;
- SPI NOR Flash and SPI NAND Flash detect, read, write and erase;
- ECDSA256 signature generation based on SID;
- Chip-specific extra commands (efuse access, multi-core boot, etc.).

## Supported Chips

XFEL supports dozens of Allwinner SOCs. See the [Support List](../reference/support-list) for detailed chip models and feature support.

## Links

- Source repository: [https://github.com/xboot/xfel](https://github.com/xboot/xfel)
- Releases: [https://github.com/xboot/xfel/releases](https://github.com/xboot/xfel/releases)
- Discussion thread (Chinese): [https://whycan.com/t_6546.html](https://whycan.com/t_6546.html)
- Linux-Sunxi community: [http://sunxi.org/](http://sunxi.org/)
