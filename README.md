# Ultrapkg

Ultrapkg is a lightweight, modular package installer and `.deb` helper for the Flax system philosophy: simple, stable, secure, predictable, and easy to reason about.

It is designed to be a better fit than monolithic AppImage installers for XFCE and XFWM-based desktops. It keeps the visual clarity of a modern app-manager, but stays compatible with XFCE themes and lightweight desktop behavior.

## Mission

- Replace the `gdebi`-style workflow with a cleaner, explicit package install path.
- Support local `.deb` installation and metadata inspection.
- Add `.deb` thumbnail generation for file managers like Thunar.
- Keep the app small, maintainable, and GPL-licensed.
- Fit the Flax philosophy: modular, hierarchical, secure, and free of unnecessary complexity.

## Design goals

- Lightweight enough for XFCE and XFWM.
- Minimal system overhead.
- Strong separation between metadata, installation, orchestration, and thumbnailing.
- Clear C++ implementation that is easier to review and maintain than a giant single-file GUI application.

## Build

```bash
cmake -S . -B build
cmake --build build
sudo install -m 0755 build/ultrapkg /usr/local/bin/ultrapkg
```

## Usage

```bash
ultrapkg info ./package.deb
ultrapkg install ./package.deb
ultrapkg remove package-name
ultrapkg thumb ./package.deb /tmp/package.png 128
```

## License

This project is licensed under the GNU General Public License v3.0 or later.

## Philosophy alignment

This project follows the Flax system direction:

- modular hierarchy
- clean separation of responsibility
- minimalism over bloat
- security by default
- careful desktop integration
- strong open-source licensing
