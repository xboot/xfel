# spinor

SPI NOR Flash 操作命令，支持探测、擦除、读取与写入。

## 用法

### 探测 SPI NOR Flash

```shell
xfel spinor
```

### 擦除

```shell
xfel spinor erase <address> <length>
```

### 读取

```shell
xfel spinor read <address> <length> <file>
```

### 写入

```shell
xfel spinor write <address> <file>
```

## 参数

| 参数 | 说明 |
|------|------|
| `<address>` | Flash 起始地址 |
| `<length>` | 读 / 擦除的字节数 |
| `<file>` | 读取保存或待写入的文件路径 |

## 说明

该组命令用于操作 SPI NOR Flash。执行读写前需先执行 `xfel spinor` 探测 Flash 型号与容量，探测成功后方可进行后续操作。

- **erase**：擦除指定区域，擦除会将其内容置为 `0xFF`；
- **read**：将 Flash 数据读取到本地文件；
- **write**：将本地文件写入 Flash（内部自动处理擦除与编程）。

:::note
SPI Flash 操作依赖芯片的 SPI 初始化能力，需芯片支持。
:::

## 示例

探测 SPI NOR Flash：

```shell
xfel spinor
```

输出示例：

```
Found spi nor flash 'W25Q128' with 16777216 bytes
```

擦除并写入 U-Boot：

```shell
xfel spinor erase 0x000000 0x80000
xfel spinor write 0x000000 u-boot.bin
```

读取 Flash 内容到文件：

```shell
xfel spinor read 0x000000 0x100000 flash_dump.bin
```
