# 编译安装

本文档说明如何在各平台从源码编译 XFEL 工具，并安装到系统中。

## 依赖

XFEL 依赖于 `libusb-1.0` 库，编译前需先安装对应平台的开发包。

## Linux 平台

### 安装依赖

在 Ubuntu / Debian 上：

```shell
sudo apt install libusb-1.0-0-dev
```

### 编译与安装

在源码根目录执行：

```shell
cd xfel
make
sudo make install
```

编译完成后会生成 `xfel` 可执行文件，`make install` 会将其安装到系统路径（默认 `/usr/local/bin`）。

### Udev 规则（可选）

为避免每次都以 root 权限运行，可将仓库根目录下的 `99-xfel.rules` 拷贝到 udev 规则目录：

```shell
sudo cp 99-xfel.rules /etc/udev/rules.d/
sudo udevadm control --reload
```

该规则将 FEL 设备（`1f3a:efe8`）的访问权限设为 `666`，普通用户即可访问。

## Windows 平台

Windows 采用交叉编译方式，在 Ubuntu 中完成。

### 安装交叉编译工具链

```shell
sudo apt install mingw-w64
sudo apt install autoconf
sudo apt install libtool-bin
```

### 交叉编译 libusb

```shell
git clone https://github.com/libusb/libusb.git
cd libusb
./autogen.sh
./configure --host=x86_64-w64-mingw32 --prefix=/usr/x86_64-w64-mingw32/
make
sudo make install
```

### 编译 XFEL

```shell
cd xfel
CROSS=x86_64-w64-mingw32- make
```

编译 32 位 Windows 程序时，将 `CROSS` 替换为 `i686-w64-mingw32-`。

## macOS 平台

:::note
macOS ARM（Apple Silicon）支持待补充。
:::

### 安装 Command Line Tools

确保已安装 Xcode Command Line Tools：

```shell
xcode-select --install
```

### 安装 libusb

使用 [Homebrew](https://brew.sh/) 安装依赖：

```shell
brew install libusb
```

### 编译

```shell
cd xfel
make
```

## 预编译版本

如需直接使用，可在 [Releases](https://github.com/xboot/xfel/releases/latest) 页面下载 Windows 预编译版本。
