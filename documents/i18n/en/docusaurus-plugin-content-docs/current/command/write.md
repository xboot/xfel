# write

Write a local file to device memory, with a progress bar.

## Usage

```shell
xfel write <address> <file>
```

## Arguments

| Argument | Description |
|----------|-------------|
| `<address>` | Start memory address |
| `<file>` | Path of the local file to write |

## Description

Reads the entire contents of a local file and writes it to device memory at the given address. A progress bar is shown during transfer. This is the most commonly used command for loading firmware into memory.
