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
