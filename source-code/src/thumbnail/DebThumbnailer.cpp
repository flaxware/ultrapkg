#include "thumbnail/DebThumbnailer.h"

#include <array>
#include <cstdlib>
#include <ctime>
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

} // namespace

bool DebThumbnailer::generatePreview(const std::filesystem::path& archive_path,
                                    const std::filesystem::path& output_path,
                                    unsigned int size,
                                    std::ostream& log) {
    if (!std::filesystem::exists(archive_path)) {
        log << "package not found: " << archive_path << '\n';
        return false;
    }

    if (!output_path.parent_path().empty()) {
        std::filesystem::create_directories(output_path.parent_path());
    }

    const std::string ext = lower(archive_path.extension().string());
    const std::string label = (ext == ".appimage") ? "app" : "deb";
    const std::string bg = (ext == ".appimage") ? "#1d4ed8" : "#0f172a";
    const std::string accent = (ext == ".appimage") ? "#a78bfa" : "#7c3aed";

    if (ext == ".deb") {
        const std::string temp_dir = "/tmp/ultrapkg-thumb-" + std::to_string(std::time(nullptr));
        const std::string extract_cmd =
            "mkdir -p " + shellQuote(temp_dir) +
            " && dpkg-deb -x " + shellQuote(archive_path.string()) +
            " " + shellQuote(temp_dir);

        if (std::system(extract_cmd.c_str()) != 0) {
            log << "failed to extract package files\n";
            return false;
        }
    }

    const std::string render_cmd =
        "convert -size " + std::to_string(size) + "x" + std::to_string(size) +
        " xc:'" + bg + "' -fill '#f8fafc' -draw 'roundrectangle 8,8 " +
        std::to_string(size - 8) + "," + std::to_string(size - 8) + " 14,14' " +
        "-fill '" + accent + "' -draw 'rectangle 14,14 " +
        std::to_string(size - 14) + "," + std::to_string(size - 14) + "' " +
        "-fill white -gravity center -pointsize 20 -annotate +0+0 '" + label + "' " +
        shellQuote(output_path.string());

    if (std::system(render_cmd.c_str()) != 0) {
        log << "thumbnail generation failed\n";
        return false;
    }

    std::error_code ec;
    std::filesystem::permissions(output_path,
                                 std::filesystem::perms::owner_exec |
                                     std::filesystem::perms::group_exec |
                                     std::filesystem::perms::others_exec,
                                 std::filesystem::perm_options::add,
                                 ec);
    return true;
}

} // namespace ultrapkg
