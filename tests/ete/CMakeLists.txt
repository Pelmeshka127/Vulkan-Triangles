cmake_minimum_required(VERSION 3.8)

project(Ete LANGUAGES CXX)

add_executable(ete ete.cpp  ../../src/point.cpp ../../src/plane.cpp ../../src/line.cpp ../../src/vector.cpp
                            ../../src/triangle.cpp ../../src/octree.cpp ../../src/intersection.cpp
                            ../../src/double_numbers.cpp)