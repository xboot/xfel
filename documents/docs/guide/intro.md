# 关于 XFEL

XFEL 是一款面向全志（Allwinner）SOC 的轻量级 FEL 工具，通过 USB OTG 接口即可对芯片进行初始烧录、调试与恢复操作。

## 什么是 FEL

FEL 是全志 SOC BootROM 中内置的一段低级子程序。当芯片进入 FEL 模式后，主机可通过 USB 与之通信，完成内存读写、代码执行、Flash 烧录等操作，常用于设备初始编程与系统恢复。

进入 FEL 模式的一般方法：

1. 按住板载的 FEL / BOOT 按键；
2. 复位或上电开发板；
3. 主机通过 USB 连接设备（USB ID 为 `1f3a:efe8`）。

## 特性

- 跨平台支持：Linux、Windows、macOS；
- 基于 libusb-1.0，无需内核驱动；
- 内存读写、函数执行、看门狗复位；
- SID 读取、JTAG 调试使能；
- DDR 控制器初始化；
- SPI NOR Flash 与 SPI NAND Flash 探测、读写、擦除；
- 基于 ECDSA256 的 SID 签名；
- 芯片扩展命令（efuse 读写、多核引导等）。

## 支持的芯片

XFEL 支持数十款全志 SOC，详细的芯片型号与功能支持情况见[支持列表](../reference/support-list)。

## 相关链接

- 源码仓库：[https://github.com/xboot/xfel](https://github.com/xboot/xfel)
- 发布版本：[https://github.com/xboot/xfel/releases](https://github.com/xboot/xfel/releases)
- 中文讨论帖：[https://whycan.com/t_6546.html](https://whycan.com/t_6546.html)
- Linux-Sunxi 社区：[http://sunxi.org/](http://sunxi.org/)
