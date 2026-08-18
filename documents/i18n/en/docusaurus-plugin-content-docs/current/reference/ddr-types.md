# DDR Parameters

The table below lists all chips that support DDR initialization. Chips that require a `type` list their supported type names; chips marked as "default config" need no `type`, just run `xfel ddr`. Chips not listed do not support DDR initialization.

| Chip | Supported DDR types |
|------|---------------------|
| D1 / D1s / F133 | `d1`, `f133` |
| H2 / H3 | `h2`, `h3` |
| H616 / H313 / H618 / T507 | `lpddr4` |
| V3s / S3 | `v3s`, `s3` |
| V851 / V853 | `v851`, `v853` |
| R528 / T113 | `r528-s3`, `t113-s3`, `t113-s4` |
| R128 | `r128-s2`, `r128-s3`, `xr875` |
| T153 | `t153-m3` |
| A40i / R40 | `ddr3`, `lpddr3` |
| A523 / A527 / T527 / MR527 | `lpddr4` |
| F1C100S / F1C200S / F1C500S | Default config (no type needed) |
| F101 | `f101-s2`, `f101-s3` |
| V821 | Default config (no type needed) |
| V831 | Default config (no type needed) |
