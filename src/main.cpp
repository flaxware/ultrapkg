#include "thumbnail/DebThumbnailer.h"

#include <array>
#include <cstdlib>
#include <filesystem>
#include <fstream>
#include <iostream>
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

} // namespace

bool DebThumbnailer::generatePreview(const std::filesystem::path& deb_path,
                                    const std::filesystem::path& output_path,
                                    unsigned int size,
                                    std::ostream& log) {
    if (!std::filesystem::exists(deb_path)) {
        log << "deb package not found: " << deb_path << '\n';
        return false;
    }

    if (!output_path.parent_path().empty()) {
        std::filesystem::create_directories(output_path.parent_path());
    }

    const std::string temp_dir = "/tmp/ultrapkg-thumb-" + std::to_string(std::time(nullptr));
    const std::string extract_cmd = "mkdir -p " + shellQuote(temp_dir) + " && dpkg-deb -x " + shellQuote(deb_path.string()) + " " + shellQuote(temp_dir);
    if (std::system(extract_cmd.c_str()) != 0) {
        log << "failed to extract package to temporary directory\n";
        return false;
    }

    const std::string render_cmd =
        "convert -size " + std::to_string(size) + "x" + std::to_string(size) +
        " xc:'#0f172a' -fill '#f8fafc' -draw 'roundrectangle 8,8 " +
        std::to_string(size - 8) + "," + std::to_string(size - 8) + " 14,14' " +
        "-fill '#7c3aed' -draw 'rectangle 14,14 " + std::to_string(size - 14) + "," +
        std::to_string(size - 14) + "' -fill white -gravity center -pointsize 20 -annotate +0+0 'deb' " +
        shellQuote(output_path.string());

    if (std::system(render_cmd.c_str()) != 0) {
        log << "thumbnail generation failed\n";
        return false;
    }

    return true;
}

} // namespace ultrapkg
