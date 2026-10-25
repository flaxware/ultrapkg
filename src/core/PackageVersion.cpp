#pragma once

#include <string>

namespace ultrapkg {

class PackageVersion {
public:
    explicit PackageVersion(std::string value = {});
    static PackageVersion parse(const std::string& value);

    bool isValid() const noexcept;
    const std::string& raw() const noexcept;
    int compare(const PackageVersion& other) const noexcept;

private:
    std::string value_;
    static std::string normalize(std::string value);
    static int compareSegments(const std::string& left, const std::string& right);
};

bool operator==(const PackageVersion& left, const PackageVersion& right);
bool operator!=(const PackageVersion& left, const PackageVersion& right);
bool operator<(const PackageVersion& left, const PackageVersion& right);
bool operator>(const PackageVersion& left, const PackageVersion& right);
bool operator<=(const PackageVersion& left, const PackageVersion& right);
bool operator>=(const PackageVersion& left, const PackageVersion& right);

} // namespace ultrapkg
