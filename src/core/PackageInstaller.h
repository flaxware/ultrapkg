#include "core/PackageInfo.h"

#include <array>
#include <cstdio>
#include <cstdlib>
#include <filesystem>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>

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

std::string execCapture(const std::string& command) {
    std::array<char, 4096> buffer{};
    std::string result;
    FILE* pipe = popen(command.c_str(), "r");
    if (!pipe) {
        return {};
    }

    while (fgets(buffer.data(), static_cast<int>(buffer.size()), pipe) != nullptr) {
        result += buffer.data();
    }

    const int status = pclose(pipe);
    (void)status;
    return result;
}

std::string trim(std::string value) {
    while (!value.empty() && (value.front() == ' ' || value.front() == '\t' || value.front() == '\r' || value.front() == '\n')) {
        value.erase(value.begin());
    }
    while (!value.empty() && (value.back() == ' ' || value.back() == '\t' || value.back() == '\r' || value.back() == '\n')) {
        value.pop_back();
    }
    return value;
}

std::string extractControlData(const std::filesystem::path& deb_path) {
    const std::string arListCmd = "ar t " + shellQuote(deb_path.string()) + " 2>/dev/null";
    const std::string arList = execCapture(arListCmd);
    if (arList.empty()) {
        return {};
    }

    std::vector<std::string> members;
    std::stringstream stream(arList);
    std::string member;
    while (std::getline(stream, member)) {
        if (!member.empty()) {
            members.push_back(member);
        }
    }

    for (const auto& candidate : members) {
        if (candidate.find("control.tar") != std::string::npos) {
            const std::string extractCmd = "ar p " + shellQuote(deb_path.string()) + " " + shellQuote(candidate) + " | tar -xOf - ./control 2>/dev/null";
            const std::string control = execCapture(extractCmd);
            if (!control.empty()) {
                return control;
            }
        }
    }

    return {};
}

} // namespace

PackageInfo PackageInfoReader::fromDeb(const std::filesystem::path& deb_path) {
    PackageInfo info{};

    if (!std::filesystem::exists(deb_path)) {
        return info;
    }

    const std::string control = extractControlData(deb_path);
    if (control.empty()) {
        return info;
    }

    std::string current_field;
    std::string current_value;
    std::stringstream input(control);
    std::string line;

    const auto assignField = [&]() {
        if (current_field == "Package") {
            info.name = trim(current_value);
        } else if (current_field == "Version") {
            info.version = trim(current_value);
        } else if (current_field == "Architecture") {
            info.architecture = trim(current_value);
        } else if (current_field == "Maintainer") {
            info.maintainer = trim(current_value);
        } else if (current_field == "Description") {
            info.description = trim(current_value);
        }
    };

    while (std::getline(input, line)) {
        if (line.empty()) {
            assignField();
            current_field.clear();
            current_value.clear();
            continue;
        }

        const auto pos = line.find(':');
        if (pos == std::string::npos) {
            current_value += (current_value.empty() ? "" : "\n") + trim(line);
            continue;
        }

        if (!current_field.empty()) {
            assignField();
        }

        current_field = trim(line.substr(0, pos));
        current_value = trim(line.substr(pos + 1));
    }

    if (!current_field.empty()) {
        assignField();
    }

    info.valid = !info.name.empty() && !info.version.empty();
    return info;
}

} // namespace ultrapkg
