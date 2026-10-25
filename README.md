# FLAX OS 💎

> Ultrapkg — a lightweight package manager for the Flax system.

<!-- PROJECT HEADER -->
<br />
<div align="center">
  <h3 align="center">ULTRAPKG</h3>
  <p align="center">
    <a href="https://flaxware.github.io"><strong>Flax website</strong></a>
    &middot;
    <a href="https://github.com/flaxware/ultrapkg">Source code</a>
    &middot;
    <a href="https://github.com/flaxware/flax.system">Flax system</a>
  </p>
</div>

Ultrapkg is a minimal package helper built for the Flax philosophy: small, predictable, secure, and easy to reason about. It is not a giant package ecosystem. It is a clean local tool for managing Debian-style `.deb` packages without dragging in the full weight of a desktop package manager.

The goal is simple: make package inspection, install, and removal flows direct, readable, and dependable.

## Versions

| Version | Status | Notes |
|---|---|---|
| 0.1.0 | Early development | Core install, metadata inspection, and thumbnail generation |

## Mission

- Replace noisy package-manager flows with a clean local helper.
- Handle Debian and Ubuntu-style `.deb` files directly.
- Keep runtime behavior lightweight and predictable.
- Make metadata inspection easy to understand and debug.
- Keep the attack surface small and the design security-first.

## Ideals

Our ideals are being:

