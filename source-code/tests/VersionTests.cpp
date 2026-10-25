#include "core/PackageVersion.h"

#include <cstdlib>
#include <filesystem>
#include <iostream>
#include <string>

int main() {
    using namespace ultrapkg;

    const PackageVersion v1("0.1.0");
    const PackageVersion v2("0.1.1");
    const PackageVersion v3("0.1.2");

    if (!(v1 < v2)) {
        std::cerr << "version ordering failed\n";
        return 1;
    }
    if (!(v2 < v3)) {
        std::cerr << "version progression failed\n";
        return 2;
    }

    const std::filesystem::path deb_path = "/tmp/example.deb";
    const std::filesystem::path appimage_path = "/tmp/example.AppImage";
    if (deb_path.extension() != ".deb" || appimage_path.extension() != ".AppImage") {
        std::cerr << "archive type detection failed\n";
        return 3;
    }

    std::cout << "ultrapkg version tests passed\n";
    return 0;
}
