#pragma once

#include <filesystem>
#include <string>

namespace ultrapkg {

struct PackageInfo {
    std::string name;
    std::string version;
    std::string architecture;
    std::string maintainer;
    std::string description;
    bool valid = false;
};

class PackageInfoReader {
public:
    static PackageInfo fromDeb(const std::filesystem::path& deb_path);
};

} // namespace ultrapkg
