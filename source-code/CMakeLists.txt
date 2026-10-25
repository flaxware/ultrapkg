cmake_minimum_required(VERSION 3.21)
project(ultrapkg VERSION 0.1.2 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_executable(ultrapkg
    src/main.cpp
    src/core/PackageInfo.cpp
    src/core/PackageInstaller.cpp
    src/core/PackageManager.cpp
    src/core/PackageVersion.cpp
    src/thumbnail/DebThumbnailer.cpp
)

target_include_directories(ultrapkg
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/src
)

enable_testing()
add_executable(ultrapkg_tests
    tests/VersionTests.cpp
    src/core/PackageVersion.cpp
)

target_include_directories(ultrapkg_tests
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/src
)

add_test(NAME ultrapkg_version_tests COMMAND ultrapkg_tests)

install(TARGETS ultrapkg RUNTIME DESTINATION bin)
