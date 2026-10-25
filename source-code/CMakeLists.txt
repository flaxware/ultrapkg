#include "core/PackageManager.h"
#include "thumbnail/DebThumbnailer.h"

#include <filesystem>
#include <iostream>
#include <stdexcept>
#include <string>

namespace {

void printHelp() {
    std::cout << "Usage:\n";
    std::cout << "  ultrapkg version\n";
    std::cout << "  ultrapkg install <package.deb>\n";
    std::cout << "  ultrapkg remove <package-name>\n";
    std::cout << "  ultrapkg info <package.deb>\n";
    std::cout << "  ultrapkg list\n";
    std::cout << "  ultrapkg search <query>\n";
    std::cout << "  ultrapkg show <package-name>\n";
    std::cout << "  ultrapkg apt install <package.deb>\n";
    std::cout << "  ultrapkg apt list\n";
    std::cout << "  ultrapkg apt search <query>\n";
    std::cout << "  ultrapkg apt show <package-name>\n";
    std::cout << "  ultrapkg aptitude search <query>\n";
    std::cout << "  ultrapkg aptitude depends <package-name>\n";
    std::cout << "  ultrapkg -dpkg <package.deb>\n";
    std::cout << "  ultrapkg --dpkg <package.deb>\n";
    std::cout << "  ultrapkg thumb <package.deb> <output.png> [size]\n";
    std::cout << "  ultrapkg help\n";
}

void printVersion() {
    std::cout << "ultrapkg v0.1.0\n";
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

        if (command == "version" || command == "--version") {
            printVersion();
            return 0;
        }

        if (command == "-dpkg" || command == "--dpkg") {
            if (argc < 3) {
                printHelp();
                return 1;
            }
            ultrapkg::PackageManager manager;
            return manager.install(std::filesystem::path(argv[2]), std::cout) ? 0 : 2;
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

        if (command == "list") {
            ultrapkg::PackageManager manager;
            return manager.aptList(std::cout) ? 0 : 1;
        }

        if (command == "search") {
            if (argc < 3) {
                printHelp();
                return 1;
            }
            ultrapkg::PackageManager manager;
            return manager.aptSearch(argv[2], std::cout) ? 0 : 1;
        }

        if (command == "show") {
            if (argc < 3) {
                printHelp();
                return 1;
            }
            ultrapkg::PackageManager manager;
            return manager.aptShow(argv[2], std::cout) ? 0 : 1;
        }

        if (command == "apt") {
            if (argc < 3) {
                printHelp();
                return 1;
            }

            ultrapkg::PackageManager manager;
            const std::string subcommand = argv[2];
            if (subcommand == "install") {
                if (argc < 4) {
                    printHelp();
                    return 1;
                }
                return manager.aptInstall(std::filesystem::path(argv[3]), std::cout) ? 0 : 2;
            }
            if (subcommand == "remove") {
                if (argc < 4) {
                    printHelp();
                    return 1;
                }
                return manager.aptRemove(argv[3], std::cout) ? 0 : 2;
            }
            if (subcommand == "list") {
                return manager.aptList(std::cout) ? 0 : 1;
            }
            if (subcommand == "search") {
                if (argc < 4) {
                    printHelp();
                    return 1;
                }
                return manager.aptSearch(argv[3], std::cout) ? 0 : 1;
            }
            if (subcommand == "show") {
                if (argc < 4) {
                    printHelp();
                    return 1;
                }
                return manager.aptShow(argv[3], std::cout) ? 0 : 1;
            }
            printHelp();
            return 1;
        }

        if (command == "aptitude") {
            if (argc < 3) {
                printHelp();
                return 1;
            }

            ultrapkg::PackageManager manager;
            const std::string subcommand = argv[2];
            if (subcommand == "search") {
                if (argc < 4) {
                    printHelp();
                    return 1;
                }
                return manager.aptitudeSearch(argv[3], std::cout) ? 0 : 1;
            }
            if (subcommand == "depends") {
                if (argc < 4) {
                    printHelp();
                    return 1;
                }
                return manager.aptitudeDepends(argv[3], std::cout) ? 0 : 1;
            }
            if (subcommand == "install") {
                if (argc < 4) {
                    printHelp();
                    return 1;
                }
                return manager.aptitudeInstall(std::filesystem::path(argv[3]), std::cout) ? 0 : 2;
            }
            printHelp();
            return 1;
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
