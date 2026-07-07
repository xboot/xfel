# F1C200s: Write a System to SPI NAND Flash

This example shows how to use XFEL to flash a complete system to SPI NAND Flash.

## Applicable chips

- F1C100S / F1C200S / F1C500S (boards with SPI NAND Flash)

## Steps

```shell
xfel spinand                                          # Detect SPI NAND Flash
xfel spinand erase 0x000000 134217728                 # Erase the whole Flash (128 MB)
xfel spinand write 0x000000 u-boot-sunxi-with-nand-spl.bin   # Write U-Boot (with SPL)
xfel spinand write 0x80000 splash.bmp                 # Write boot splash
xfel spinand write 0x100000 kernel.itb                # Write kernel
xfel spinand write 0x600000 rootfs.squashfs           # Write root filesystem
```

## Example partition layout

| Offset | Content | Description |
|--------|---------|-------------|
| `0x000000` | U-Boot (with SPL) | Bootloader |
| `0x080000` | splash.bmp | Boot splash image |
| `0x100000` | kernel.itb | Kernel image (FIT format) |
| `0x600000` | rootfs.squashfs | Read-only root filesystem |

## Explanation

1. Run `xfel spinand` first to detect the Flash and confirm the model and capacity;
2. Erase the corresponding region before writing;
3. The partition offsets must match U-Boot's partition table.

:::note
If the SPL must be written along page boundaries, use the `splwrite` subcommand:

```shell
xfel spinand splwrite 1048576 0x000000 u-boot-sunxi-with-nand-spl.bin
```
:::
