# Weston DPMS Extension

Module for Weston shell providing DPMS functionality.

## Setup

1. Install Pre-requisites.
```shell
sudo apt install libweston-10-dev meson ninja-build
```
2. Build and Install.
```
mkdir build && cd build
meson .. --prefix=/usr
ninja && sudo ninja install
```

3. Add `weston-shell-dpms.so` to modules key in weston config. Eg:
```
[core]
shell=kiosk-shell.so
modules=systemd-notify.so,weston-shell-dpms.so
```

## Controlling DPMS state.

This repo also generates a [client](src/weston-dpms.c) program that you can use to control the DPMS state.

DPMS state can be controlled by passing -m option with on, off as value.

To Turn on the monitor:

```shell
./build/weston-dpms -m on
```

To Turn off the monitor:

```shell
./build/weston-dpms -m off
```

Ensure that `WAYLAND_DISPLAY` env variable is set.