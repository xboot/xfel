# F1C200s：向 SPI NAND Flash 写入系统

本示例演示如何使用 XFEL 将完整系统烧录到 SPI NAND Flash。

## 适用芯片

- F1C100S / F1C200S / F1C500S（带 SPI NAND Flash 的板子）

## 操作步骤

```shell
xfel spinand                                          # 探测 SPI NAND Flash
xfel spinand erase 0x000000 134217728                 # 擦除整个 Flash（128MB）
xfel spinand write 0x000000 u-boot-sunxi-with-nand-spl.bin   # 烧录 U-Boot（含 SPL）
xfel spinand write 0x80000 splash.bmp                 # 烧录开机画面
xfel spinand write 0x100000 kernel.itb                # 烧录内核
xfel spinand write 0x600000 rootfs.squashfs           # 烧录根文件系统
```

## 分区布局示例

| 偏移地址 | 内容 | 说明 |
|----------|------|------|
| `0x000000` | U-Boot（含 SPL） | 引导加载程序 |
| `0x080000` | splash.bmp | 开机画面 |
| `0x100000` | kernel.itb | 内核镜像（FIT 格式） |
| `0x600000` | rootfs.squashfs | 只读根文件系统 |

## 说明

1. 先执行 `xfel spinand` 探测 Flash，确认型号与容量；
2. 烧录前需擦除对应区域；
3. 各分区偏移地址需与 U-Boot 的分区表配置一致。

:::note
若 SPL 需要按页边界写入，可使用 `splwrite` 子命令：

```shell
xfel spinand splwrite 1048576 0x000000 u-boot-sunxi-with-nand-spl.bin
```
:::
