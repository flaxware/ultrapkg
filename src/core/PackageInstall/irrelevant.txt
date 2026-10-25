#pragma once

#include <filesystem>
#include <iosfwd>
#include <string>

namespace ultrapkg {

class PackageInstaller {
public:
    explicit PackageInstaller(std::string root_prefix = "");

    bool installPackage(const std::filesystem::path& deb_path, std::ostream& log) const;
    bool uninstallPackage(const std::string& package_name, std::ostream& log) const;

private:
    std::string root_prefix_;
    bool isRoot() const;
    std::string buildInstallCommand(const std::filesystem::path& deb_path) const;
    std::string buildRemoveCommand(const std::string& package_name) const;
};

} // namespace ultrapkg
