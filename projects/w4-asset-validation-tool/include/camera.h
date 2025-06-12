// === camera.h ===
#ifndef CAMERA_H
#define CAMERA_H
#include <glm/glm.hpp>
#include <GLFW/glfw3.h>

class Camera {
public:
    glm::vec3 position;
    float pitch, yaw;
    Camera();
    glm::mat4 getViewMatrix() const;
    void processInput(GLFWwindow* window);
};

#endif
