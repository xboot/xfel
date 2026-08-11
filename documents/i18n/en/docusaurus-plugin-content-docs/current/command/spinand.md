# spinand

SPI NAND Flash commands: detect, erase, read, write, and split write.

## Usage

### Detect SPI NAND Flash

```shell
xfel spinand
```

### Erase

```shell
xfel spinand erase <address> <length>
```

### Read

```shell
xfel spinand read <address> <length> <file>
```

### Write

```shell
xfel spinand write <address> <file>
```

### Split write

```shell
xfel spinand splwrite <split-size> <address> <file>
```

## Arguments

| Argument | Description |
|----------|-------------|
| `<address>` | Flash start address |
| `<length>` | Number of bytes to read / erase |
| `<file>` | File to save read data, or file to write |
| `<split-size>` | Split block size for `splwrite` |

## Description

These commands operate on SPI NAND Flash. As with SPI NOR, the Flash model and capacity must be detected first.

- **erase**: erases the specified region;
- **read**: reads Flash data to a local file;
- **write**: writes a local file to Flash;
- **splwrite**: writes in chunks of the given size, used for SPI NAND SPL flashing to ensure the bootloader is written along page boundaries correctly.
