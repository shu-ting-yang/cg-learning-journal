#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <nlohmann/json.hpp>
#include "AssetLoader.h"
#include "Validator.h"
#include "shader.h"
#include "camera.h"

using json = nlohmann::json;
using namespace std;
struct Vertex {
    glm::vec3 position;
    glm::vec3 normal;
};

float lastX = 400, lastY = 300;
bool firstMouse = true;

void mouse_callback(GLFWwindow* window, double xpos, double ypos) {
    static Camera* cam = reinterpret_cast<Camera*>(glfwGetWindowUserPointer(window));
    if (!cam) return;

    if (firstMouse) {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }
    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos;
    lastX = xpos;
    lastY = ypos;

    const float sensitivity = 0.1f;
    cam->yaw += xoffset * sensitivity;
    cam->pitch += yoffset * sensitivity;

    if (cam->pitch > 89.0f) cam->pitch = 89.0f;
    if (cam->pitch < -89.0f) cam->pitch = -89.0f;
}

std::vector<Vertex> createCube() {
    std::vector<Vertex> vertices;
    glm::vec3 normals[] = {
        { 0,  0,  1}, { 0,  0, -1}, { 1,  0,  0},
        {-1,  0,  0}, { 0,  1,  0}, { 0, -1,  0},
    };
    glm::vec3 positions[][4] = {
        { {-1,-1, 1}, { 1,-1, 1}, { 1, 1, 1}, {-1, 1, 1} },
        { { 1,-1,-1}, {-1,-1,-1}, {-1, 1,-1}, { 1, 1,-1} },
        { { 1,-1, 1}, { 1,-1,-1}, { 1, 1,-1}, { 1, 1, 1} },
        { {-1,-1,-1}, {-1,-1, 1}, {-1, 1, 1}, {-1, 1,-1} },
        { {-1, 1, 1}, { 1, 1, 1}, { 1, 1,-1}, {-1, 1,-1} },
        { {-1,-1,-1}, { 1,-1,-1}, { 1,-1, 1}, {-1,-1, 1} },
    };
    for (int f = 0; f < 6; ++f) {
        Vertex v0 = {positions[f][0], normals[f]};
        Vertex v1 = {positions[f][1], normals[f]};
        Vertex v2 = {positions[f][2], normals[f]};
        Vertex v3 = {positions[f][3], normals[f]};
        vertices.push_back(v0); vertices.push_back(v1); vertices.push_back(v2);
        vertices.push_back(v2); vertices.push_back(v3); vertices.push_back(v0);
    }
    return vertices;
}
// Add this after your OpenGL setup, before your render loop:
void testAssetLoading() {
    AssetLoader loader;
    Validator validator;
    
    // Test with a simple model (you'll need to download one)
    AssetInfo info = loader.loadAsset("../assets/test_models/Box.gltf");
    
    std::cout << "=== ASSET LOADING TEST ===" << std::endl;
    std::cout << "File: " << info.filename << std::endl;
    
    if (info.isValid) {
        std::cout << "✅ Loaded successfully" << std::endl;
        std::cout << "Meshes: " << info.meshCount << std::endl;
        std::cout << "Materials: " << info.materialCount << std::endl;
        
        ValidationResult validation = validator.validateAsset(info);
        std::cout << "Validation: " << (validation.passed ? "✅ PASSED" : "❌ FAILED") << std::endl;
        
        for (const auto& warning : validation.warnings) {
            std::cout << "⚠️  " << warning << std::endl;
        }
        for (const auto& error : validation.errors) {
            std::cout << "❌ " << error << std::endl;
        }
    } else {
        std::cout << "❌ Failed: " << info.errorMessage << std::endl;
    }
}

int main() {
    // Initialize GLFW
    if (!glfwInit()) {
        std::cerr << "Failed to initialize GLFW\n";
        return -1;
    }
    // Set OpenGL version (important for compatibility)
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    #ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    #endif

    std::cout << "Creating window..." << std::endl;

    // Create a windowed mode window and its OpenGL context
    GLFWwindow* window = glfwCreateWindow(800, 600, "Asset Pipeline", NULL, NULL);
    if (!window) {
        std::cerr << "Failed to create GLFW window\n";
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);

    // Load GLAD (load all OpenGL function pointers)
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cerr << "Failed to initialize GLAD\n";
        return -1;
    }
    glEnable(GL_DEPTH_TEST);

    // set up vao vbo -> should be replaced by gltf load
    
    auto vertices = createCube();

    GLuint VAO, VBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), vertices.data(), GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));
    glEnableVertexAttribArray(1);
    // set up vao vbo -> should be replaced by gltf load

    

    // Print OpenGL version
    std::cout << "OpenGL version: " << glGetString(GL_VERSION) << "\n";
    Shader lambert2("shader/basic.vert", "shader/lambert2.frag");

    Camera camera;
    glfwSetWindowUserPointer(window, &camera);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

   /************** Test asset loading ********/
   testAssetLoading();
   /************** Render Loop ********/

    while(!glfwWindowShouldClose(window)){
        //input
        if(glfwGetKey(window, GLFW_KEY_ESCAPE)==GLFW_PRESS){
            glfwSetWindowShouldClose(window,true);
        }
        glClear(GL_COLOR_BUFFER_BIT);

        glfwPollEvents();
        camera.processInput(window);
        
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        
        Shader* activeShader = &lambert2;
        activeShader->use();
        glm::mat4 model = glm::mat4(1.0f);
        glm::mat4 view = camera.getViewMatrix();
        glm::mat4 proj = glm::perspective(glm::radians(45.0f), 800.0f/600.0f, 0.1f, 100.0f);
        activeShader->setMat4("model", model);
        activeShader->setMat4("view", view);
        activeShader->setMat4("projection", proj);
        activeShader->setVec3("lightDir", glm::normalize(glm::vec3(-1, -1, -1)));
        activeShader->setFloat("Li", 1.0f);

        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, vertices.size());
        glfwSwapBuffers(window);
    }
    glfwTerminate();
    return 0;

}