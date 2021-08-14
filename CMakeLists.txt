cmake_minimum_required(VERSION 3.20)
project(openglbook)

set(CMAKE_CXX_STANDARD 20)

find_package(FreeGLUT CONFIG REQUIRED)
find_package(glew CONFIG REQUIRED)

link_libraries(FreeGLUT::freeglut GLEW::GLEW)

add_executable(chapter.1.1 01/chapter.1.1.cpp)
