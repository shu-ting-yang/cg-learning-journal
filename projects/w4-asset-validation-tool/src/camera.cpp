// === camera.cpp ===
#include "camera.h"
#include <glm/gtc/matrix_transform.hpp>

Camera::Camera() : position(0.0f, 0.0f, 5.0f), pitch(0.0f), yaw(-90.0f) {}

glm::mat4 Camera::getViewMatrix() const {
    glm::vec3 front;
    front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    front.y = sin(glm::radians(pitch));
    front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    return glm::lookAt(position, position + glm::normalize(front), glm::vec3(0.0f, 1.0f, 0.0f));
}

void Camera::processInput(GLFWwindow* window) {
    const float speed = 0.05f;
    glm::vec3 front = glm::normalize(glm::vec3(
        cos(glm::radians(yaw)) * cos(glm::radians(pitch)),
        sin(glm::radians(pitch)),
        sin(glm::radians(yaw)) * cos(glm::radians(pitch))
    ));
    glm::vec3 right = glm::normalize(glm::cross(front, glm::vec3(0.0f, 1.0f, 0.0f)));
    glm::vec3 up = glm::normalize(glm::cross(right, front));

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) position += speed * front;
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) position -= speed * front;
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) position -= speed * right;
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) position += speed * right;
}
