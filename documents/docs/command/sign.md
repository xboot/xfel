# sign

基于芯片 SID 的 SHA256 摘要生成 ECDSA256 签名文件。

## 用法

```shell
xfel sign <public-key> <private-key> <file>
```

## 参数

| 参数 | 说明 |
|------|------|
| `<public-key>` | ECDSA256 公钥（66 个十六进制字符，33 字节） |
| `<private-key>` | ECDSA256 私钥（64 个十六进制字符，32 字节） |
| `<file>` | 签名结果保存的文件路径 |

## 说明

该命令用于安全启动场景，流程如下：

1. 读取芯片 SID；
2. 计算 SID 字符串的 SHA256 摘要；
3. 使用私钥对摘要进行 ECDSA256 签名；
4. 用公钥验证签名，验证通过后将 64 字节签名保存到指定文件。

若公钥或私钥参数为空，将使用内置的默认密钥对。该命令依赖芯片支持 SID 读取。

## 示例

使用自定义密钥对生成签名并保存到 `signature.bin`：

```shell
xfel sign 03cfd18e4a4b40d652944 8aa2df8bbb677128258b8fbfc5b9e492fbbba4e84832f \
  dc57b8a9e0e2b7f8b4c929bd8db2844e53f01f171bbcdf6e628908dbf2b2e6a9 \
  signature.bin
```

输出示例：

```
Unique ID:
    80000-28000045007e8c13d27a23dca0ffffef
Sha256 digest:
    ...
Ecdsa256 public key:
    ...
Ecdsa256 private key:
    ...
Ecdsa256 signature:
    ...
Ecdsa256 signature verify successed and saved to 'signature.bin'.
```

:::warning
私钥属于敏感信息，请妥善保管，切勿泄露或提交到版本库。
:::
