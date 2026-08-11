# write32

Write a 32-bit value to device memory.

## Usage

```shell
xfel write32 <address> <value>
```

## Arguments

| Argument | Description |
|----------|-------------|
| `<address>` | Memory address to write |
| `<value>` | 32-bit value to write |

## Description

Writes 4 bytes to the given address, commonly used to configure registers or modify memory flags.

:::note
The FEL protocol only supports byte access; this command is implemented via byte write operations.
:::
