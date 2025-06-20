cmake_minimum_required(VERSION 3.20)
project(AssetValidator VERSION 1.0.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Find vcpkg packages
find_package(assimp CONFIG REQUIRED)
find_package(glm CONFIG REQUIRED)
find_package(fmt CONFIG REQUIRED)
find_package(nlohmann_json CONFIG REQUIRED)

# Source files
set(SOURCES
    src/main.cpp
    src/AssetLoader.cpp
    src/Validator.cpp
    src/Reporter.cpp
    src/ValidationConfig.cpp
)

set(HEADERS
    include/AssetLoader.h
    include/Validator.h
    include/Reporter.h
    include/ValidationConfig.h
    include/ValidationResult.h
)

# Create executable
add_executable(${PROJECT_NAME} ${SOURCES} ${HEADERS})

# Include directories
target_include_directories(${PROJECT_NAME} PRIVATE 
    include
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

# Link libraries
target_link_libraries(${PROJECT_NAME} PRIVATE 
    assimp::assimp
    glm::glm
    fmt::fmt
    nlohmann_json::nlohmann_json
)

# Compiler flags
if(MSVC)
    target_compile_options(${PROJECT_NAME} PRIVATE /W4)
else()
    target_compile_options(${PROJECT_NAME} PRIVATE -Wall -Wextra -Wpedantic)
endif()

# Copy config file to build directory
configure_file(
    ${CMAKE_CURRENT_SOURCE_DIR}/config/validation_config.json
    ${CMAKE_CURRENT_BINARY_DIR}/validation_config.json
    COPYONLY
)