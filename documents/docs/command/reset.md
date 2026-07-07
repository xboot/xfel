# reset

通过看门狗（watchdog）复位设备。

## 用法

```shell
xfel reset
```

## 说明

该命令触发芯片的看门狗复位机制，使设备重新启动。并非所有芯片都支持该命令，若不支持将提示相应信息。

## 示例

```shell
xfel reset
```

若芯片不支持，输出：

```
The '<芯片名称>' chip don't support reset command
```
