cmake_minimum_required(VERSION 3.14)

add_library(openatl INTERFACE)
target_include_directories(openatl INTERFACE
    ${CMAKE_CURRENT_SOURCE_DIR}/openatl/include
)

add_library(wtl INTERFACE)
target_include_directories(wtl SYSTEM INTERFACE
    ${CMAKE_CURRENT_SOURCE_DIR}/wtl/include
)
