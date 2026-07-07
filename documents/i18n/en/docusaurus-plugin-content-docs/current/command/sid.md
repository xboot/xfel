# sid

Read and display the chip's SID (Security ID).

## Usage

```shell
xfel sid
```

## Description

The SID is the unique security identifier (Unique ID) of Allwinner chips. Each chip has a different SID, commonly used for device binding and secure boot verification. This command reads the SID and prints it as a string.

Not all chips support this command. Some chips (such as the F1C100S family) have no SID and will report unsupported.

## Example

```shell
xfel sid
```

Sample output:

```
80000-28000045007e8c13d27a23dca0ffffef
```
