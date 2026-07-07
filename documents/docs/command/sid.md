# sid

读取并显示芯片的 SID（Security ID）信息。

## 用法

```shell
xfel sid
```

## 说明

SID 是全志芯片的唯一安全标识符（Unique ID），每颗芯片各不相同，常用于设备绑定与安全启动校验。该命令读取并以字符串形式输出 SID。

并非所有芯片都支持该命令，部分芯片（如 F1C100S 系列）无 SID，将提示不支持。

## 示例

```shell
xfel sid
```

输出示例：

```
80000-28000045007e8c13d27a23dca0ffffef
```
