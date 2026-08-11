# version

显示已连接芯片的版本信息。

## 用法

```shell
xfel version
```

## 说明

该命令无需参数，用于确认 FEL 设备是否已成功连接，并输出芯片的识别信息，包括：

- **magic**：FEL 协议魔数；
- **ID**：芯片识别号（十六进制）及芯片名称；
- **dflag**：设备标志；
- **dlength**：数据长度；
- **scratchpad**：暂存区地址。
