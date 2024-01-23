cmake_minimum_required(VERSION 3.8)

project(Triangles LANGUAGES CXX)

add_executable(triangles double_numbers.cpp point.cpp vector.cpp line.cpp
                plane.cpp triangle.cpp octree.cpp intersection.cpp 
                app.cpp camera.cpp device.cpp keyboard.cpp
                model.cpp pipeline.cpp render_system.cpp
                render.cpp swap_chain.cpp window.cpp main.cpp)