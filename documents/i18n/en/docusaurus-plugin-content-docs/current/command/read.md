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
