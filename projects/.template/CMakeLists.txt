cmake_minimum_required(VERSION 3.10.0)
project(triangle_glfw VERSION 0.1.0 LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 17)

# include glad
include_directories(include)
add_library(glad src/glad.c)

# include homebrew
include_directories(/opt/homebrew/include)
link_directories(/opt/homebrew/lib)

#include glm
include_directories(/opt/homebrew/include)

#include shader
include_directories(${CMAKE_SOURCE_DIR}/shaders)


add_executable(triangle_glfw 
src/main.cpp
src/glad.c
)

find_package(OpenGL REQUIRED)
find_package(glfw3 REQUIRED)
target_link_libraries(triangle_glfw PRIVATE OpenGL::GL glfw)
