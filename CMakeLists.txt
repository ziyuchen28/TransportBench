cmake_minimum_required(VERSION 3.16)
project(apibench CXX)
set(CMAKE_CXX_STANDARD 17)
find_package(CURL REQUIRED)
add_executable(apibench main.cpp)
target_link_libraries(apibench PRIVATE CURL::libcurl)

