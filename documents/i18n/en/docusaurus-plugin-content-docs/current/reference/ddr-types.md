# DDR Types Reference

The optional `type` of `xfel ddr [type]` varies by chip. The table below lists the supported DDR type names for each chip. Chips not listed use a fixed default configuration (no `type` needed).

| Chip | Supported DDR types |
|------|---------------------|
| D1 / F133 | `d1`, `f133` |
| H2 / H3 | `h2`, `h3` |
| H616 / H313 / H618 | `lpddr4` |
| V3s / S3 | `v3s`, `s3` |
| V851 / V853 | `v851`, `v853` |
| R528 / T113 | `r528-s3`, `t113-s3`, `t113-s4` |
| R128 | `r128-s2`, `r128-s3`, `xr875` |
| T153 | `t153-m3` |
| A40i / R40 | `ddr3`, `lpddr3` |
| A523 / A527 / T527 | `lpddr4` |

## Usage examples

```shell
# D1 / F133
xfel ddr f133

# R528-S3
xfel ddr r528-s3

# A523 family with LPDDR4
xfel ddr lpddr4
```

:::note
If you are unsure which type to use, just run `xfel ddr` to use the chip default. Specify a type only when the on-board memory differs from the default configuration.
:::
