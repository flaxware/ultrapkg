cmake_minimum_required(VERSION 3.21)
project(ultrapkg VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_executable(ultrapkg
    src/main.cpp
    src/core/PackageInfo.cpp
    src/core/PackageInstaller.cpp
    src/core/PackageManager.cpp
    src/thumbnail/DebThumbnailer.cpp
)

target_include_directories(ultrapkg
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/src
)

install(TARGETS ultrapkg DESTINATION bin)
