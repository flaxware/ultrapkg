#include "core/PackageInstaller.h"

#include "core/PackageInfo.h"

#include <cstdlib>
#include <filesystem>
#include <fstream>
#include <string>
#include <unistd.h>

namespace ultrapkg {
namespace {

std::filesystem::path canonicalizePath(const std::filesystem::path& value) {
    std::error_code ec;
    auto abs = std::filesystem::absolute(value, ec);
    if (ec) {
        return value;
    }

    auto normalized = std::filesystem::weakly_canonical(abs, ec);
    if (ec) {
        normalized = abs;
    }
    return normalized;
}

bool ensureWriteableDir(const std::filesystem::path& dir) {
    std::error_code ec;
    std::filesystem::create_directories(dir, ec);
    if (ec) {
        return false;
    }
    return std::filesystem::exists(dir) && std::filesystem::is_directory(dir);
}

std::filesystem::path defaultRoot() {
    const char* env_root = std::getenv("ULTRAPKG_ROOT");
    if (env_root != nullptr && env_root[0] != '\0') {
        return canonicalizePath(std::filesystem::path(env_root));
    }

    const char* home = std::getenv("HOME");
    if (home != nullptr && home[0] != '\0') {
        return canonicalizePath(std::filesystem::path(home) / ".local" / "share" / "ultrapkg");
    }

    return canonicalizePath(std::filesystem::path("/var/lib/ultrapkg"));
}

} // namespace

PackageInstaller::PackageInstaller(std::string root_prefix)
    : root_prefix_(std::move(root_prefix)) {}

std::filesystem::path PackageInstaller::defaultRootPath() {
    return defaultRoot();
}

std::filesystem::path PackageInstaller::canonicalize(const std::filesystem::path& path) {
    return canonicalizePath(path);
}

std::filesystem::path PackageInstaller::safeRootPath() const {
    const auto configured = root_prefix_.empty() ? defaultRoot() : canonicalizePath(root_prefix_);
    std::filesystem::path root = configured;
    if (root.empty()) {
        root = defaultRoot();
    }

    if (std::filesystem::exists(root) && !std::filesystem::is_directory(root)) {
        throw std::runtime_error("package root is not a directory: " + root.string());
    }

    if (!ensureWriteableDir(root)) {
        throw std::runtime_error("could not prepare package root: " + root.string());
    }

    return root;
}

bool PackageInstaller::isRoot() const {
    return ::getuid() == 0;
}

bool PackageInstaller::installPackage(const std::filesystem::path& deb_path, std::ostream& log) const {
    if (!std::filesystem::exists(deb_path) || !std::filesystem::is_regular_file(deb_path)) {
        log << "package file not found: " << deb_path << '\n';
        return false;
    }

    const auto info = PackageInfoReader::fromDeb(deb_path);
    if (!info.valid) {
        log << "could not read valid package metadata from: " << deb_path << '\n';
        return false;
    }

    const auto root = safeRootPath();
    const auto archives = root / "archives";
    const auto installed = root / "installed";
    if (!ensureWriteableDir(archives) || !ensureWriteableDir(installed)) {
        log << "could not prepare package database directories under: " << root << '\n';
        return false;
    }

    const auto destination = archives / deb_path.filename();
    std::error_code ec;
    std::filesystem::copy_file(deb_path, destination, std::filesystem::copy_options::overwrite_existing, ec);
    if (ec) {
        log << "failed to stage package file: " << ec.message() << '\n';
        return false;
    }

    const auto receipt = installed / (info.name + ".receipt");
    std::ofstream out(receipt, std::ios::trunc);
    if (!out) {
        log << "failed to write package receipt: " << receipt << '\n';
        return false;
    }

    out << "name=" << info.name << '\n';
    out << "version=" << info.version << '\n';
    out << "architecture=" << info.architecture << '\n';
    out << "maintainer=" << info.maintainer << '\n';
    out << "description=" << info.description << '\n';
    out << "source=" << deb_path.string() << '\n';
    out.flush();

    log << "installed " << info.name << " version " << info.version << " via lightweight local record\n";
    return true;
}

bool PackageInstaller::uninstallPackage(const std::string& package_name, std::ostream& log) const {
    if (package_name.empty()) {
        log << "package name is empty\n";
        return false;
    }

    const auto root = safeRootPath();
    const auto receipt = root / "installed" / (package_name + ".receipt");
    if (!std::filesystem::exists(receipt)) {
        log << "package not installed: " << package_name << '\n';
        return false;
    }

    std::error_code ec;
    std::filesystem::remove(receipt, ec);
    if (ec) {
        log << "failed to remove receipt: " << receipt << '\n';
        return false;
    }

    log << "removed " << package_name << " from local package database\n";
    return true;
}

} // namespace ultrapkg
