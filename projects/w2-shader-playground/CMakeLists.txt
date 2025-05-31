cmake_minimum_required(VERSION 3.10.0)
project(shader_playground VERSION 0.1.0 LANGUAGES C CXX)

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


add_executable(shader_playground 
src/main.cpp
src/glad.c
)

find_package(OpenGL REQUIRED)
find_package(glfw3 REQUIRED)
target_link_libraries(shader_playground PRIVATE OpenGL::GL glfw)
