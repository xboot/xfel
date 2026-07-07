# extra

Run chip-specific extra commands. The available commands differ per chip.

## Usage

```shell
xfel extra [...]
```

## Description

The `extra` command invokes chip-specific functionality such as efuse access, multi-core boot, and secure boot checks. The exact subcommands depend on the connected chip. Running `xfel extra` without arguments prints the usage of the supported extra commands for the current chip.

### Common extra commands

| Subcommand | Description | Supported chips (partial) |
|------------|-------------|---------------------------|
| `efuse dump` | Dump all efuse information | D1/F133, R528/T113, V821, V851/V853, A523/A527/T527, T153, R128, V3s/S3, A40i/R40 |
| `efuse read32 <offset>` | Read a 32-bit value from efuse | same as above |
| `efuse write32 <offset> <value>` | Write a 32-bit value to efuse | same as above |
| `efuse write <offset> <file>` | Write a file to efuse | same as above |
| `exec arm64 <address>` | Boot the ARM64 core and jump to address | A523/A527/T527 |
| `exec riscv <address>` | Boot the RISC-V core and jump to address | R128, V821 |
| `exec dsp <address>` | Boot the DSP core and jump to address | R128 |
| `checksboot` | Check whether secure boot is enabled | A523/A527/T527, A733 |

## Examples

List the extra commands supported by the current chip:

```shell
xfel extra
```

Dump efuse information:

```shell
xfel extra efuse dump
```

Boot the ARM64 core to a given address:

```shell
xfel extra exec arm64 0x4a000000
```

:::warning
Writing efuse is a one-time or irreversible operation. Make sure before executing; an incorrect write may brick the chip.
:::