- [General](#general)
- [Efficient](#efficient)
- [Perfect](#perfect)
- [Flexible](#flexible)
- [Portable](#portable)
- [Safe](#safe)

> Flax philosophy: small systems, clear structure, honest tools, security before convenience.

## General

Ultrapkg is a focused package utility for local .deb workflows. It understands the job it is meant to do and does not try to become a full app store or an all-in-one package ecosystem.

This fits the Flax system idea of a high-quality, single-purpose tool that stays small, readable, and modular.

### What it does

Ultrapkg gives you:

- direct `.deb` installation
- local package receipts and lightweight state tracking
- apt-like and aptitude-like query flows
- package metadata inspection from the archive itself
- thumbnail generation for desktop file previews

## Efficient

### Lightweight design

Ultrapkg is intentionally small. It avoids the complexity of large package managers and keeps its responsibilities narrow.

### Stability

The project favors predictable behavior over broad abstraction. Each layer has a specific job and a narrow interface.

### Resource awareness

Minimal runtime overhead, minimal background complexity, and no unnecessary daemons or layered systems.

## Perfect

Ultrapkg is built to be ready to use in the real world: predictable, reviewable, and easy to debug.

### Good defaults

- direct package operations
- clean output
- small surface area
- simple behavior

### Desktop fit

The project is designed to fit a minimal Linux desktop where the system stays clean, efficient, and under user control.

## Flexible

### Plain text and minimal structure

Flax system values plain-text configuration and simple toolchains. Ultrapkg follows that rhythm by relying on conventional package metadata and clear command-driven workflows.

### Local-first workflow

Ultrapkg is built around local package interaction rather than heavyweight dependency resolution and giant online package landscapes.

## Portable

Ultrapkg is modular by design.

### Modularity

The project is split into layers such as:

- metadata inspection
- package installation
- lifecycle orchestration
- thumbnail generation

This keeps each feature isolated and easier to maintain.

### Maintainability

The codebase is designed to stay readable and small enough that contributors can reason about it without digging through a huge monolith.

## Safe

Security is part of the project design from the start.

- keep privilege handling narrow and explicit
- validate package inputs
- avoid unnecessary automation
- prefer direct, auditable actions over hidden magic

Ultrapkg is not trying to hide powerful behavior behind a massive system. It is trying to expose the correct minimal operation clearly and safely.

## Why this project exists

Most package managers are designed to manage huge ecosystems. Ultrapkg is not trying to replace the full Debian ecosystem. It is trying to provide a clean helper for local, direct workflows that are easy to trust.

This makes it useful for:

- local `.deb` installation
- package inspection without a giant toolchain
- small desktop environments
- system-level workflows that value clarity over complexity

## Project goals

- Make local `.deb` handling simple and safe.
- Keep the interface clear and command-oriented.
- Reduce dependency on heavy package-management stacks.
- Support desktop integration without turning the project into a bloated app store.
- Match the Flax system value of minimal, modular, maintainable software.

## Target users

- Linux developers working with `.deb` packages
- users of minimal desktop environments
- sysadmins and maintainers who want controlled local package execution
- people who prefer predictable tools over large package ecosystems

## Features

- install `.deb` packages directly
- uninstall installed packages
- inspect package metadata
- search and query package records
- show local package data in a simple format
- generate preview thumbnails for `.deb` files
- work in a minimal Linux environment without excessive dependencies

## Installation

### Requirements

You will typically need:

- a Linux system with Debian/Ubuntu-style packaging tools
- `gcc` or `clang`
- `cmake`
- `make` or `ninja`
- `dpkg` tools
- `imagemagick` if thumbnail generation is used

### Build from source

```bash
git clone https://github.com/flaxware/ultrapkg.git
cd ultrapkg
cmake -S . -B build
cmake --build build
```

### Install locally

```bash
sudo install build/ultrapkg /usr/local/bin/ultrapkg
```

### Verify installation

```bash
ultrapkg version
ultrapkg help
```

## Quick start

Install a package:

```bash
ultrapkg install ./some-package.deb
```

Inspect package metadata:

```bash
ultrapkg info ./some-package.deb
```

List installed or managed packages:

```bash
ultrapkg list
```

Search package information:

```bash
ultrapkg search firefox
```

Remove a package:

```bash
ultrapkg remove package-name
```

Generate a preview thumbnail from a `.deb` file:

```bash
ultrapkg thumb ./some-package.deb ./output.png 128
```

## Command reference

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
```

### Command behavior

- `install` and `dpkg`: install a local Debian package
- `remove`: uninstall a package by name
- `info`: show metadata for a package file
- `list`: show known package entries
- `search`: search local or package metadata
- `show`: display package details
- `apt` and `aptitude`: emulate familiar apt-style query flows
- `thumb`: generate a preview image for `.deb` packages

## Architecture

Ultrapkg is intentionally split into simple responsibilities:

### Metadata layer

Reads Debian metadata and exposes package information without installing anything.

### Install layer

Handles actual install and removal actions, validation, and privilege escalation.

### Orchestration layer

Connects the flows and exposes a stable interface for package operations.

### Thumbnail layer

Generates PNG previews for `.deb` files so desktop environments can show them nicely in file managers such as Thunar.

## Security notes

Ultrapkg is designed to be explicit and minimal. It should not perform hidden system changes or broad package automation without user intent. When installing or removing packages, privilege escalation should be controlled and obvious.

This fits the Flax principle: clear actions, clear trust boundaries, low surprise.

## Roadmap

Planned work includes:

- better package details viewer
- GUI front-end for local installs
- repository-aware update support
- deeper file manager integration
- improved package validation and archive integrity checks

## Contributing

Contributions are welcome.

If you want to help:

1. Fork the repository.
2. Create a feature branch.
3. Make small, focused changes.
4. Validate behavior with the project tooling.
5. Open a clean pull request with a clear summary.

Please keep changes aligned with the Flax philosophy: simple, secure, reviewable, and minimal.

## Philosophy

Flax system is a Unix-first, modular, security-conscious design philosophy. It aims to make complex systems understandable again by keeping responsibilities small and configuration transparent.

Ultrapkg fits that idea by staying honest:

- one job at a time
- clear CLI behavior
- minimal complexity
- strong defaults
- no unnecessary ceremony

## License

See the repository license file for usage and distribution terms.

## Contact

- Website: https://flaxware.github.io
- Repository: https://github.com/flaxware/ultrapkg
- Flax system: https://github.com/flaxware/flax.system
