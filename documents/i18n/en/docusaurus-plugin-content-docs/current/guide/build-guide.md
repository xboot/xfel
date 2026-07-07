# Build and Install

This document describes how to build XFEL from source on each platform and install it to your system.

## Dependencies

XFEL depends on the `libusb-1.0` library. Install the development package for your platform before compiling.

## Linux

### Install dependencies

On Ubuntu / Debian:

```shell
sudo apt install libusb-1.0-0-dev
```

### Build and install

From the source root directory:

```shell
cd xfel
make
sudo make install
```

This produces the `xfel` executable; `make install` copies it to the system path (default `/usr/local/bin`).

### Udev rules (optional)

To avoid running as root every time, copy `99-xfel.rules` from the repository root to the udev rules directory:

```shell
sudo cp 99-xfel.rules /etc/udev/rules.d/
sudo udevadm control --reload
```

This rule sets the FEL device (`1f3a:efe8`) access mode to `666` so non-root users can access it.

## Windows

Windows binaries are produced via cross-compilation on Ubuntu.

### Install the cross toolchain

```shell
sudo apt install mingw-w64
sudo apt install autoconf
sudo apt install libtool-bin
```

### Cross-compile libusb

```shell
git clone https://github.com/libusb/libusb.git
cd libusb
./autogen.sh
./configure --host=x86_64-w64-mingw32 --prefix=/usr/x86_64-w64-mingw32/
make
sudo make install
```

### Build XFEL

```shell
cd xfel
CROSS=x86_64-w64-mingw32- make
```

For 32-bit Windows, replace `CROSS` with `i686-w64-mingw32-`.

## macOS

:::note
macOS ARM (Apple Silicon) support is to be added.
:::

### Install Command Line Tools

Make sure Xcode Command Line Tools are installed:

```shell
xcode-select --install
```

### Install libusb

Install the dependency with [Homebrew](https://brew.sh/):

```shell
brew install libusb
```

### Build

```shell
cd xfel
make
```

## Prebuilt binaries

To use XFEL directly, download the prebuilt Windows binary from the [Releases](https://github.com/xboot/xfel/releases/latest) page.
