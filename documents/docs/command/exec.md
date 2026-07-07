# exec

调用并执行设备内存中指定地址的函数。

## 用法

```shell
xfel exec <address>
```

## 参数

| 参数 | 说明 |
|------|------|
| `<address>` | 待执行代码的内存地址 |

## 说明

该命令使 CPU 跳转到指定地址开始执行，常配合 `write` 命令先将固件加载到内存，再通过 `exec` 启动。

## 示例

加载并执行 U-Boot：

```shell
xfel write 0x81700000 u-boot.bin
xfel exec 0x81700000
```
