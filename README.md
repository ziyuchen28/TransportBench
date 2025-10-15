# TransportBench

Prereqs:
macOS: brew install curl cmake ninja
Ubuntu/Debian: sudo apt-get install libcurl4-openssl-dev cmake ninja-build build-essential
Fedora/RHEL: sudo dnf install libcurl-devel cmake ninja-build gcc-c++
Windows (option A, MSYS2/MinGW): pacman -S --needed base-devel mingw-w64-x86_64-toolchain mingw-w64-x86_64-cmake mingw-w64-x86_64-ninja mingw-w64-x86_64-curl (run in MSYS2 MinGW64 shell)
Windows (option B, MSVC/Visual Studio): install VS 2022 + CMake; install or point to a libcurl build


Build:
# Linux/macOS (Ninja):
cmake --preset ninja-release
cmake --build --preset ninja-release
# Windows (Visual Studio users without Ninja):
cmake --preset msvc-release
cmake --build --preset msvc-release

Run:
