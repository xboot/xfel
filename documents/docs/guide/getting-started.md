# 快速开始

本节通过一个简单示例，帮助你在几分钟内上手 XFEL。

## 前置准备

1. 已按[编译安装](./build-guide)完成 XFEL 的编译或下载；
2. 一块支持 FEL 模式的全志 SOC 开发板；
3. 一根 USB 数据线（连接开发板 OTG 接口与主机）。

## 第一步：让设备进入 FEL 模式

1. 按住开发板上的 FEL（或 BOOT）按键；
2. 复位或上电开发板；
3. 通过 USB 连接到主机。

## 第二步：确认设备连接

执行以下命令查看芯片版本：

```shell
xfel version
```

若连接正常，将输出类似如下信息：

```
AWUC    ID=0x00185900(D1/F133) dflag=0x00 dlength=0x10 scratchpad=0x00008000
```

其中 `ID` 为芯片识别号，括号内为芯片名称。若提示 `No FEL device found!`，请检查设备是否已进入 FEL 模式、USB 连接以及 udev 规则。

## 第三步：初始化 DDR 并执行代码

以 D1/F133 为例，初始化 DDR 并将固件加载到内存中执行：

```shell
xfel ddr f133                          # 初始化 DDR 控制器
xfel write 0x80200000 opensbi.bin      # 将固件写入内存
xfel exec 0x80200000                   # 跳转执行
```

## 更多操作

- [命令参考](../command/version)：了解所有可用命令；
- [使用示例](../examples/f1c100s-uboot)：查看典型场景的完整流程；
- [支持列表](../reference/support-list)：确认芯片功能支持情况。
