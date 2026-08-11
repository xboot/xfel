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
