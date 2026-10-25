#pragma once

#include <filesystem>
#include <iosfwd>
#include <string>

namespace ultrapkg {

class PackageManager {
public:
    bool install(const std::filesystem::path& deb_path, std::ostream& log);
    bool uninstall(const std::string& package_name, std::ostream& log);
    bool inspect(const std::filesystem::path& deb_path, std::ostream& log);
};

} // namespace ultrapkg
