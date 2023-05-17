cmake_minimum_required(VERSION 3.22)
project(snail_cli)

set(CMAKE_CXX_STANDARD 14)

include(toolchain.cmake)

include(FetchContent)
FetchContent_Declare(
        CLI11
        GIT_REPOSITORY https://github.com/CLIUtils/CLI11
        GIT_TAG        v2.3.2
)
FetchContent_MakeAvailable(cli11)

add_subdirectory(snail)
include_directories(include)
#set(CMAKE_EXE_LINKER_FLAGS "-static-libgcc -static-libstdc++")
add_executable(snail_cli main.cpp)
target_link_libraries(snail_cli PRIVATE snail CLI11::CLI11 )
