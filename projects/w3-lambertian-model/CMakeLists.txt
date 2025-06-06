cmake_minimum_required(VERSION 3.10)
project(lambertian_model VERSION 0.1 LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 17)

# === Include Paths ===
include_directories(include)                         # for shader.h, camera.h, glad/
include_directories(/opt/homebrew/include)           # for GLFW, GLM
link_directories(/opt/homebrew/lib)

# === External GLAD ===
add_library(glad src/glad.c)

# === Executable Target ===
add_executable(lambertian_model
    src/main.cpp
    src/shader.cpp     # <--- make sure these are added
    src/camera.cpp     # <--- make sure these are added
)
# === Linking ===
find_package(OpenGL REQUIRED)
find_package(glfw3 REQUIRED)
target_link_libraries(lambertian_model PRIVATE glad OpenGL::GL glfw)

# === Copy Shader Files ===
add_custom_command(TARGET lambertian_model POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E make_directory $<TARGET_FILE_DIR:lambertian_model>/shader
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_SOURCE_DIR}/shaders
        $<TARGET_FILE_DIR:lambertian_model>/shader
)
