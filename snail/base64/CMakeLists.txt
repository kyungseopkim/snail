project(base64 VERSION 1.0.0 LANGUAGES CXX)

add_library(base64 STATIC base64.cpp)
target_include_directories(base64 PUBLIC .)
