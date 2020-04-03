cmake_minimum_required(VERSION 3.5)

project(bip39-cxx LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)


add_subdirectory(src)

add_executable(main main.cpp)

target_link_libraries(main PRIVATE bip39-cxx)
