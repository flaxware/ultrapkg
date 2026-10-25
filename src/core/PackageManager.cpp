#include "core/PackageInfo.h"
#include "core/PackageInstaller.h"
#include "core/PackageManager.h"

#include <filesystem>
#include <fstream>
#include <iostream>

namespace ultrapkg {

bool PackageManager::install(const std::filesystem::path& deb_path, std::ostream& log) {
    PackageInstaller installer;
    return installer.installPackage(deb_path, log);
}

bool PackageManager::uninstall(const std::string& package_name, std::ostream& log) {
    PackageInstaller installer;
    return installer.uninstallPackage(package_name, log);
}

bool PackageManager::inspect(const std::filesystem::path& deb_path, std::ostream& log) {
    const auto info = PackageInfoReader::fromDeb(deb_path);
    if (!info.valid) {
        log << "could not read valid package metadata from: " << deb_path << '\n';
        return false;
    }

    log << "package: " << info.name << '\n';
    log << "version: " << info.version << '\n';
    log << "architecture: " << info.architecture << '\n';
    log << "maintainer: " << info.maintainer << '\n';
    log << "description: " << info.description << '\n';
    return true;
}

} // namespace ultrapkg
