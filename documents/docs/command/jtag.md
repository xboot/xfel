# jtag

使能芯片的 JTAG 调试接口。

## 用法

```shell
xfel jtag
```

## 说明

该命令配置芯片的相关引脚与寄存器，开启 JTAG 调试功能，便于通过 JTAG 调试器（如 J-Link、ST-Link）连接调试。

并非所有芯片都支持该命令，若不支持将提示相应信息。

## 示例

```shell
xfel jtag
```
