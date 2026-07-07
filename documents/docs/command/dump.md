# dump

以二进制格式转储指定内存区域并输出到标准输出（stdout）。

## 用法

```shell
xfel dump <address> <length>
```

## 参数

| 参数 | 说明 |
|------|------|
| `<address>` | 内存起始地址 |
| `<length>` | 读取的字节数 |

## 说明

与 `hexdump` 不同，`dump` 直接输出原始二进制数据，便于通过重定向保存到文件或交由其他工具处理。

## 示例

将内存数据保存到文件：

```shell
xfel dump 0x80000000 1024 > memory.bin
```
