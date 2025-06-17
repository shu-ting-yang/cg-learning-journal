#include "shader.h"
#include <glad/glad.h>
#include <fstream>
#include <sstream>
#include <iostream>

Shader::Shader(const char* vertexPath, const char* fragmentPath) {
    // Read shader files
    std::ifstream vFile(vertexPath), fFile(fragmentPath);
    if (!vFile.is_open()) {
        std::cerr << "ERROR: Could not open vertex shader file: " << vertexPath << std::endl;
        return;
    }
    if (!fFile.is_open()) {
        std::cerr << "ERROR: Could not open fragment shader file: " << fragmentPath << std::endl;
        return;
    }
    
    std::stringstream vStream, fStream;
    vStream << vFile.rdbuf();
    fStream << fFile.rdbuf();
    std::string vCode = vStream.str();
    std::string fCode = fStream.str();
    
    std::cout << "Vertex shader source:\n" << vCode << std::endl;
    std::cout << "Fragment shader source:\n" << fCode << std::endl;
    
    const char* vShaderCode = vCode.c_str();
    const char* fShaderCode = fCode.c_str();
    
    // Compile vertex shader
    unsigned int vertex = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex, 1, &vShaderCode, NULL);
    glCompileShader(vertex);
    
    // Check vertex shader compilation
    int success;
    char infoLog[512];
    glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(vertex, 512, NULL, infoLog);
        std::cerr << "ERROR: Vertex shader compilation failed\n" << infoLog << std::endl;
    } else {
        std::cout << "Vertex shader compiled successfully" << std::endl;
    }
    
    // Compile fragment shader
    unsigned int fragment = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment, 1, &fShaderCode, NULL);
    glCompileShader(fragment);
    
    // Check fragment shader compilation
    glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(fragment, 512, NULL, infoLog);
        std::cerr << "ERROR: Fragment shader compilation failed\n" << infoLog << std::endl;
    } else {
        std::cout << "Fragment shader compiled successfully" << std::endl;
    }
    
    // Link shaders
    ID = glCreateProgram();
    glAttachShader(ID, vertex);
    glAttachShader(ID, fragment);
    glLinkProgram(ID);
    
    // Check linking
    glGetProgramiv(ID, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(ID, 512, NULL, infoLog);
        std::cerr << "ERROR: Shader program linking failed\n" << infoLog << std::endl;
    } else {
        std::cout << "Shader program linked successfully" << std::endl;
    }
    
    // Clean up
    glDeleteShader(vertex);
    glDeleteShader(fragment);
}

void Shader::use() const {
    glUseProgram(ID);
}

void Shader::setMat4(const std::string& name, const glm::mat4& mat) const {
    int location = glGetUniformLocation(ID, name.c_str());
    if (location == -1) {
        std::cerr << "Warning: Uniform '" << name << "' not found in shader" << std::endl;
    }
    glUniformMatrix4fv(location, 1, GL_FALSE, &mat[0][0]);
}

void Shader::setVec3(const std::string& name, const glm::vec3& vec) const {
    int location = glGetUniformLocation(ID, name.c_str());
    if (location == -1) {
        std::cerr << "Warning: Uniform '" << name << "' not found in shader" << std::endl;
    }
    glUniform3fv(location, 1, &vec[0]);
}

void Shader::setFloat(const std::string& name, float val) const {
    int location = glGetUniformLocation(ID, name.c_str());
    if (location == -1) {
        std::cerr << "Warning: Uniform '" << name << "' not found in shader" << std::endl;
    }
    glUniform1f(location, val);
}