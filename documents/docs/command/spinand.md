# spinand

SPI NAND Flash 操作命令，支持探测、擦除、读取、写入及带拆分的写入。

## 用法

### 探测 SPI NAND Flash

```shell
xfel spinand
```

### 擦除

```shell
xfel spinand erase <address> <length>
```

### 读取

```shell
xfel spinand read <address> <length> <file>
```

### 写入

```shell
xfel spinand write <address> <file>
```

### 带拆分写入

```shell
xfel spinand splwrite <split-size> <address> <file>
```

## 参数

| 参数 | 说明 |
|------|------|
| `<address>` | Flash 起始地址 |
| `<length>` | 读 / 擦除的字节数 |
| `<file>` | 读取保存或待写入的文件路径 |
| `<split-size>` | `splwrite` 的拆分块大小 |

## 说明

该组命令用于操作 SPI NAND Flash。与 SPI NOR 类似，需先探测 Flash 型号与容量。

- **erase**：擦除指定区域；
- **read**：将 Flash 数据读取到本地文件；
- **write**：将本地文件写入 Flash；
- **splwrite**：按指定块大小拆分写入，用于 SPI NAND SPL 烧录场景，确保引导程序按页边界正确写入。
