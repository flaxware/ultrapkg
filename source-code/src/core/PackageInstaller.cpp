#include "core/PackageInstaller.h"
#include "core/PackageInfo.h"

#include <algorithm>
#include <array>
#include <cstdio>
#include <cstdlib>
#include <filesystem>
#include <fstream>
#include <sstream>
#include <string>
#include <unistd.h>

namespace ultrapkg {
namespace {

std::filesystem::path canonicalizePath(const std::filesystem::path& value) {
    std::error_code ec;
    const auto abs = std::filesystem::absolute(value, ec);
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

bool isSupportedArchiveFile(const std::filesystem::path& path) {
    if (!std::filesystem::exists(path) || !std::filesystem::is_regular_file(path)) {
        return false;
    }

    const auto ext = path.extension().string();
    return ext == ".deb" || ext == ".AppImage" || ext == ".appimage";
}

void markExecutableByDefault(const std::filesystem::path& path) {
    if (!isSupportedArchiveFile(path)) {
        return;
    }

    std::error_code ec;
    const auto status = std::filesystem::status(path, ec);
    if (ec) {
        return;
    }

    auto perms = status.permissions();
    perms |= std::filesystem::perms::owner_exec;
    perms |= std::filesystem::perms::group_exec;
    perms |= std::filesystem::perms::others_exec;
    std::filesystem::permissions(path, perms, std::filesystem::perm_options::replace, ec);
}

std::string shellQuote(const std::string& value) {
    std::string out;
    out.reserve(value.size() + 2);
    out.push_back('\'');
    for (char ch : value) {
        if (ch == '\'') {
            out += "'\\''";
        } else {
            out.push_back(ch);
        }
    }
    out.push_back('\'');
    return out;
}

std::string trim(std::string value) {
    while (!value.empty() && (value.front() == ' ' || value.front() == '\t' || value.front() == '\n' || value.front() == '\r')) {
        value.erase(value.begin());
    }
    while (!value.empty() && (value.back() == ' ' || value.back() == '\t' || value.back() == '\n' || value.back() == '\r')) {
        value.pop_back();
    }
    return value;
}

std::string execCapture(const std::string& command) {
    std::array<char, 4096> buffer{};
    std::string result;
    FILE* pipe = popen(command.c_str(), "r");
    if (!pipe) {
        return {};
    }

    while (fgets(buffer.data(), static_cast<int>(buffer.size()), pipe) != nullptr) {
        result += buffer.data();
    }

    const int status = pclose(pipe);
    (void)status;
    return result;
}

std::string readControlFromArchive(const std::filesystem::path& deb_path) {
    if (!std::filesystem::exists(deb_path) || !std::filesystem::is_regular_file(deb_path)) {
        return {};
    }

    const std::filesystem::path abs_path = std::filesystem::absolute(deb_path);
    const std::string archive_list = execCapture("ar t " + shellQuote(abs_path.string()) + " 2>/dev/null");
    if (archive_list.empty()) {
        return {};
    }

    std::istringstream stream(archive_list);
    std::string member;
    while (std::getline(stream, member)) {
        if (member.empty() || member.find("control.tar") == std::string::npos) {
            continue;
        }

        const std::string extract_cmd =
            "ar p " + shellQuote(abs_path.string()) + " " + shellQuote(member) +
            " | tar -xOf - ./control 2>/dev/null";
        const std::string control = execCapture(extract_cmd);
        if (!control.empty()) {
            return control;
        }
    }

    return {};
}

} // namespace

PackageInstaller::PackageInstaller(std::string root_prefix)
    : root_prefix_(std::move(root_prefix)) {}

std::filesystem::path PackageInstaller::defaultRootPath() {
    return defaultRoot();
}

std::filesystem::path PackageInstaller::canonicalize(const std::filesystem::path& path) const {
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

bool PackageInstaller::installPackage(const std::filesystem::path& archive_path, std::ostream& log) const {
    if (!isSupportedArchiveFile(archive_path)) {
        log << "package file not found or unsupported type: " << archive_path << '\n';
        return false;
    }

    const auto info = PackageInfoReader::fromDeb(archive_path);
    if (!info.valid) {
        log << "could not read valid package metadata from: " << archive_path << '\n';
        return false;
    }

    const auto root = safeRootPath();
    const auto archives = root / "archives";
    const auto installed = root / "installed";
    if (!ensureWriteableDir(archives) || !ensureWriteableDir(installed)) {
        log << "could not prepare package database directories under: " << root << '\n';
        return false;
    }

    const auto destination = archives / archive_path.filename();
    std::error_code ec;
    std::filesystem::copy_file(archive_path, destination, std::filesystem::copy_options::overwrite_existing, ec);
    if (ec) {
        log << "failed to stage package file: " << ec.message() << '\n';
        return false;
    }

    markExecutableByDefault(destination);

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
    out << "source=" << archive_path.string() << '\n';
    out << "type=" << ((archive_path.extension() == ".appimage" || archive_path.extension() == ".AppImage") ? "appimage" : "deb") << '\n';
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
