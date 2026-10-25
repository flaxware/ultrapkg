# FLAX OS 💎

> Ultrapkg - a lightweight, local-first package helper for the Flax system.

<br />
<div align="center">
  <h3 align="center">ULTRAPKG</h3>
  <p align="center">
    <a href="https://flaxware.github.io"><strong>Website</strong></a>
    &middot;
    <a href="https://github.com/flaxware/ultrapkg">Source code</a>
    &middot;
    <a href="https://github.com/flaxware/flax.system">Flax system</a>
  </p>
</div>

Ultrapkg is built for the Flax system philosophy: small, clear, modular, efficient, and secure.

It is not a giant package ecosystem. It is not trying to replace the entire Debian toolchain. It is a focused local package helper for the practical Linux workflows that matter most: install a `.deb`, inspect metadata, track local package state, and keep the system light enough to reason about.

Ultrapkg is for users who want direct control without the noise of large package-manager stacks.

## Versions

| Version | Status | Notes |
|---|---|---|
| 0.1.2 | Active development | AppImage support, archive-aware metadata, cleaner local package workflow |

## Mission

- Replace noisy package-manager flows with a clean local helper.
- Handle Debian and Ubuntu-style `.deb` files directly.
- Support AppImage bundles for portable desktop workflows.
- Keep runtime behavior lightweight and predictable.
- Make metadata inspection easy to understand and debug.
- Keep the system surface area small and the design security-first.

## Ideals

Our ideals are being:

