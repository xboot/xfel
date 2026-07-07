# jtag

Enable the chip's JTAG debug interface.

## Usage

```shell
xfel jtag
```

## Description

Configures the chip's pins and registers to enable JTAG debugging, so a JTAG debugger (such as J-Link, ST-Link) can be attached.

Not all chips support this command; if unsupported, a corresponding message is printed.

## Example

```shell
xfel jtag
```
