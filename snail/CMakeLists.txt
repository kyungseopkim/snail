cmake_minimum_required(VERSION 3.10)
project(snail VERSION 0.0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 11)

# dependencies
# -DLEVELDB_BUILD_TESTS=OFF -DLEVELDB_BUILD_BENCHMARKS=OFF  -DCPR_BUILD_TESTS=OFF -DBUILD_SHARED_LIBS=OFF


include(FetchContent)
FetchContent_Declare(cpr
        GIT_REPOSITORY https://github.com/libcpr/cpr.git
        GIT_TAG 1.9.4
        CMAKE_ARGS -DCPR_BUILD_TESTS=OFF -DBUILD_STATIC_LIBS=ON)
FetchContent_MakeAvailable(cpr)

FetchContent_Declare(leveldb GIT_REPOSITORY https://github.com/google/leveldb.git
        GIT_TAG 1.23
        CMAKE_ARGS -DLEVELDB_BUILD_TESTS=OFF -DLEVELDB_BUILD_BENCHMARKS=OFF -DBUILD_SHARED_LIBS=OFF
        )
FetchContent_MakeAvailable(leveldb)

add_subdirectory(base64)
# project

include_directories(include)
# sources
file(GLOB SOURCES "*.cpp")
add_library(snail STATIC ${SOURCES})

target_link_libraries(snail PUBLIC cpr::cpr leveldb base64 )
target_include_directories(snail PUBLIC ${PROJECT_SOURCE_DIR}/include)

