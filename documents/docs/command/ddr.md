# ddr

初始化芯片的 DDR 控制器，使外接内存可用。

## 用法

```shell
xfel ddr [type]
```

## 参数

| 参数 | 说明 |
|------|------|
| `[type]` | 可选，DDR 配置类型名称。不同芯片支持不同的类型，见[DDR 类型参考](../reference/ddr-types) |

## 说明

进入 FEL 模式时，DDR 控制器尚未初始化，内存不可用。执行该命令后，XFEL 会根据芯片型号（及可选的类型参数）配置 DDR 控制器，之后即可使用 `write`、`exec` 等命令操作内存。

若不指定 `type`，将使用芯片的默认 DDR 配置；当芯片存在多种内存配置时，需指定具体类型。

## 示例

使用默认配置初始化 DDR：

```shell
xfel ddr
```

为 F133 指定 DDR 类型：

```shell
xfel ddr f133
```

为 R528 指定型号：

```shell
xfel ddr r528-s3
```

初始化失败时输出：

```
Failed to initial ddr controller
```

详细的类型列表见[DDR 类型参考](../reference/ddr-types)。
