#include "core/PackageVersion.h"

#include <algorithm>
#include <cctype>
#include <string>
#include <vector>

namespace ultrapkg {
namespace {

std::string lower(std::string value) {
    std::transform(value.begin(), value.end(), value.begin(), [](unsigned char c) {
        return static_cast<char>(std::tolower(c));
    });
    return value;
}

std::vector<std::string> splitVersionParts(const std::string& value) {
    std::vector<std::string> parts;
    std::string current;
    for (char ch : value) {
        if (std::isalnum(static_cast<unsigned char>(ch))) {
            current.push_back(ch);
        } else {
            if (!current.empty()) {
                parts.push_back(current);
                current.clear();
            }
        }
    }
    if (!current.empty()) {
        parts.push_back(current);
    }
    return parts;
}

} // namespace

PackageVersion::PackageVersion(std::string value)
    : value_(normalize(std::move(value))) {}

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

    std::string out;
    out.reserve(value.size());
    for (char ch : value) {
        if (ch == '~' || ch == '-' || ch == '+' || ch == ':') {
            if (!out.empty() && out.back() != '.') {
                out.push_back('.');
            }
        } else if (std::isalnum(static_cast<unsigned char>(ch))) {
            out.push_back(static_cast<char>(std::tolower(static_cast<unsigned char>(ch))));
        }
    }

    while (!out.empty() && out.back() == '.') {
        out.pop_back();
    }
    return out;
}

int PackageVersion::compareSegments(const std::string& left, const std::string& right) {
    const bool left_is_num = std::all_of(left.begin(), left.end(), [](unsigned char ch) {
        return std::isdigit(ch) != 0;
    });
    const bool right_is_num = std::all_of(right.begin(), right.end(), [](unsigned char ch) {
        return std::isdigit(ch) != 0;
    });

    if (left_is_num && right_is_num) {
        const auto lhs = std::stoll(left);
        const auto rhs = std::stoll(right);
        if (lhs < rhs) return -1;
        if (lhs > rhs) return 1;
        return 0;
    }

    if (left_is_num != right_is_num) {
        return left_is_num ? 1 : -1;
    }

    const auto cmp = lower(left).compare(lower(right));
    if (cmp < 0) return -1;
    if (cmp > 0) return 1;
    return 0;
}

int PackageVersion::compare(const PackageVersion& other) const noexcept {
    const auto left = splitVersionParts(value_);
    const auto right = splitVersionParts(other.value_);
    const std::size_t count = std::max(left.size(), right.size());

    for (std::size_t i = 0; i < count; ++i) {
        const std::string l = (i < left.size()) ? left[i] : "0";
        const std::string r = (i < right.size()) ? right[i] : "0";
        const int cmp = compareSegments(l, r);
        if (cmp != 0) {
            return cmp;
        }
    }

    return 0;
}

bool operator==(const PackageVersion& left, const PackageVersion& right) {
    return left.compare(right) == 0;
}

bool operator!=(const PackageVersion& left, const PackageVersion& right) {
    return !(left == right);
}

bool operator<(const PackageVersion& left, const PackageVersion& right) {
    return left.compare(right) < 0;
}

bool operator>(const PackageVersion& left, const PackageVersion& right) {
    return left.compare(right) > 0;
}

bool operator<=(const PackageVersion& left, const PackageVersion& right) {
    return left.compare(right) <= 0;
}

bool operator>=(const PackageVersion& left, const PackageVersion& right) {
    return left.compare(right) >= 0;
}

} // namespace ultrapkg
