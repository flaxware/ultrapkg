# FLAX OS💎
> Ultrapkg — the lightweight .deb manager for Flax system.

Ultrapkg is built to replace heavy package-manager flows with a compact, reliable local package helper.
It keeps Debian/Ubuntu-compatible `.deb` metadata handling but avoids the cost and complexity of `dpkg`, `apt`, and `aptitude` for common local workflows.

## Current version

This project starts at v0.1.0.

## Features

- `.deb` metadata inspection with direct archive parsing
- apt-like local install/remove/list/search/show commands
- aptitude-style search/depends commands without invoking `aptitude`
- lightweight package receipts stored in a local package database
- fast version comparison for package upgrade checks
- `.deb` thumbnail preview generation

## Commands

```bash
ultrapkg version
ultrapkg install ./package.deb
ultrapkg remove pkgname
ultrapkg info ./package.deb
ultrapkg search hello
ultrapkg show pkgname
ultrapkg list
ultrapkg apt install ./package.deb
ultrapkg apt search hello
ultrapkg apt show pkgname
ultrapkg aptitude search hello
ultrapkg aptitude depends pkgname
ultrapkg thumb ./package.deb /tmp/thumb.png 128
```

## Philosophy

Ultrapkg is intentionally small, transparent, modular, and fast. It emphasizes:

- clear package metadata processing
- explicit version ordering
- local package state without shelling out to heavyweight package managers
- predictable behavior over hidden magic

## Build

```bash
cmake -S . -B build
cmake --build build
ctest --test-dir build --output-on-failure
```

## License

GPL-3.0-or-later.
