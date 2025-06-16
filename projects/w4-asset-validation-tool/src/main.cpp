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

// Structure to hold OpenGL render data for a mesh
struct MeshRenderData {
    GLuint VAO = 0;
    GLuint VBO = 0;
    GLuint EBO = 0;
    size_t indexCount = 0;
    std::string name;
    
    // Cleanup OpenGL resources
    ~MeshRenderData() {
        if (VAO) glDeleteVertexArrays(1, &VAO);
        if (VBO) glDeleteBuffers(1, &VBO);
        if (EBO) glDeleteBuffers(1, &EBO);
    }
    
    // Delete copy constructor and assignment to avoid double deletion
    MeshRenderData(const MeshRenderData&) = delete;
    MeshRenderData& operator=(const MeshRenderData&) = delete;
    
    // Allow move semantics
    MeshRenderData(MeshRenderData&& other) noexcept 
        : VAO(other.VAO), VBO(other.VBO), EBO(other.EBO), 
          indexCount(other.indexCount), name(std::move(other.name)) {
        other.VAO = other.VBO = other.EBO = 0;
    }
    
    MeshRenderData& operator=(MeshRenderData&& other) noexcept {
        if (this != &other) {
            // Clean up existing resources
            if (VAO) glDeleteVertexArrays(1, &VAO);
            if (VBO) glDeleteBuffers(1, &VBO);
            if (EBO) glDeleteBuffers(1, &EBO);
            
            // Move data
            VAO = other.VAO;
            VBO = other.VBO;
            EBO = other.EBO;
            indexCount = other.indexCount;
            name = std::move(other.name);
            
            // Reset other object
            other.VAO = other.VBO = other.EBO = 0;
        }
        return *this;
    }
    
    MeshRenderData() = default;
};

// Function to create OpenGL buffers from loaded mesh data
MeshRenderData createMeshRenderData(const MeshData& meshData) {
    MeshRenderData renderData;
    renderData.name = meshData.name;
    renderData.indexCount = meshData.indices.size();
    
    // 1. Generate VAO, VBO, and EBO
    glGenVertexArrays(1, &renderData.VAO);
    glGenBuffers(1, &renderData.VBO);
    glGenBuffers(1, &renderData.EBO);
    
    // 2. Bind VAO
    glBindVertexArray(renderData.VAO);
    
    // 3. Upload vertex data to VBO
    glBindBuffer(GL_ARRAY_BUFFER, renderData.VBO);
    glBufferData(GL_ARRAY_BUFFER, meshData.vertices.size() * sizeof(Vertex), meshData.vertices.data(), GL_STATIC_DRAW);
    
    // 4. Upload index data to EBO
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, renderData.EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, meshData.indices.size() * sizeof(unsigned int), meshData.indices.data(), GL_STATIC_DRAW);
    
    // 5. Set up vertex attributes
    // Position attribute (location = 0)
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
    glEnableVertexAttribArray(0);
    
    // Normal attribute (location = 1)
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));
    glEnableVertexAttribArray(1);
    
    // Texture coordinate attribute (location = 2)
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, texCoords));
    glEnableVertexAttribArray(2);
    
    // 6. Unbind VAO
    glBindVertexArray(0);
    
    return renderData;
}

// Function to render a mesh
void renderMesh(const MeshRenderData& renderData) {
    // 1. Bind the VAO
    glBindVertexArray(renderData.VAO);
    
    // 2. Draw the mesh using indices
    glDrawElements(GL_TRIANGLES, renderData.indexCount, GL_UNSIGNED_INT, 0);
    
    // 3. Unbind VAO (optional, but good practice)
    glBindVertexArray(0);
}

// Function to load and setup a 3D model for rendering
std::vector<MeshRenderData> loadModel(const std::string& filepath) {
    std::vector<MeshRenderData> renderDataList;
    
    AssetLoader loader;
    AssetInfo info = loader.loadAsset(filepath);
    
    if (!info.isValid) {
        std::cerr << "Failed to load model: " << info.errorMessage << std::endl;
        return renderDataList;
    }
    
    std::cout << "Loaded model: " << info.filename << std::endl;
    std::cout << "Meshes: " << info.meshCount << std::endl;
    std::cout << "Total vertices: " << info.totalVerticesCount << std::endl;
    
    // Create render data for each mesh
    renderDataList.reserve(info.meshes.size());
    for (const auto& meshData : info.meshes) {
        std::cout << "Processing mesh: " << meshData.name 
                  << " (vertices: " << meshData.vertices.size() 
                  << ", indices: " << meshData.indices.size() << ")" << std::endl;
        
        renderDataList.push_back(createMeshRenderData(meshData));
    }
    
    return renderDataList;
}

// Test function for asset loading
void testAssetLoading() {
    AssetLoader loader;
    Validator validator;
    
    // Test with a simple model (you'll need to download one)
    AssetInfo info = loader.loadAsset("../assets/test_models/Box.gltf");
    
    std::cout << "=== ASSET LOADING TEST ===" << std::endl;
    std::cout << "File: " << info.filename << std::endl;
    
    if (info.isValid) {
        std::cout << "[O] Loaded successfully" << std::endl;
        std::cout << "Meshes: " << info.meshCount << std::endl;
        std::cout << "Materials: " << info.materialCount << std::endl;
        
        ValidationResult validation = validator.validateAsset(info);
        std::cout << "Validation: " << (validation.passed ? "PASSED" : "FAILED") << std::endl;
        
        for (const auto& warning : validation.warnings) {
            std::cout << "[!]  " << warning << std::endl;
        }
        for (const auto& error : validation.errors) {
            std::cout << "[X] " << error << std::endl;
        }
    } else {
        std::cout << "[X] Failed: " << info.errorMessage << std::endl;
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

    // Print OpenGL version
    std::cout << "OpenGL version: " << glGetString(GL_VERSION) << "\n";
    
    // Setup shaders
    Shader lambert2("shader/basic.vert", "shader/lambert2.frag");

    // Setup camera
    Camera camera;
    glfwSetWindowUserPointer(window, &camera);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    /************** Test asset loading ********/
    testAssetLoading();
    
    /************** Load 3D Model ********/
    // TODO: Replace with your actual model path
    std::string modelPath = "../assets/test_models/Box.gltf";
    std::vector<MeshRenderData> modelMeshes = loadModel(modelPath);
    
    if (modelMeshes.empty()) {
        std::cerr << "No meshes loaded! Using fallback..." << std::endl;
        // You could add a fallback cube here if needed
        glfwTerminate();
        return -1;
    }
    
    /************** Render Loop ********/
    while(!glfwWindowShouldClose(window)) {
        // Input
        if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
            glfwSetWindowShouldClose(window, true);
        }
        
        glfwPollEvents();
        camera.processInput(window);
        
        // Clear buffers
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        
        // Setup matrices
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
        
        // Render all meshes from the loaded model
        for (const auto& meshRenderData : modelMeshes) {
            renderMesh(meshRenderData);
        }
        
        glfwSwapBuffers(window);
    }
    
    glfwTerminate();
    return 0;
}