cmake_minimum_required(VERSION 3.10.0)
project(learn_opengl VERSION 0.1.0 LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# include
include_directories(include)
include_directories(/opt/homebrew/include)
link_directories(/opt/homebrew/lib)
include_directories(${CMAKE_SOURCE_DIR}/shaders)
include_directories(external/imgui)
include_directories(external/stb)
# add
add_library(glad src/glad.c)

# add Dear ImGui as a library
set(IMGUI_SOURCES
    external/imgui/imgui.cpp
    external/imgui/imgui_demo.cpp
    external/imgui/imgui_draw.cpp
    external/imgui/imgui_tables.cpp
    external/imgui/imgui_widgets.cpp
    external/imgui/backends/imgui_impl_glfw.cpp
    external/imgui/backends/imgui_impl_opengl3.cpp
)
add_library(imgui ${IMGUI_SOURCES})

add_executable(learn_opengl 
src/main.cpp
src/glad.c
)

find_package(OpenGL REQUIRED)
find_package(glfw3 REQUIRED)
find_package(glm REQUIRED)
find_package(assimp REQUIRED)

# Link libraries
target_link_libraries(learn_opengl PRIVATE 
    OpenGL::GL 
    glfw
    glad
    imgui
    assimp        # from homebrew
)