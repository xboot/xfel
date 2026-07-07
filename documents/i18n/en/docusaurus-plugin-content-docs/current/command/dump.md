# dump

Dump a memory region as raw binary to standard output (stdout).

## Usage

```shell
xfel dump <address> <length>
```

## Arguments

| Argument | Description |
|----------|-------------|
| `<address>` | Start memory address |
| `<length>` | Number of bytes to read |

## Description

Unlike `hexdump`, `dump` writes raw binary data directly, suitable for piping to a file or another tool via redirection.

## Example

Save memory contents to a file:

```shell
xfel dump 0x80000000 1024 > memory.bin
```
