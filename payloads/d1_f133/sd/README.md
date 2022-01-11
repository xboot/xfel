## build

```shell
export PATH=$PATH:/opt/Xuantie-900-gcc-elf-newlib-x86_64-V2.2.4/bin
echo $PATH
riscv64-unknown-elf-gcc  --version

make CROSS=riscv64-unknown-elf-
xxd -g 1 -i  output/sd.bin > payload.inc

```
