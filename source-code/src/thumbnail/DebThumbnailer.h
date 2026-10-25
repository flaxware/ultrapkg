#pragma once

#include <filesystem>
#include <iostream>
#include <iosfwd>
#include <string>

namespace ultrapkg {

class DebThumbnailer {
public:
    static bool generatePreview(const std::filesystem::path& archive_path,
                                const std::filesystem::path& output_path,
                                unsigned int size = 128,
                                std::ostream& log = std::cout);
};

} // namespace ultrapkg
