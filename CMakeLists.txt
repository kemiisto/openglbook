cmake_minimum_required(VERSION 3.20)
project(openglbook)

set(CMAKE_CXX_STANDARD 20)

find_package(FreeGLUT CONFIG REQUIRED)
find_package(glew CONFIG REQUIRED)

link_libraries(FreeGLUT::freeglut GLEW::GLEW)

add_executable(chapter.1.1 01/chapter.1.1.cpp)
add_executable(chapter.1.2 01/chapter.1.2.cpp)
add_executable(chapter.1.3 01/chapter.1.3.cpp)

add_executable(chapter.2.1 02/chapter.2.1.cpp)
add_executable(chapter.2.2 02/chapter.2.2.cpp)
add_executable(chapter.2.3 02/chapter.2.3.cpp)
add_executable(chapter.2.4 02/chapter.2.4.cpp)
