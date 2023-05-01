cmake_minimum_required(VERSION 3.22)
project(snail)

set(CMAKE_CXX_STANDARD 11)

# dependencies
include(FetchContent)
FetchContent_Declare(
        CLI11
        GIT_REPOSITORY https://github.com/CLIUtils/CLI11
        GIT_TAG        v2.3.2
)
FetchContent_MakeAvailable(cli11)

FetchContent_Declare(cpr GIT_REPOSITORY https://github.com/libcpr/cpr.git
        GIT_TAG 1.9.4 )
FetchContent_MakeAvailable(cpr)

# project

add_subdirectory(base64)
add_subdirectory(leveldb)

include_directories(include)

# sources
file(GLOB_RECURSE SOURCES "src/*.cpp")
add_executable(snail ${SOURCES} )
target_link_libraries(snail base64 cpr::cpr CLI11::CLI11 leveldb)


