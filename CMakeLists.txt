cmake_minimum_required(VERSION 3.5.1)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

project(MyGlfwTest)
add_executable(MyGlfwTest main.cpp glad/src/glad.c opengl.cpp)

set(CMAKE_BUILD_TYPE Debug)

target_include_directories(MyGlfwTest PUBLIC glad/include)

# GLFW:
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
add_subdirectory(glfw-3.3.8/)
target_link_libraries(MyGlfwTest glfw)
target_include_directories(MyGlfwTest PUBLIC glfw-3.3.8/include)

# Freetype
target_include_directories(MyGlfwTest PUBLIC freetype-2.13.0/include)
add_subdirectory(freetype-2.13.0/)
target_link_libraries(MyGlfwTest freetype)
