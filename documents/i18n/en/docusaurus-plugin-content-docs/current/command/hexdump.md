# hexdump

Dump a memory region in hexadecimal format.

## Usage

```shell
xfel hexdump <address> <length>
```

## Arguments

| Argument | Description |
|----------|-------------|
| `<address>` | Start memory address (decimal or `0x` prefixed hex) |
| `<length>` | Number of bytes to read |

## Description

Reads device memory and prints it with address, hex values and ASCII characters, making it easy to inspect memory contents.

## Example

Dump 256 bytes starting at `0x80000000`:

```shell
xfel hexdump 0x80000000 256
```
