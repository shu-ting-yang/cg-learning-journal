// === main.cpp ===
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>

#include "shader.h"
#include "camera.h"

struct Vertex {
    glm::vec3 position;
    glm::vec3 normal;
};

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

int main() {
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    #ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    #endif

    GLFWwindow* window = glfwCreateWindow(800, 600, "Lambertian Cube Viewer", NULL, NULL);
    glfwMakeContextCurrent(window);
    gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);

    glEnable(GL_DEPTH_TEST);

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

    Shader lambert1("shader/basic.vert", "shader/lambert1.frag");
    Shader lambert2("shader/basic.vert", "shader/lambert2.frag");

    Camera camera;
    glfwSetWindowUserPointer(window, &camera);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    int shaderMode = 1;

    while (!glfwWindowShouldClose(window)) {

        if(glfwGetKey(window, GLFW_KEY_ESCAPE)==GLFW_PRESS){
                glfwSetWindowShouldClose(window,true);
        } 

        glfwPollEvents();
        camera.processInput(window);

        if (glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS) shaderMode = 1;
        if (glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS) shaderMode = 2;

        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        Shader* activeShader = shaderMode == 1 ? &lambert1 : &lambert2;
        activeShader->use();

        glm::mat4 model = glm::mat4(1.0f);
        glm::mat4 view = camera.getViewMatrix();
        glm::mat4 proj = glm::perspective(glm::radians(45.0f), 800.0f/600.0f, 0.1f, 100.0f);
        activeShader->setMat4("model", model);
        activeShader->setMat4("view", view);
        activeShader->setMat4("projection", proj);
        activeShader->setVec3("lightDir", glm::normalize(glm::vec3(-1, -1, -1)));
        if (shaderMode == 2) activeShader->setFloat("Li", 1.0f);

        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, vertices.size());

        glfwSwapBuffers(window);
    }

    glfwTerminate();
    return 0;
}

