#include "core/PackageManager.h"

#include "core/PackageInfo.h"
#include "core/PackageInstaller.h"
#include "core/PackageVersion.h"

#include <algorithm>
#include <cctype>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

namespace ultrapkg {
namespace {

std::filesystem::path defaultRootPath() {
    return PackageInstaller::defaultRootPath();
}

std::vector<PackageInfo> readInstalledReceipts() {
    const auto root = defaultRootPath();
    const auto installed_dir = root / "installed";
    std::vector<PackageInfo> packages;

    if (!std::filesystem::exists(installed_dir) || !std::filesystem::is_directory(installed_dir)) {
        return packages;
    }

    for (const auto& entry : std::filesystem::directory_iterator(installed_dir)) {
        if (!entry.is_regular_file() || entry.path().extension() != ".receipt") {
            continue;
        }

        std::ifstream input(entry.path());
        PackageInfo info{};
        std::string line;
        while (std::getline(input, line)) {
            const auto pos = line.find('=');
            if (pos == std::string::npos) {
                continue;
            }
            const std::string key = line.substr(0, pos);
            const std::string value = line.substr(pos + 1);
            if (key == "name") {
                info.name = value;
            } else if (key == "version") {
                info.version = value;
            } else if (key == "architecture") {
                info.architecture = value;
            } else if (key == "maintainer") {
                info.maintainer = value;
            } else if (key == "description") {
                info.description = value;
            }
        }

        if (!info.name.empty() && !info.version.empty()) {
            info.valid = true;
            packages.push_back(info);
        }
    }

    std::sort(packages.begin(), packages.end(), [](const PackageInfo& a, const PackageInfo& b) {
        return a.name < b.name;
    });
    return packages;
}

std::string lower(std::string value) {
    std::transform(value.begin(), value.end(), value.begin(), [](unsigned char c) {
        return static_cast<char>(std::tolower(c));
    });
    return value;
}

bool matchesQuery(const PackageInfo& info, const std::string& query) {
    if (query.empty()) {
        return true;
    }
    const auto q = lower(query);
    return lower(info.name).find(q) != std::string::npos ||
           lower(info.version).find(q) != std::string::npos ||
           lower(info.description).find(q) != std::string::npos;
}

} // namespace

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

bool PackageManager::aptInstall(const std::filesystem::path& deb_path, std::ostream& log) {
    return install(deb_path, log);
}

bool PackageManager::aptRemove(const std::string& package_name, std::ostream& log) {
    return uninstall(package_name, log);
}

bool PackageManager::aptList(std::ostream& log) const {
    const auto packages = readInstalledReceipts();
    if (packages.empty()) {
        log << "no packages installed\n";
        return true;
    }

    for (const auto& package : packages) {
        log << package.name << " " << package.version << '\n';
    }
    return true;
}

bool PackageManager::aptSearch(const std::string& query, std::ostream& log) const {
    const auto packages = readInstalledReceipts();
    bool found = false;
    for (const auto& package : packages) {
        if (matchesQuery(package, query)) {
            log << package.name << " " << package.version << " - " << package.description << '\n';
            found = true;
        }
    }
    if (!found) {
        log << "no packages match: " << query << '\n';
    }
    return found;
}

bool PackageManager::aptShow(const std::string& package_name, std::ostream& log) const {
    const auto packages = readInstalledReceipts();
    for (const auto& package : packages) {
        if (package.name == package_name) {
            log << "Package: " << package.name << '\n';
            log << "Version: " << package.version << '\n';
            log << "Architecture: " << package.architecture << '\n';
            log << "Maintainer: " << package.maintainer << '\n';
            log << "Description: " << package.description << '\n';
            return true;
        }
    }

    log << "package not found: " << package_name << '\n';
    return false;
}

bool PackageManager::aptitudeSearch(const std::string& query, std::ostream& log) const {
    return aptSearch(query, log);
}

bool PackageManager::aptitudeDepends(const std::string& package_name, std::ostream& log) const {
    const auto packages = readInstalledReceipts();
    for (const auto& package : packages) {
        if (package.name == package_name) {
            log << package.name << " depends on: none (ultrapkg lightweight metadata mode)\n";
            return true;
        }
    }

    log << "package not found: " << package_name << '\n';
    return false;
}

bool PackageManager::aptitudeInstall(const std::filesystem::path& deb_path, std::ostream& log) {
    return install(deb_path, log);
}

bool PackageManager::upgrade(const std::string& package_name, const std::filesystem::path& deb_path, std::ostream& log) {
    const auto incoming = PackageInfoReader::fromDeb(deb_path);
    if (!incoming.valid) {
        log << "could not read valid package metadata from: " << deb_path << '\n';
        return false;
    }

    const auto installed = readInstalledReceipts();
    auto it = std::find_if(installed.begin(), installed.end(), [&](const PackageInfo& p) {
        return p.name == package_name || p.name == incoming.name;
    });

    if (it != installed.end()) {
        const PackageVersion current(it->version);
        const PackageVersion next(incoming.version);
        if (next <= current) {
            log << "package " << package_name << " is already at or newer than " << incoming.version << '\n';
            return true;
        }
    }

    return install(deb_path, log);
}

std::vector<PackageInfo> PackageManager::installedPackages() const {
    return readInstalledReceipts();
}

} // namespace ultrapkg
