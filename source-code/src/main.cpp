#include "core/PackageManager.h"
#include "core/PackageInfo.h"
#include "core/PackageVersion.h"
#include "thumbnail/DebThumbnailer.h"

#include <filesystem>
#include <fstream>
#include <iostream>
#include <string>

namespace {

std::string lower(std::string value) {
    for (char& ch : value) {
        ch = static_cast<char>(std::tolower(static_cast<unsigned char>(ch)));
    }
    return value;
}

bool isSupportedArchive(const std::filesystem::path& archive_path) {
    const auto ext = lower(archive_path.extension().string());
    return ext == ".deb" || ext == ".appimage";
}

} // namespace

int main(int argc, char** argv) {
    try {
        if (argc < 2) {
            std::cout << "Usage:\n";
            std::cout << "  ultrapkg version\n";
            std::cout << "  ultrapkg install <package.deb|package.AppImage>\n";
            std::cout << "  ultrapkg appimage <package.AppImage>\n";
            std::cout << "  ultrapkg remove <package-name>\n";
            std::cout << "  ultrapkg info <package.deb|package.AppImage>\n";
            std::cout << "  ultrapkg list\n";
            std::cout << "  ultrapkg search <query>\n";
            std::cout << "  ultrapkg show <package-name>\n";
            std::cout << "  ultrapkg apt install <package.deb|package.AppImage>\n";
            std::cout << "  ultrapkg thumb <package.deb|package.AppImage> <output.png> [size]\n";
            std::cout << "  ultrapkg help\n";
            return 1;
        }

        const std::string command = argv[1];
        if (command == "help" || command == "--help" || command == "-h") {
            std::cout << "Usage:\n";
            std::cout << "  ultrapkg version\n";
            std::cout << "  ultrapkg install <package.deb|package.AppImage>\n";
            std::cout << "  ultrapkg appimage <package.AppImage>\n";
            std::cout << "  ultrapkg remove <package-name>\n";
            std::cout << "  ultrapkg info <package.deb|package.AppImage>\n";
            std::cout << "  ultrapkg list\n";
            std::cout << "  ultrapkg search <query>\n";
            std::cout << "  ultrapkg show <package-name>\n";
            std::cout << "  ultrapkg apt install <package.deb|package.AppImage>\n";
            std::cout << "  ultrapkg thumb <package.deb|package.AppImage> <output.png> [size]\n";
            std::cout << "  ultrapkg help\n";
            return 0;
        }

        if (command == "version" || command == "--version") {
            std::cout << "ultrapkg v0.1.2\n";
            return 0;
        }

        if (command == "appimage") {
            if (argc < 3) {
                std::cerr << "missing AppImage path\n";
                return 1;
            }
            ultrapkg::PackageManager manager;
            return manager.install(std::filesystem::path(argv[2]), std::cout) ? 0 : 2;
        }

        if (command == "install" || command == "dpkg") {
            if (argc < 3) {
                std::cerr << "missing package path\n";
                return 1;
            }
            ultrapkg::PackageManager manager;
            return manager.install(std::filesystem::path(argv[2]), std::cout) ? 0 : 2;
        }

        if (command == "remove") {
            if (argc < 3) {
                std::cerr << "missing package name\n";
                return 1;
            }
            ultrapkg::PackageManager manager;
            return manager.uninstall(argv[2], std::cout) ? 0 : 2;
        }

        if (command == "info") {
            if (argc < 3) {
                std::cerr << "missing package path\n";
                return 1;
            }
            ultrapkg::PackageManager manager;
            return manager.inspect(std::filesystem::path(argv[2]), std::cout) ? 0 : 2;
        }

        if (command == "list") {
            ultrapkg::PackageManager manager;
            return manager.aptList(std::cout) ? 0 : 1;
        }

        if (command == "search") {
            if (argc < 3) {
                std::cerr << "missing search query\n";
                return 1;
            }
            ultrapkg::PackageManager manager;
            return manager.aptSearch(argv[2], std::cout) ? 0 : 1;
        }

        if (command == "show") {
            if (argc < 3) {
                std::cerr << "missing package name\n";
                return 1;
            }
            ultrapkg::PackageManager manager;
            return manager.aptShow(argv[2], std::cout) ? 0 : 1;
        }

        if (command == "apt") {
            if (argc < 3) {
                std::cerr << "missing apt subcommand\n";
                return 1;
            }
            ultrapkg::PackageManager manager;
            const std::string subcommand = argv[2];
            if (subcommand == "install") {
                if (argc < 4) {
                    std::cerr << "missing package path\n";
                    return 1;
                }
                return manager.aptInstall(std::filesystem::path(argv[3]), std::cout) ? 0 : 2;
            }
            if (subcommand == "list") {
                return manager.aptList(std::cout) ? 0 : 1;
            }
            if (subcommand == "show") {
                if (argc < 4) {
                    std::cerr << "missing package name\n";
                    return 1;
                }
                return manager.aptShow(argv[3], std::cout) ? 0 : 1;
            }
            if (subcommand == "search") {
                if (argc < 4) {
                    std::cerr << "missing search query\n";
                    return 1;
                }
                return manager.aptSearch(argv[3], std::cout) ? 0 : 1;
            }
            std::cerr << "unsupported apt subcommand\n";
            return 1;
        }

        if (command == "thumb") {
            if (argc < 4) {
                std::cerr << "thumb requires <package> <output>\n";
                return 1;
            }

            const std::filesystem::path archive_path = argv[2];
            const std::filesystem::path output_path = argv[3];
            unsigned int size = 128;
            if (argc >= 5) {
                size = static_cast<unsigned int>(std::stoul(argv[4]));
            }

            if (!isSupportedArchive(archive_path)) {
                std::cerr << "unsupported archive type: " << archive_path << '\n';
                return 1;
            }

            return ultrapkg::DebThumbnailer::generatePreview(archive_path, output_path, size, std::cout) ? 0 : 3;
        }

        std::cerr << "unknown command: " << command << '\n';
        return 1;
    } catch (const std::exception& ex) {
        std::cerr << "ultrapkg error: " << ex.what() << '\n';
        return 1;
    }
}
