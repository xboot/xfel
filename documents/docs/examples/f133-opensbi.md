# F133：写入 OpenSBI 到 DDR 并执行

本示例演示如何使用 XFEL 在 D1/F133 上初始化 DDR、加载 OpenSBI 到内存并执行。

## 适用芯片

- D1 / D1s / F133（XuanTie C906 RISC-V）

## 操作步骤

```shell
xfel ddr f133                          # 为 F133 初始化 DDR 控制器
xfel write 0x80200000 opensbi.bin      # 将 OpenSBI 写入 0x80200000
xfel exec 0x80200000                   # 调用函数地址执行
```

## 说明

1. `xfel ddr f133` 使用 F133 专用的 DDR 配置初始化内存控制器；
2. `xfel write` 将 `opensbi.bin` 加载到内存地址 `0x80200000`；
3. `xfel exec` 跳转到该地址开始执行 OpenSBI。

:::tip
D1 与 F133 共享同一芯片 ID（`0x00185900`），DDR 类型可使用 `d1` 或 `f133`。
:::
