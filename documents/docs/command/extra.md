# extra

执行芯片特定的扩展命令。不同芯片提供的扩展命令不同。

## 用法

```shell
xfel extra [...]
```

## 说明

`extra` 命令用于执行各芯片自定义的功能，如 efuse 读写、多核引导、安全启动检测等。具体可用的子命令取决于当前连接的芯片。执行不带参数的 `xfel extra` 时，芯片会打印其支持的扩展命令用法。

### 常见扩展命令

| 子命令 | 说明 | 支持芯片（部分） |
|--------|------|------------------|
| `efuse dump` | 转储全部 efuse 信息 | D1/F133、R528/T113、V821、V851/V853、A523/A527/T527、T153、R128、V3s/S3、A40i/R40 等 |
| `efuse read32 <offset>` | 读取 efuse 的 32 位值 | 同上 |
| `efuse write32 <offset> <value>` | 写入 efuse 的 32 位值 | 同上 |
| `efuse write <offset> <file>` | 将文件写入 efuse | 同上 |
| `exec arm64 <address>` | 引导 ARM64 核并跳转执行 | A523/A527/T527 |
| `exec riscv <address>` | 引导 RISC-V 核并跳转执行 | R128、V821 |
| `exec dsp <address>` | 引导 DSP 核并跳转执行 | R128 |
| `checksboot` | 检测安全启动是否启用 | A523/A527/T527、A733 |

## 示例

查看当前芯片支持的扩展命令：

```shell
xfel extra
```

转储 efuse 信息：

```shell
xfel extra efuse dump
```

引导 ARM64 核到指定地址执行：

```shell
xfel extra exec arm64 0x4a000000
```

:::warning
efuse 写入为一次性或不可逆操作，请务必确认后再执行，错误写入可能导致芯片无法启动。
:::
