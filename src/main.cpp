#include "core/PackageManager.h"
#include "thumbnail/DebThumbnailer.h"

#include <filesystem>
#include <iostream>
#include <stdexcept>
#include <string>

namespace {

void printHelp() {
    std::cout << "Usage:\n";
    std::cout << "  ultrapkg install <package.deb>\n";
    std::cout << "  ultrapkg remove <package-name>\n";
    std::cout << "  ultrapkg info <package.deb>\n";
    std::cout << "  ultrapkg thumb <package.deb> <output.png> [size]\n";
    std::cout << "  ultrapkg help\n";
}

} // namespace

int main(int argc, char** argv) {
    try {
        if (argc < 2) {
            printHelp();
            return 1;
        }

        const std::string command = argv[1];

        if (command == "help" || command == "--help" || command == "-h") {
            printHelp();
            return 0;
        }

        if (command == "install") {
            if (argc < 3) {
                printHelp();
                return 1;
            }
            ultrapkg::PackageManager manager;
            return manager.install(std::filesystem::path(argv[2]), std::cout) ? 0 : 2;
        }

        if (command == "remove") {
            if (argc < 3) {
                printHelp();
                return 1;
            }
            ultrapkg::PackageManager manager;
            return manager.uninstall(argv[2], std::cout) ? 0 : 2;
        }

        if (command == "info") {
            if (argc < 3) {
                printHelp();
                return 1;
            }
            ultrapkg::PackageManager manager;
            return manager.inspect(std::filesystem::path(argv[2]), std::cout) ? 0 : 2;
        }

        if (command == "thumb") {
            if (argc < 4) {
                printHelp();
                return 1;
            }

            unsigned int size = 128;
            if (argc >= 5) {
                size = static_cast<unsigned int>(std::stoul(argv[4]));
            }

            return ultrapkg::DebThumbnailer::generatePreview(
                std::filesystem::path(argv[2]),
                std::filesystem::path(argv[3]),
                size,
                std::cout) ? 0 : 3;
        }

        std::cerr << "unknown command: " << command << '\n';
        printHelp();
        return 1;
    } catch (const std::exception& ex) {
        std::cerr << "ultrapkg error: " << ex.what() << '\n';
        return 1;
    }
}
