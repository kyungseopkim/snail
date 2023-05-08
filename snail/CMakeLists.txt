cmake_minimum_required(VERSION 3.10)
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

FetchContent_Declare(cpr 
        GIT_REPOSITORY https://github.com/libcpr/cpr.git
        GIT_TAG 1.9.4 
        CMAKE_ARGS -DCPR_BUILD_TESTS=OFF -DBUILD_STATIC_LIBS=ON)
FetchContent_MakeAvailable(cpr)

FetchContent_Declare(leveldb GIT_REPOSITORY https://github.com/google/leveldb.git
        GIT_TAG 1.23       
        )
FetchContent_MakeAvailable(leveldb)



# project
add_subdirectory(base64)

include_directories(include)

# sources
file(GLOB_RECURSE SOURCES "src/*.cpp")
add_library(snail ${SOURCES} )
target_link_libraries(snail PRIVATE base64 cpr::cpr CLI11::CLI11 leveldb )