- [General](#general)
- [Efficient](#efficient)
- [Perfect](#perfect)
- [Flexible](#flexible)
- [Portable](#portable)
- [Safe](#safe)

> Flax philosophy: small systems, clear structure, honest tools, and security before convenience.

## General

Ultrapkg is a focused package utility for local archive workflows. It understands the job it is meant to do and does not try to become a full app store, package marketplace, or all-in-one system shell.

This matches the Flax system idea of a high-quality single-purpose tool: small, readable, and modular.

### What it does

Ultrapkg gives you:

- direct `.deb` installation
- AppImage support for portable desktop apps
- local package receipts and lightweight state tracking
- apt-like and aptitude-like query flows
- package metadata inspection from the archive itself
- thumbnail generation for desktop file preview integration

## Efficient

### Lightweight design

Ultrapkg is intentionally small. It avoids heavy dependency stacks and keeps responsibilities narrow.

### Stability

The project favors predictable behavior over big abstractions. Small modules are easier to inspect, maintain, and debug.

### Resource awareness

Minimal runtime overhead, low complexity, and no unnecessary background daemons or package-manager bloat.

## Perfect

Ultrapkg is built for real-world use: predictable, reviewable, and easy to debug.

### Good defaults

- direct package operations
- clear output
- narrow responsibility scope
- small attack surface
- simple, explicit behavior

### Desktop fit

The project is designed for minimal Linux desktops where the system stays clean, efficient, and under user control.

## Flexible

### Plain text and simple structure

Flax system values plain-text configuration and straightforward tooling. Ultrapkg follows that rhythm by keeping package metadata readable and command-driven.

### Local-first workflow

Ultrapkg is built around direct local package interaction rather than giant online ecosystems.

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

The codebase is sized to stay readable and understandable without becoming a huge monolith.

## Safe

Security is part of the design from the start.

- validate archive inputs carefully
- keep privilege handling explicit
- avoid hidden system actions
- prefer clean, obvious behavior over automation tricks

Ultrapkg is not trying to hide risky behavior behind a giant package stack. It aims to expose minimal, direct operations clearly and safely.

## Why this project exists

Most package managers are designed for huge ecosystems. Ultrapkg is not trying to replace the full Debian universe.

It is trying to provide a clean local helper for workflows that are easy to trust and easy to reason about.

This makes it useful for:

- local `.deb` installation
- AppImage-based desktop application workflows
- package inspection without a giant toolchain
- minimal desktop environments
- package operations that value clarity over complexity

## Project goals

- Make local archive handling simple and safe.
- Keep the interface clear and command-oriented.
- Reduce dependence on heavyweight package-management stacks.
- Support desktop integration without turning the project into a bloated app store.
- Match the Flax system value of minimal, modular, maintainable software.

## Target users

- Linux developers working with `.deb` and AppImage packages
- users of minimal desktop environments
- system administrators and maintainers who want controlled local package execution
- people who prefer predictable tools over large package ecosystems

## Features

- install `.deb` packages directly
- install AppImage archives directly
- uninstall tracked packages
- inspect metadata from package archives
- search and query package records
- show local package information in a readable format
- generate preview thumbnails for package files
- work in minimal Linux environments without excessive dependencies

## Installation

### Requirements

You will usually need:

- a Linux system with Debian/Ubuntu-style packaging tools
- `gcc` or `clang`
- `cmake`
- `make` or `ninja`
- `dpkg` tools
- `imagemagick` for thumbnail generation

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

Install a Debian package:

```bash
ultrapkg install ./some-package.deb
```

Install an AppImage:

```bash
ultrapkg install ./some-app.AppImage
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

Generate a preview thumbnail:

```bash
ultrapkg thumb ./some-package.deb ./output.png 128
```

For an AppImage:

```bash
ultrapkg thumb ./some-app.AppImage ./app-preview.png 128
```

## Command reference

```bash
ultrapkg version
ultrapkg install <package.deb|package.AppImage>
ultrapkg appimage <package.AppImage>
ultrapkg dpkg <package.deb>
ultrapkg remove <package-name>
ultrapkg info <package.deb|package.AppImage>
ultrapkg list
ultrapkg search <query>
ultrapkg show <package-name>
ultrapkg apt install <package.deb|package.AppImage>
ultrapkg apt list
ultrapkg apt search <query>
ultrapkg apt show <package-name>
ultrapkg aptitude search <query>
ultrapkg aptitude depends <package-name>
ultrapkg thumb <package.deb|package.AppImage> <output.png> [size]
ultrapkg help
```

### Command behavior

- `install` and `dpkg`: install a local Debian package
- `appimage`: install or track an AppImage archive
- `remove`: uninstall a package by name
- `info`: show metadata for a package or AppImage
- `list`: show tracked package entries
- `search`: search local package metadata
- `show`: display a package record in a readable format
- `apt` and `aptitude`: support familiar apt-style query flows
- `thumb`: generate preview thumbnails for `.deb` or `.AppImage` files

## Why Ultrapkg is better for Flax-style workflows

Classic package tools are excellent in their domains. But Ultrapkg is designed for a different job: lightweight local control in a minimal Unix-like system.

### Compared with `apt`

`apt` is great for system package management and package repositories. It is robust, widely known, and deeply integrated into Debian-based systems.

But for a Flax workflow, `apt` is often too broad and too opinionated: it assumes a full repository model, a global package database, and a more complicated operational model.

Ultrapkg is better when you want:

- a local, direct workflow
- fewer moving parts
- more explicit package tracking
- less dependency on the full apt stack
- easier reasoning in a stripped-down or custom Unix environment

### Compared with `aptitude`

`aptitude` is powerful and very useful, but it is part of the bigger Debian package-management world. It is useful for package solving and dependency inspection, but it carries a lot of historical complexity and a traditional package-manager mindset.

Ultrapkg is better when you want a simpler, lighter pattern:

- no heavy package solver assumptions
- no giant repository abstraction forcing you into a full-system mindset
- a cleaner fit for local archive handling and minimal system design

### Compared with `dpkg`

`dpkg` is excellent as a low-level package tool, but it is intentionally minimal and rough around the edges for everyday use. It is not a user-friendly package workflow on its own.

Ultrapkg is better when you want:

- readable package metadata output
- lighter local package tracking
- better ergonomics for inspection and direct installs
- a clearer command interface for small, focused use cases

### Compared with `AppImage`

AppImage is a very strong portability model: one file, easy to carry, simple to run, and often very convenient for desktop apps.

But AppImage is not a package manager. It does not give you a clean package database, package history, install tracking, or a system-level package lifecycle in the same way a local package helper can.

Ultrapkg is better when you want:

- local tracking and metadata persistence
- a package-style mental model without the full Debian repo machinery
- better integration with minimal Unix workflows
- clear commands that fit the Flax system philosophy

### The real difference

The big idea is not “Ultrapkg replaces all packaging tools.”

The real idea is:

- `apt` is for full-system package infrastructure
- `dpkg` is for low-level package operations
- `aptitude` is for richer dependency-style workflows
- `AppImage` is for portable file-based app delivery
- Ultrapkg is for clean, local-first package handling in a small, modular Unix system

For Flax-style systems, that tradeoff matters.

Ultrapkg is optimized for clarity, control, and minimal operational weight.

## Architecture

Ultrapkg is intentionally split into simple responsibilities.

### Metadata layer

Reads archive metadata and exposes package information without performing heavyweight package actions.

### Install layer

Handles actual install and removal actions, validation, and local database tracking.

### Orchestration layer

Connects the package flows and exposes a stable command interface.

### Thumbnail layer

Generates PNG previews for archives so desktop environments can show them cleanly in file managers.

## Security notes

Ultrapkg is designed to be explicit and minimal. It should not perform hidden system changes or broad package automation without clear user intent. When installing or removing packages, privilege escalation should be controlled and obvious.

This matches the Flax principle: clear actions, clear trust boundaries, low surprise.

## Roadmap

Planned work includes:

- better package detail viewer
- GUI front-end for local installs
- repository-aware update support
- deeper file manager and desktop integration
- improved archive validation and integrity checks
- stronger AppImage metadata and launcher handling

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
