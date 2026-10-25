#pragma once

#include "core/PackageInfo.h"

#include <filesystem>
#include <iosfwd>
#include <string>

namespace ultrapkg {

class PackageInstaller {
public:
    explicit PackageInstaller(std::string root_prefix = {});

    static std::filesystem::path defaultRootPath();
    std::filesystem::path canonicalize(const std::filesystem::path& path) const;
    std::filesystem::path safeRootPath() const;
    bool isRoot() const;
    bool installPackage(const std::filesystem::path& archive_path, std::ostream& log) const;
    bool uninstallPackage(const std::string& package_name, std::ostream& log) const;

private:
    std::string root_prefix_;
};

} // namespace ultrapkg
