cmake_minimum_required(VERSION 3.8)

project(Unit LANGUAGES CXX)

add_executable(unit unit.cpp    ../../src/point.cpp ../../src/plane.cpp ../../src/line.cpp ../../src/vector.cpp
                                ../../src/triangle.cpp ../../src/octree.cpp ../../src/intersection.cpp
                                ../../src/double_numbers.cpp)

target_link_libraries(unit PRIVATE ${GTEST_LIBRARIES})