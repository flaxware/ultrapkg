#include "core/PackageInfo.h"

#include <array>
#include <cctype>
#include <cstdio>
#include <cstdlib>
#include <filesystem>
#include <fstream>
#include <sstream>
#include <string>

namespace ultrapkg {
namespace {

std::string shellQuote(const std::string& value) {
    std::string out;
    out.reserve(value.size() + 2);
    out.push_back('\'');
    for (char ch : value) {
        if (ch == '\'') {
            out += "'\\''";
        } else {
            out.push_back(ch);
        }
    }
    out.push_back('\'');
    return out;
}

std::string lower(std::string value) {
    for (char& ch : value) {
        ch = static_cast<char>(std::tolower(static_cast<unsigned char>(ch)));
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

    pclose(pipe);
    return result;
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

} // namespace

PackageInfo PackageInfoReader::fromDeb(const std::filesystem::path& archive_path) {
    PackageInfo info{};
    if (!std::filesystem::exists(archive_path)) {
        return info;
    }

    const auto ext = lower(archive_path.extension().string());
    if (ext == ".appimage") {
        info.name = archive_path.stem().string();
        info.version = "appimage";
        info.architecture = "appimage";
        info.maintainer = "n/a";
        info.description = "AppImage package";
        info.valid = true;
        return info;
    }

    if (ext != ".deb") {
        return info;
    }

    const std::string path = shellQuote(archive_path.string());
    const auto readField = [&](const std::string& field) -> std::string {
        const std::string command = "dpkg-deb -f " + path + " " + field;
        std::string value = execCapture(command);
        if (!value.empty() && value.back() == '\n') {
            value.pop_back();
        }
        return value;
    };

    const std::string name = readField("Package");
    const std::string version = readField("Version");
    const std::string arch = readField("Architecture");
    const std::string maintainer = readField("Maintainer");
    const std::string description = readField("Description");

    if (!name.empty() && !version.empty()) {
        info.name = name;
        info.version = version;
        info.architecture = arch;
        info.maintainer = maintainer;
        info.description = description;
        info.valid = true;
    }

    return info;
}

} // namespace ultrapkg
