#include "core/PackageInstaller.h"

#include "core/PackageInfo.h"

#include <filesystem>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <unistd.h>

namespace ultrapkg {

namespace {

std::string shellQuote(const std::string& value) {
    std::string out;
    out.reserve(value.size() + 2);
    out.push_back('\'');
    for (char c : value) {
        if (c == '\'') {
            out += "'\\''";
        } else {
            out.push_back(c);
        }
    }
    out.push_back('\'');
    return out;
}

bool runCommand(const std::string& command, std::ostream& log) {
    log << "running: " << command << '\n';
    const int status = std::system(command.c_str());
    return status == 0;
}

std::filesystem::path defaultRootPath(const std::string& root_prefix) {
    if (!root_prefix.empty()) {
        return std::filesystem::path(root_prefix);
    }

    if (::getuid() == 0) {
        return std::filesystem::path("/var/lib/ultrapkg");
    }

    const char* home = std::getenv("HOME");
    if (home != nullptr && home[0] != '\0') {
        return std::filesystem::path(home) / ".local" / "share" / "ultrapkg";
    }

    return std::filesystem::path(".") / ".ultrapkg";
}

void writeReceipt(const std::filesystem::path& db_dir, const PackageInfo& info, const std::filesystem::path& source) {
    const auto receipt_dir = db_dir / "installed";
    std::filesystem::create_directories(receipt_dir);

    const auto receipt_path = receipt_dir / (info.name + ".receipt");
    std::ofstream out(receipt_path, std::ios::trunc);
    if (!out) {
        return;
    }

    out << "name=" << info.name << '\n';
    out << "version=" << info.version << '\n';
    out << "architecture=" << info.architecture << '\n';
    out << "maintainer=" << info.maintainer << '\n';
    out << "description=" << info.description << '\n';
    out << "source=" << source.string() << '\n';
}

std::filesystem::path installedDbPath(const std::string& root_prefix) {
    return defaultRootPath(root_prefix) / "installed";
}

} // namespace

PackageInstaller::PackageInstaller(std::string root_prefix)
    : root_prefix_(std::move(root_prefix)) {}

bool PackageInstaller::isRoot() const {
    return ::getuid() == 0;
}

std::string PackageInstaller::buildInstallCommand(const std::filesystem::path& deb_path) const {
    const std::string root = defaultRootPath(root_prefix_).string();
    std::string command = "mkdir -p " + shellQuote(root + "/archives") + " && mkdir -p " + shellQuote(root + "/installed") + " && cp " + shellQuote(deb_path.string()) + " " + shellQuote(root + "/archives/" + deb_path.filename().string());
    return command;
}

std::string PackageInstaller::buildRemoveCommand(const std::string& package_name) const {
    const std::string root = defaultRootPath(root_prefix_).string();
    std::string command = "rm -f " + shellQuote(root + "/installed/" + package_name + ".receipt");
    return command;
}

bool PackageInstaller::installPackage(const std::filesystem::path& deb_path, std::ostream& log) const {
    if (!std::filesystem::exists(deb_path)) {
        log << "package file not found: " << deb_path << '\n';
        return false;
    }

    const auto info = PackageInfoReader::fromDeb(deb_path);
    if (!info.valid) {
        log << "could not read valid package metadata from: " << deb_path << '\n';
        return false;
    }

    const std::filesystem::path root = defaultRootPath(root_prefix_);
    const std::filesystem::path archive_dir = root / "archives";
    std::filesystem::create_directories(archive_dir);
    std::filesystem::copy_file(
        deb_path,
        archive_dir / deb_path.filename(),
        std::filesystem::copy_options::overwrite_existing);

    writeReceipt(root, info, deb_path);
    log << "installed " << info.name << " version " << info.version << " via lightweight local record\n";
    return true;
}

bool PackageInstaller::uninstallPackage(const std::string& package_name, std::ostream& log) const {
    if (package_name.empty()) {
        log << "package name is empty\n";
        return false;
    }

    const std::filesystem::path db_path = installedDbPath(root_prefix_);
    const std::filesystem::path receipt = db_path / (package_name + ".receipt");
    if (!std::filesystem::exists(receipt)) {
        log << "package not installed: " << package_name << '\n';
        return false;
    }

    std::filesystem::remove(receipt);
    log << "removed " << package_name << " from local package database\n";
    return true;
}

} // namespace ultrapkg
