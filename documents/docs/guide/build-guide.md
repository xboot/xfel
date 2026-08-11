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

编译完成后会生成 `xfel` 可执行文件。`make install` 会安装以下文件：

| 文件 | 默认路径 |
| --- | --- |
| `xfel` 可执行文件 | `/usr/local/bin/xfel` |
| udev 规则 `99-xfel.rules` | `/etc/udev/rules.d/99-xfel.rules` |
| 许可证 `LICENSE` | `/usr/local/share/licenses/xfel/LICENSE` |

### 自定义安装路径

安装路径通过以下变量控制（均可在命令行覆盖）：

| 变量 | 默认值 | 说明 |
| --- | --- | --- |
| `PREFIX` | `/usr/local` | 可执行文件与许可证的安装前缀 |
| `DESTDIR` | （空） | 暂存目录，前置于所有安装路径之前，便于打包 |
| `UDEV_RULES_DIR` | `/etc/udev/rules.d` | udev 规则的安装目录 |

安装到 `/usr` 前缀、并将 udev 规则放入 `/lib/udev/rules.d`：

```shell
sudo make install PREFIX=/usr UDEV_RULES_DIR=/lib/udev/rules.d
```

发行版打包时通常配合 `DESTDIR` 进行暂存安装：

```shell
make install PREFIX=/usr DESTDIR=$pkgdir UDEV_RULES_DIR=/lib/udev/rules.d
```

### Udev 规则（可选）

> `make install` 默认已安装 udev 规则，可通过 `UDEV_RULES_DIR` 变量修改其目录。本节适用于未执行 `make install`、仅手动放置规则的情况。

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
