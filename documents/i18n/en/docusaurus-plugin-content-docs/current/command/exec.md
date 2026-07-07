# exec

Call and execute a function at the given memory address.

## Usage

```shell
xfel exec <address>
```

## Arguments

| Argument | Description |
|----------|-------------|
| `<address>` | Memory address of the code to execute |

## Description

Makes the CPU jump to the given address and start executing. It is typically combined with `write` to first load firmware into memory, then launch it with `exec`.

## Example

Load and execute U-Boot:

```shell
xfel write 0x81700000 u-boot.bin
xfel exec 0x81700000
```
