Source: ultrapkg
Section: utils
Priority: optional
Maintainer: flaxware <flaxware@users.noreply.github.com>
Build-Depends: debhelper (>= 12), cmake, g++, pkg-config
Standards-Version: 4.6.0
Homepage: https://github.com/flaxware/ultrapkg
Rules-Requires-Root: no

Package: ultrapkg
Architecture: any
Depends: ${misc:Depends}, coreutils, imagemagick
Description: Lightweight package manager and .deb thumbnail helper for flax.system
 Ultrapkg is a clean, modular package utility for local .deb files and desktop
 integration. It follows the Flax system philosophy: simple, stable, and easy
 to maintain.

ultrapkg (0.1.0-1) unstable; urgency=medium

  * Initial v0.1 release with versioning, apt-compatible CLI, and a lightweight
    local package metadata database.

 -- flaxware <flaxware@users.noreply.github.com>  Sat, 25 Oct 2026 00:00:00 +0000
