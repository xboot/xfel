# sign

Generate an ECDSA256 signature file based on the SHA256 digest of the chip's SID.

## Usage

```shell
xfel sign <public-key> <private-key> <file>
```

## Arguments

| Argument | Description |
|----------|-------------|
| `<public-key>` | ECDSA256 public key (66 hex characters, 33 bytes) |
| `<private-key>` | ECDSA256 private key (64 hex characters, 32 bytes) |
| `<file>` | Path of the file to save the signature |

## Description

This command is used for secure boot scenarios. The flow is:

1. Read the chip SID;
2. Compute the SHA256 digest of the SID string;
3. Sign the digest with the private key using ECDSA256;
4. Verify the signature with the public key, and save the 64-byte signature to the specified file on success.

If the public or private key argument is empty, a built-in default key pair is used. This command depends on the chip supporting SID reading.

## Example

Generate a signature with a custom key pair and save it to `signature.bin`:

```shell
xfel sign 03cfd18e4a4b40d652944 8aa2df8bbb677128258b8fbfc5b9e492fbbba4e84832f \
  dc57b8a9e0e2b7f8b4c929bd8db2844e53f01f171bbcdf6e628908dbf2b2e6a9 \
  signature.bin
```

Sample output:

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
The private key is sensitive. Keep it safe and never leak it or commit it to a repository.
:::
