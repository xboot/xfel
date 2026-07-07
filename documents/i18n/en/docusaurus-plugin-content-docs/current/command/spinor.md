# spinor

SPI NOR Flash commands: detect, erase, read and write.

## Usage

### Detect SPI NOR Flash

```shell
xfel spinor
```

### Erase

```shell
xfel spinor erase <address> <length>
```

### Read

```shell
xfel spinor read <address> <length> <file>
```

### Write

```shell
xfel spinor write <address> <file>
```

## Arguments

| Argument | Description |
|----------|-------------|
| `<address>` | Flash start address |
| `<length>` | Number of bytes to read / erase |
| `<file>` | File to save read data, or file to write |

## Description

These commands operate on SPI NOR Flash. Run `xfel spinor` first to detect the Flash model and capacity before performing any read/write operations.

- **erase**: erases the specified region (sets it to `0xFF`);
- **read**: reads Flash data to a local file;
- **write**: writes a local file to Flash (erase and program are handled internally).

:::note
SPI Flash operations depend on the chip's SPI initialization capability.
:::

## Examples

Detect SPI NOR Flash:

```shell
xfel spinor
```

Sample output:

```
Found spi nor flash 'W25Q128' with 16777216 bytes
```

Erase and write U-Boot:

```shell
xfel spinor erase 0x000000 0x80000
xfel spinor write 0x000000 u-boot.bin
```

Read Flash contents to a file:

```shell
xfel spinor read 0x000000 0x100000 flash_dump.bin
```
