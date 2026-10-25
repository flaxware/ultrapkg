#include "core/PackageVersion.h"

#include <algorithm>
#include <cctype>
#include <string>
#include <vector>

namespace ultrapkg {
namespace {

std::string lower(std::string value) {
    std::transform(value.begin(), value.end(), value.begin(), [](unsigned char ch) {
        return static_cast<char>(std::tolower(ch));
    });
    return value;
}

std::vector<std::string> splitTokens(const std::string& value) {
    std::vector<std::string> tokens;
    std::string current;

    for (char ch : value) {
        if (std::isalnum(static_cast<unsigned char>(ch)) != 0) {
            current.push_back(ch);
        } else {
            if (!current.empty()) {
                tokens.push_back(current);
                current.clear();
            }
            if (ch == '~') {
                tokens.push_back("~");
            }
        }
    }

    if (!current.empty()) {
        tokens.push_back(current);
    }

    return tokens;
}

long long toNumber(const std::string& value) {
    try {
        return std::stoll(value.empty() ? "0" : value);
    } catch (...) {
        return 0;
    }
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

    while (!value.empty() && (value.front() == 'v' || value.front() == 'V')) {
        value.erase(value.begin());
    }
    return value;
}

int PackageVersion::compareSegments(const std::string& left, const std::string& right) {
    if (left == right) {
        return 0;
    }

    const bool left_numeric = std::all_of(left.begin(), left.end(), [](unsigned char ch) {
        return std::isdigit(ch) != 0;
    });
    const bool right_numeric = std::all_of(right.begin(), right.end(), [](unsigned char ch) {
        return std::isdigit(ch) != 0;
    });

    if (left_numeric && right_numeric) {
        const auto left_num = toNumber(left);
        const auto right_num = toNumber(right);
        if (left_num < right_num) return -1;
        if (left_num > right_num) return 1;
        return 0;
    }

    if (left_numeric) return -1;
    if (right_numeric) return 1;

    const auto lhs = lower(left);
    const auto rhs = lower(right);
    if (lhs < rhs) return -1;
    if (lhs > rhs) return 1;
    return 0;
}

int PackageVersion::compare(const PackageVersion& other) const noexcept {
    if (value_ == other.value_) {
        return 0;
    }

    const auto left_tokens = splitTokens(value_);
    const auto right_tokens = splitTokens(other.value_);
    const auto common = std::min(left_tokens.size(), right_tokens.size());

    for (std::size_t i = 0; i < common; ++i) {
        if (left_tokens[i] == right_tokens[i]) {
            continue;
        }

        if (left_tokens[i] == "~") {
            return -1;
        }
        if (right_tokens[i] == "~") {
            return 1;
        }

        const auto result = compareSegments(left_tokens[i], right_tokens[i]);
        if (result != 0) {
            return result;
        }
    }

    if (left_tokens.size() < right_tokens.size()) {
        return -1;
    }
    if (left_tokens.size() > right_tokens.size()) {
        return 1;
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
