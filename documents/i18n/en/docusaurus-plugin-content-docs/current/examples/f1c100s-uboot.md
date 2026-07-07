# F1C100s: Write U-Boot to DDR and Execute

This example shows how to use XFEL to initialize DDR, load U-Boot into memory and execute it on the F1C100s.

## Applicable chips

- F1C100S / F1C200S / F1C500S (Single-core ARM9)

## Steps

```shell
xfel ddr                               # Initialize the DDR controller
xfel write 0x81700000 u-boot.bin       # Write U-Boot to 0x81700000
xfel exec 0x81700000                   # Call the function address and execute
```

## Explanation

1. `xfel ddr` initializes the F1C100s DDR controller so external SDRAM is usable;
2. `xfel write` loads the compiled `u-boot.bin` to memory address `0x81700000`;
3. `xfel exec` jumps to that address to start U-Boot.

This approach does not modify Flash contents and is only for temporary debugging and execution.
