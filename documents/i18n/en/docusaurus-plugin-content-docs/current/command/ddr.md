# ddr

Initialize the chip's DDR controller so that external memory becomes usable.

## Usage

```shell
xfel ddr [type]
```

## Arguments

| Argument | Description |
|----------|-------------|
| `[type]` | Optional DDR configuration type name. Different chips support different types, see the [DDR types reference](../reference/ddr-types) |

## Description

When entering FEL mode, the DDR controller is not yet initialized and memory is unavailable. After running this command, XFEL configures the DDR controller based on the chip model (and optional type), after which `write`, `exec` and other memory commands can be used.

If `type` is omitted, the chip's default DDR configuration is used. When a chip has multiple memory configurations, a specific type must be provided.
