# F1C100s：写入 U-Boot 到 DDR 并执行

本示例演示如何使用 XFEL 在 F1C100s 上初始化 DDR、加载 U-Boot 到内存并跳转执行。

## 适用芯片

- F1C100S / F1C200S / F1C500S（Single-core ARM9）

## 操作步骤

```shell
xfel ddr                               # 初始化 DDR 控制器
xfel write 0x81700000 u-boot.bin       # 将 U-Boot 写入 0x81700000
xfel exec 0x81700000                   # 调用函数地址执行
```

## 说明

1. `xfel ddr` 初始化 F1C100s 的 DDR 控制器，使外接 SDRAM 可用；
2. `xfel write` 将编译好的 `u-boot.bin` 加载到内存地址 `0x81700000`；
3. `xfel exec` 跳转到该地址开始执行 U-Boot。

该方式不会修改 Flash 内容，仅用于临时调试与运行。
