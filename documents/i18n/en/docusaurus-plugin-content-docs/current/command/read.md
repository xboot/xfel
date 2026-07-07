# read

Read data from device memory and save it to a file, with a progress bar.

## Usage

```shell
xfel read <address> <length> <file>
```

## Arguments

| Argument | Description |
|----------|-------------|
| `<address>` | Start memory address |
| `<length>` | Number of bytes to read |
| `<file>` | Path of the file to save the data |

## Description

Reads the specified amount of memory and writes it to a local file. A progress bar is shown during transfer, suitable for reading larger blocks of data.

## Example

Read 1 MB from address `0x81700000` to `dump.bin`:

```shell
xfel read 0x81700000 0x100000 dump.bin
```
