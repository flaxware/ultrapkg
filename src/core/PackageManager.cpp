#pragma once

#include <filesystem>
#include <iosfwd>
#include <string>
#include <vector>

namespace ultrapkg {

struct PackageInfo;

class PackageManager {
public:
    bool install(const std::filesystem::path& deb_path, std::ostream& log);
    bool uninstall(const std::string& package_name, std::ostream& log);
    bool inspect(const std::filesystem::path& deb_path, std::ostream& log);

    bool aptInstall(const std::filesystem::path& deb_path, std::ostream& log);
    bool aptRemove(const std::string& package_name, std::ostream& log);
    bool aptList(std::ostream& log) const;
    bool aptSearch(const std::string& query, std::ostream& log) const;
    bool aptShow(const std::string& package_name, std::ostream& log) const;
    bool aptitudeSearch(const std::string& query, std::ostream& log) const;
    bool aptitudeDepends(const std::string& package_name, std::ostream& log) const;
    bool aptitudeInstall(const std::filesystem::path& deb_path, std::ostream& log);
    bool upgrade(const std::string& package_name, const std::filesystem::path& deb_path, std::ostream& log);
    std::vector<PackageInfo> installedPackages() const;
};

} // namespace ultrapkg
