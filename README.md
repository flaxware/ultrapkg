# FLAX OS 💎

> Ultrapkg — a lightweight package manager for the Flax system.

Ultrapkg is built for the Flax system philosophy: simple, strong, efficient, modular, and secure. It is not trying to be a giant package ecosystem. It is trying to do the important things well: install local `.deb` files, inspect package metadata, keep a minimal package database, and provide the familiar apt/aptitude-style workflow without the usual weight.

## Mission

- Replace noisy package-manager flows with a clean local package helper.
- Handle Debian/Ubuntu-style `.deb` files directly.
- Keep the runtime lightweight and predictable.
- Make package metadata inspection and install actions easy to reason about.
- Follow security-first design and keep the surface area small.

## Flax system philosophy

Our ideals are:

- General: hierarchical and composable
- Efficient: lightweight and resource-aware
- Perfect: ready to ship and dependable
- Flexible: plain-text configuration where it matters
- Portable: modular, maintainable, and cleanly layered
- Safe: security before convenience

Ultrapkg follows that same rhythm: one clear job, stable behavior, and minimal complexity.

## What the project does

Ultrapkg is a compact package helper with:

- direct `.deb` installation via `ultrapkg dpkg <package.deb>`
- local package receipt tracking for lightweight state management
- apt-like commands for list/search/show/install/remove flows
- aptitude-like commands for search and dependency-style queries without depending on the full external toolchain
- metadata inspection of package control data directly from the archive
- thumbnail generation for package previews in desktop environments

## Command set

```bash
ultrapkg version
ultrapkg install <package.deb>
ultrapkg dpkg <package.deb>
ultrapkg remove <package-name>
ultrapkg info <package.deb>
ultrapkg list
ultrapkg search <query>
ultrapkg show <package-name>
ultrapkg apt install <package.deb>
ultrapkg apt list
ultrapkg apt search <query>
ultrapkg apt show <package-name>
ultrapkg aptitude search <query>
ultrapkg aptitude depends <package-name>
ultrapkg thumb <package.deb> <output.png> [size]
ultrapkg help
