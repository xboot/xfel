# read32

Read a 32-bit value from device memory.

## Usage

```shell
xfel read32 <address>
```

## Arguments

| Argument | Description |
|----------|-------------|
| `<address>` | Memory address to read |

## Description

Reads 4 bytes at the given address and prints the value in hex. Useful for querying register values or verifying memory contents.

:::note
The FEL protocol only supports byte access; this command is implemented via byte read operations.
:::

## Example

```shell
xfel read32 0x01c20000
```

Sample output:

```
0x00000000
```
