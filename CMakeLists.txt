cmake_minimum_required(VERSION 3.14)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

set(CMAKE_CXX_STANDARD 17)

set(LLVM_PREFIX "/your/path/to/llvm-mingw")
set(LLVM_ARCH "x86_64")
include(llvm-mingw)

project(wtltest CXX RC)

enable_language(RC)

add_compile_options(-Wall)

add_subdirectory(3rdparty)

add_executable(wtltest WIN32 main.cpp wtltest.rc)
target_compile_definitions(wtltest PRIVATE UNICODE _UNICODE)
target_compile_options(wtltest PRIVATE -municode)
target_link_options(wtltest PRIVATE -municode)
target_link_libraries(wtltest PRIVATE wtl openatl comctl32 msimg32)
