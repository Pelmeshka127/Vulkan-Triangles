cmake_minimum_required(VERSION 3.8)

project(Triangles LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# set(CMAKE_CXX_FLAGS_DEBUG "-O0 -D DEBUG")
# set(CMAKE_CXX_FLAGS_RELEASE "-O2 -DNDEBUG")

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif()

find_package(GTest REQUIRED)
enable_testing()

add_subdirectory(./src)

add_subdirectory(./tests)

target_include_directories(triangles PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/inc)

target_include_directories(ete PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/inc)

target_include_directories(unit PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/inc)