# reset

Reset the device using the watchdog.

## Usage

```shell
xfel reset
```

## Description

Triggers the chip's watchdog reset mechanism to restart the device. Not all chips support this command; if unsupported, a corresponding message is printed.

## Example

```shell
xfel reset
```

When the chip does not support it:

```
The '<chip name>' chip don't support reset command
```
