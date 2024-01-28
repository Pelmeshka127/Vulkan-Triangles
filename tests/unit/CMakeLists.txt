cmake_minimum_required(VERSION 3.8)

project(Unit LANGUAGES CXX)

add_executable(unit unit.cpp  ../../geometry/src/point.cpp ../../geometry/src/plane.cpp ../../geometry/src/line.cpp ../../geometry/src/vector.cpp
                            ../../geometry/src/triangle.cpp ../../geometry/src/octree.cpp ../../geometry/src/intersection.cpp
                            ../../geometry/src/double_numbers.cpp)

target_link_libraries(unit PRIVATE ${GTEST_LIBRARIES})