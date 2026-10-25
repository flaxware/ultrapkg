#include "core/PackageInfo.h"

#include <array>
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

    pclose(pipe);
    return result;
}

} // namespace

PackageInfo PackageInfoReader::fromDeb(const std::filesystem::path& deb_path) {
    PackageInfo info{};

    if (!std::filesystem::exists(deb_path)) {
        return info;
    }

    const auto fields = {
        std::string("Package"),
        std::string("Version"),
        std::string("Architecture"),
        std::string("Maintainer"),
        std::string("Description")
    };

    std::string name = "";
    std::string version = "";
    std::string arch = "";
    std::string maintainer = "";
    std::string description = "";

    const std::string path = shellQuote(deb_path.string());

    const auto readField = [&](const std::string& field) -> std::string {
        const std::string command = "dpkg-deb -f " + path + " " + field;
        std::string value = execCapture(command);
        if (!value.empty() && value.back() == '\n') {
            value.pop_back();
        }
        return value;
    };

    name = readField("Package");
    version = readField("Version");
    arch = readField("Architecture");
    maintainer = readField("Maintainer");
    description = readField("Description");

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
