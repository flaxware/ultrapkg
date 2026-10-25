Source: ultrapkg
Section: utils
Priority: optional
Maintainer: flaxware <flaxware@users.noreply.github.com>
Build-Depends: debhelper (>= 12), cmake, g++, pkg-config
Standards-Version: 4.6.0
Rules-Requires-Root: no
Homepage: https://github.com/flaxware/ultrapkg

Package: ultrapkg
Architecture: any
Depends: ${misc:Depends}, coreutils
Description: Lightweight local package wrapper for flax.system
 Ultrapkg provides a lightweight apt/aptitude-style command set and direct .deb
 handling without depending on full external package manager execution for the
 common local workflow.

