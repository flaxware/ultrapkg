#include "core/PackageVersion.h"

#include <algorithm>
#include <cctype>
#include <sstream>
#include <string>
#include <vector>

namespace ultrapkg {
namespace {

std::string toLower(std::string value) {
    std::transform(value.begin(), value.end(), value.begin(), [](unsigned char ch) {
        return static_cast<char>(std::tolower(ch));
    });
    return value;
}

bool isDigitChar(char ch) {
    return ch >= '0' && ch <= '9';
}

std::vector<std::string> splitSegments(const std::string& value) {
    std::vector<std::string> segments;
    std::string current;
    for (char ch : value) {
        if ((ch >= '0' && ch <= '9') || (ch >= 'a' && ch <= 'z') || (ch >= 'A' && ch <= 'Z')) {
            current.push_back(ch);
        } else {
            if (!current.empty()) {
                segments.push_back(current);
                current.clear();
            }
            if (ch == '~') {
                segments.push_back("~");
            }
        }
    }
    if (!current.empty()) {
        segments.push_back(current);
    }
    return segments;
}

int compareNumeric(const std::string& lhs, const std::string& rhs) {
    const long long left = std::stoll(lhs.empty() ? "0" : lhs);
    const long long right = std::stoll(rhs.empty() ? "0" : rhs);
    if (left < right) return -1;
    if (left > right) return 1;
    return 0;
}

} // namespace

PackageVersion::PackageVersion(std::string value) : value_(normalize(std::move(value))) {}

PackageVersion PackageVersion::parse(const std::string& value) {
    return PackageVersion(value);
}

bool PackageVersion::isValid() const noexcept {
    return !value_.empty();
}

const std::string& PackageVersion::raw() const noexcept {
    return value_;
}

std::string PackageVersion::normalize(std::string value) {
    if (value.empty()) {
        return value;
    }

    while (!value.empty() && (value.front() == 'v' || value.front() == 'V')) {
        value.erase(value.begin());
    }

    return value;
}

int PackageVersion::compareSegments(const std::string& lhs, const std::string& rhs) {
    if (lhs == rhs) {
        return 0;
    }

    const bool lhsNumeric = std::all_of(lhs.begin(), lhs.end(), [](unsigned char ch) {
        return std::isdigit(ch);
    });
    const bool rhsNumeric = std::all_of(rhs.begin(), rhs.end(), [](unsigned char ch) {
        return std::isdigit(ch);
    });

    if (lhsNumeric && rhsNumeric) {
        return compareNumeric(lhs, rhs);
    }

    if (lhsNumeric) {
        return -1;
    }
    if (rhsNumeric) {
        return 1;
    }

    const std::string lowerLhs = toLower(lhs);
    const std::string lowerRhs = toLower(rhs);
    const auto compareValue = lowerLhs.compare(lowerRhs);
    return compareValue < 0 ? -1 : 1;
}

int PackageVersion::compare(const PackageVersion& other) const noexcept {
    if (value_ == other.value_) {
        return 0;
    }

    std::vector<std::string> lhsParts = splitSegments(value_);
    std::vector<std::string> rhsParts = splitSegments(other.value_);

    const std::size_t common = std::min(lhsParts.size(), rhsParts.size());
    for (std::size_t i = 0; i < common; ++i) {
        if (lhsParts[i] == rhsParts[i]) {
            continue;
        }
        if (lhsParts[i] == "~") {
            return -1;
        }
        if (rhsParts[i] == "~") {
            return 1;
        }
        const int result = compareSegments(lhsParts[i], rhsParts[i]);
        if (result != 0) {
            return result;
        }
    }

    if (lhsParts.size() < rhsParts.size()) {
        return -1;
    }
    if (lhsParts.size() > rhsParts.size()) {
        return 1;
    }

    return 0;
}

bool operator==(const PackageVersion& lhs, const PackageVersion& rhs) {
    return lhs.compare(rhs) == 0;
}

bool operator!=(const PackageVersion& lhs, const PackageVersion& rhs) {
    return !(lhs == rhs);
}

bool operator<(const PackageVersion& lhs, const PackageVersion& rhs) {
    return lhs.compare(rhs) < 0;
}

bool operator>(const PackageVersion& lhs, const PackageVersion& rhs) {
    return lhs.compare(rhs) > 0;
}

bool operator<=(const PackageVersion& lhs, const PackageVersion& rhs) {
    return lhs.compare(rhs) <= 0;
}

bool operator>=(const PackageVersion& lhs, const PackageVersion& rhs) {
    return lhs.compare(rhs) >= 0;
}

} // namespace ultrapkg
