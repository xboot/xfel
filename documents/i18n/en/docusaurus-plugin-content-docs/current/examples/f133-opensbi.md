# F133: Write OpenSBI to DDR and Execute

This example shows how to use XFEL to initialize DDR, load OpenSBI into memory and execute it on the D1/F133.

## Applicable chips

- D1 / D1s / F133 (XuanTie C906 RISC-V)

## Steps

```shell
xfel ddr f133                          # Initialize the DDR controller for F133
xfel write 0x80200000 opensbi.bin      # Write OpenSBI to 0x80200000
xfel exec 0x80200000                   # Call the function address and execute
```

## Explanation

1. `xfel ddr f133` initializes the memory controller with the F133-specific DDR configuration;
2. `xfel write` loads `opensbi.bin` to memory address `0x80200000`;
3. `xfel exec` jumps to that address to start OpenSBI.

:::tip
D1 and F133 share the same chip ID (`0x00185900`); the DDR type can be `d1` or `f133`.
:::
