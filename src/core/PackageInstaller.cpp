#include "core/PackageInstaller.h"

#include <cstdlib>
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

} // namespace

PackageInstaller::PackageInstaller(std::string root_prefix)
    : root_prefix_(std::move(root_prefix)) {}

bool PackageInstaller::isRoot() const {
    return ::getuid() == 0;
}

std::string PackageInstaller::buildInstallCommand(const std::filesystem::path& deb_path) const {
    std::string command = "dpkg -i ";
    if (!root_prefix_.empty()) {
        command += root_prefix_ + " ";
    }
    command += shellQuote(deb_path.string());
    return command;
}

std::string PackageInstaller::buildRemoveCommand(const std::string& package_name) const {
    std::string command = "dpkg -r ";
    if (!root_prefix_.empty()) {
        command += root_prefix_ + " ";
    }
    command += package_name;
    return command;
}

bool PackageInstaller::installPackage(const std::filesystem::path& deb_path, std::ostream& log) const {
    if (!std::filesystem::exists(deb_path)) {
        log << "package file not found: " << deb_path << '\n';
        return false;
    }

    const std::string command = isRoot()
        ? buildInstallCommand(deb_path)
        : "pkexec " + buildInstallCommand(deb_path);

    return runCommand(command, log);
}

bool PackageInstaller::uninstallPackage(const std::string& package_name, std::ostream& log) const {
    if (package_name.empty()) {
        log << "package name is empty\n";
        return false;
    }

    const std::string command = isRoot()
        ? buildRemoveCommand(package_name)
        : "pkexec " + buildRemoveCommand(package_name);

    return runCommand(command, log);
}

} // namespace ultrapkg
