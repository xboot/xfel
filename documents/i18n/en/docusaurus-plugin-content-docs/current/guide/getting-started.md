# Quick Start

This section walks you through a simple example to get started with XFEL in minutes.

## Prerequisites

1. XFEL has been built or downloaded as described in [Build and Install](./build-guide);
2. An Allwinner SOC development board that supports FEL mode;
3. A USB data cable connecting the board's OTG port to the host.

## Step 1: Put the device into FEL mode

1. Hold the FEL (or BOOT) button on the board;
2. Reset or power on the board;
3. Connect to the host via USB.

## Step 2: Verify the connection

Run the following command to check the chip version:

```shell
xfel version
```

If the connection is successful, you will see output like:

```
AWUC    ID=0x00185900(D1/F133) dflag=0x00 dlength=0x10 scratchpad=0x00008000
```

`ID` is the chip identifier and the name is shown in parentheses. If you see `No FEL device found!`, check whether the device is in FEL mode, the USB connection, and the udev rules.

## Step 3: Initialize DDR and execute code

Taking D1/F133 as an example, initialize DDR and load firmware into memory to execute:

```shell
xfel ddr f133                          # Initialize the DDR controller
xfel write 0x80200000 opensbi.bin      # Write firmware to memory
xfel exec 0x80200000                   # Jump and execute
```

## Next steps

- [Command Reference](../command/version): learn all available commands;
- [Examples](../examples/f1c100s-uboot): see complete workflows for typical scenarios;
- [Support List](../reference/support-list): confirm feature support for your chip.
