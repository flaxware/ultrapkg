# Ultrapkg architecture

## 1. The goal

Ultrapkg is not a bloated app store. It is a disciplined package helper built around the Flax system idea: small, clear modules; stable behavior; clean integration; no noise.

This is the opposite of a giant single-file app where everything is mixed together and hidden in one place.

## 2. Responsibility split

### Metadata layer

`PackageInfo` reads Debian metadata. It is only responsible for:

- package name
- version
- architecture
- maintainer
- description
- validation

It does not install anything.

### Install layer

`PackageInstaller` is responsible for actual system actions:

- validating the package path
- building the install command
- running the install or removal action
- handling privilege escalation with `pkexec`

### Orchestration layer

`PackageManager` ties the pieces together and exposes a stable interface for:

- install
- uninstall
- inspection

### Thumbnail layer

`DebThumbnailer` creates thumbnail PNGs for `.deb` files so file managers such as Thunar can preview them.

## 3. Why this architecture matches Flax philosophy

Flax system favors:

- hierarchy
- modularity
- predictability
- minimal complexity
- user clarity

Ultrapkg follows that pattern. Each part has one job. That makes the app easier to debug, review, and maintain.

## 4. Visual style guidance

The app should feel modern and polished, but not heavy.

It should borrow the best parts of AppManager:

- clean icon-driven UI
- clear package cards
- readable metadata views
- easy drag-and-drop installation behavior

But it should be designed for XFCE/XFWM themes:

- native GTK/Xfce visual integration
- lighter resource usage
- fewer dependencies
- no unnecessary layered chrome
- better fit for a minimal desktop

## 5. XFCE-first integration

This project is intended for a distro where the desktop remains clean and efficient. That means:

- no heavy custom toolkit stack
- no large central app shell where everything is hidden
- no unnecessary background daemons for simple tasks
- a direct and honest package flow

## 6. Next steps

The next major steps are:

- package details viewer
- GUI front-end for local installs
- repository-aware update support
- Thunar integration and thumbnail cache handling
- security checks for package and archive integrity
