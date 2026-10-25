# FLAX OS💎
> Ultrapkg — the lightweight .deb manager for Flax system.

This project belongs to the Flax system philosophy:
- modular
- hierarchical
- clean
- fast
- secure
- compatible with XFCE and XFWM

Ultrapkg is built to replace the noisy, bloated installer flow with a small Linux package helper that is honest, reliable, and easy to understand.

It is designed for Debian/Ubuntu-style systems, with full support for local .deb installation and metadata inspection.

## Mission

- Replace the old `gdebi`-style path with a clearer installer flow.
- Work cleanly with local `.deb` packages.
- Keep the app lightweight for XFCE/XFWM desktops.
- Support `.deb` thumbnails in file managers such as Thunar.
- Stay modular and hierarchical, not monolithic.
- Follow GPL-3.0-or-later.

## Why this exists

The bigger app-manager pattern is useful, but it often becomes a giant monolith: too many responsibilities in one place, too much code to reason about, too much visual weight for simple system tasks.

Ultrapkg does the opposite.

It uses a small, layered architecture:
- metadata layer
- install layer
- manager layer
- thumbnail layer

This keeps the project easy to debug, test, and extend.

## Flax philosophy in one line

Small tools. Clear rules. Hierarchy. Security. No unnecessary noise.

## Project layout

```text
ultrapkg/
├── CMakeLists.txt
├── LICENSE
├── README.md
├── docs/
│   ├── architecture.md
│   └── philosophy.md
├── src/
│   ├── main.cpp
│   ├── core/
│   │   ├── PackageInfo.h
│   │   ├── PackageInfo.cpp
│   │   ├── PackageInstaller.h
│   │   ├── PackageInstaller.cpp
│   │   ├── PackageManager.h
│   │   └── PackageManager.cpp
│   └── thumbnail/
│       ├── DebThumbnailer.h
│       └── DebThumbnailer.cpp
├── debian/
│   ├── changelog
│   ├── control
│   ├── copyright
│   ├── rules
│   └── ultrapkg.install
└── .gitignore
```

## Quick start: do this in order

### 1) Install build dependencies

```bash
sudo apt update
sudo apt install -y build-essential cmake pkg-config dpkg imagemagick
```

### 2) Build

```bash
cmake -S . -B build
cmake --build build
```

### 3) Install to system

```bash
sudo install -m 0755 build/ultrapkg /usr/local/bin/ultrapkg
```

### 4) Test it

```bash
ultrapkg --help
ultrapkg info ./some-package.deb
ultrapkg install ./some-package.deb
ultrapkg thumb ./some-package.deb /tmp/package-thumb.png 128
```

## Commands

```bash
ultrapkg info <package.deb>
ultrapkg install <package.deb>
ultrapkg remove <package-name>
ultrapkg thumb <package.deb> <output.png> [size]
ultrapkg help
```

## Compatibility

### .deb
- package inspection
- installation from local .deb
- uninstall by package name
- Debian metadata parsing via `dpkg-deb`

### XFCE / XFWM
- light footprint
- native GTK/desktop friendliness
- no heavy app-store shell
- works with a minimal desktop environment

### Flax system fit
- modular
- predictable
- secure by default
- simple to maintain
- small and fast

## Architecture

### Metadata layer
Reads package metadata from `.deb` files.

### Install layer
Executes install and removal safely using `dpkg` and `pkexec` when needed.

### Manager layer
Coordinates the app logic.

### Thumbnail layer
Generates preview PNGs for `.deb` files so a file manager can show them properly.

## Thumbnailing

This project includes support for generating `.deb` previews for desktop file managers.

Example:

```bash
ultrapkg thumb ./example.deb /tmp/example.png 128
```

That is a good foundation for Thunar integration and wider desktop support.

## Debian packaging

```bash
dpkg-buildpackage -us -uc
```

## License

This project is GPL-3.0-or-later.

## Summary

Ultrapkg is intentionally small, honest, and built for the Flax system model.
It is not trying to be a giant app store.
It is trying to be the right tool for the job: fast, modular, and clean.
