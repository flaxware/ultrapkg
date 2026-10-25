#include "core/PackageInfo.h"

#include <array>
#include <cstdio>
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

std::string trim(std::string value) {
    while (!value.empty() && (value.front() == ' ' || value.front() == '\t' || value.front() == '\n' || value.front() == '\r')) {
        value.erase(value.begin());
    }
    while (!value.empty() && (value.back() == ' ' || value.back() == '\t' || value.back() == '\n' || value.back() == '\r')) {
        value.pop_back();
    }
    return value;
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

std::string readControlFromArchive(const std::filesystem::path& deb_path) {
    if (!std::filesystem::exists(deb_path) || !std::filesystem::is_regular_file(deb_path)) {
        return {};
    }

    const std::filesystem::path abs_path = std::filesystem::absolute(deb_path);
    const std::string archive_list = execCapture("ar t " + shellQuote(abs_path.string()) + " 2>/dev/null");
    if (archive_list.empty()) {
        return {};
    }

    std::istringstream stream(archive_list);
    std::string member;
    while (std::getline(stream, member)) {
        if (member.empty()) {
            continue;
        }
        if (member.find("control.tar") == std::string::npos) {
            continue;
        }

        const std::string extract_cmd =
            "ar p " + shellQuote(abs_path.string()) + " " + shellQuote(member) +
            " | tar -xOf - ./control 2>/dev/null";
        const std::string control = execCapture(extract_cmd);
        if (!control.empty()) {
            return control;
        }
    }

    return {};
}

} // namespace

PackageInfo PackageInfoReader::fromDeb(const std::filesystem::path& deb_path) {
    PackageInfo info{};
    if (!std::filesystem::exists(deb_path) || !std::filesystem::is_regular_file(deb_path)) {
        return info;
    }

    const std::string control = readControlFromArchive(deb_path);
    if (control.empty()) {
        return info;
    }

    std::istringstream input(control);
    std::string line;
    std::string current_field;
    std::string current_value;

    const auto assign = [&]() {
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
            assign();
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
            assign();
        }

        current_field = trim(line.substr(0, pos));
        current_value = trim(line.substr(pos + 1));
    }

    if (!current_field.empty()) {
        assign();
    }

    info.valid = !info.name.empty() && !info.version.empty();
    return info;
}

} // namespace ultrapkg
