# version

Display the version information of the connected chip.

## Usage

```shell
xfel version
```

## Description

This command takes no arguments. It confirms that a FEL device is connected and prints the chip identification, including:

- **magic**: FEL protocol magic;
- **ID**: chip identifier (hex) and chip name;
- **dflag**: device flag;
- **dlength**: data length;
- **scratchpad**: scratchpad address.

## Example

```shell
xfel version
```

Sample output:

```
AWUC    ID=0x00185900(D1/F133) dflag=0x00 dlength=0x10 scratchpad=0x00008000
```

When no device is connected or the device is not in FEL mode, it prints `No FEL device found!`.
