cmake_minimum_required(VERSION 3.18.0)

set(CMAKE_CXX_STANDARD 17)

project(OpenglPlayground)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_BUILD_TYPE Debug)

find_package(glfw3 CONFIG REQUIRED)
find_package(GLEW REQUIRED)
find_package(OpenGL REQUIRED)

find_path(GLFW_INCLUDE_DIR GLFW)
include_directories(${GLFW_INCLUDE_DIR})

add_executable(ch2 src/ch2.cc)
target_link_libraries(ch2 PRIVATE glfw GLEW::GLEW OpenGL::GL)
