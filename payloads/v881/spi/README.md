# V861/V881 SPI NOR payload

This payload runs on the E907 in FEL mode and accesses the dedicated SPIF
controller at `0x04f00000`. It uses SRAM only; DDR initialization and C907
startup are not required. The controller uses the 24 MHz oscillator.

## Build

Normal host builds use the checked-in `chips/v881-spi.inc` and do not need
a RISC-V compiler. To rebuild the payload, use a GNU toolchain supporting
`rv32imac_zicsr_zifencei` with the `ilp32` ABI:

```sh
make -C payloads/v881/spi CROSS=riscv32-unknown-linux-gnu-
make -C payloads/v881/spi CROSS=riscv32-unknown-linux-gnu- check
```

The commands run from the xfel source directory. `check` also requires
`xxd` and compares the generated initializer with the checked-in one.
GCC 16.1.0 and GNU binutils 2.46 reproduce the current initializer exactly;
different compiler versions may generate different instruction sequences.

After changing the payload, regenerate the initializer explicitly:

```sh
make -C payloads/v881/spi CROSS=riscv32-unknown-linux-gnu- update
make
```

`OUTPUT` can select another build directory. The linker rejects payloads
whose code, data and BSS exceed the reserved 4 KiB region.

## SRAM interface

| Address range | Purpose |
| --- | --- |
| `0x00100000`–`0x00100fff` | Code, data and BSS |
| `0x00101000`–`0x001010ff` | Command buffer |
| Below `0x00101f00` | Private descending stack |
| `0x00101f80`–`0x00101f8f` | SPIF register snapshot |
| `0x00101ffc`–`0x00101fff` | Result status |
| `0x00102000`–`0x00111fff` | 64 KiB transfer buffer |

The entry point saves and restores the FEL stack. The command engine
handles the existing select, transfer, deselect and NOR busy-wait commands.
It reports a nonzero status for controller timeouts or invalid commands.

## Scope

The implementation supports single-lane SPI NOR operations with three-byte
addresses. SFDP reads, page programming and 4/32/64 KiB erase commands are
handled. Four-byte addressing, SPI NAND, quad I/O and DTR are not supported.

Hardware validation used an Avaota F2 with a 16 MiB Puya PY25Q128HA. Full
flash reads were compared and boot-image writes were checked by readback.
Always keep a complete backup before erasing or writing a flash device.
