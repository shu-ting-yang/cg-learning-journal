#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp> // for glm:lookAt, glm:perspective
#include <glm/gtc/type_ptr.hpp> //for glm::value_ptr

// add file loader utility
#include <fstream>
#include <sstream>
#include <string>

using namespace std;

//global variable
// for mvp matrix(updated inside rendered loop)
glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 3.0f);
glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);
// create projection matrix
glm::mat4 projection = glm::perspective(
    glm::radians(45.0f), // FOV in radians
    800.f / 600.f,       // aspect ratio
    0.1f,                // near plane
    100.0f               // far plane
);

//global variable for time and speed
float deltaTime = 0.0f;//Time between current fram and last frame
float lastFrame = 0.0f;//Time of last frame
float cameraSpeed = 2.5f;//adjustable movement speed
//for mouse control
float yaw = -90.0f;// rotate left
float pitch = 0.0f;
float lastX = 400.0f;
float lastY = 300.0f;
bool firstMouse = true;

//helper function to load shader string
std::string loadShaderSource(const std::string& filepath) {
    std::ifstream file(filepath);
    if (!file.is_open()) {
        std::cerr << "Failed to open shader file: " << filepath << std::endl;
        exit(-1);
    }
    std::stringstream buffer;
    buffer << file.rdbuf();
    return buffer.str();
}

std::string vertexCode = loadShaderSource("../shaders/vertex.glsl");
std::string fragmentCode = loadShaderSource("../shaders/fragment.glsl");

const char* vertex_shader_source = vertexCode.c_str();
const char* fragment_shader_source = fragmentCode.c_str();

void framebuffer_size_callback(GLFWwindow * window, int width, int height){
    glViewport(0, 0, width, height);//what's this?
    
}
void processInput(GLFWwindow* window, glm::vec3& cameraPos, glm::vec3 cameraFront, glm::vec3 cameraUp, float deltaTime){
    float velocity = cameraSpeed * deltaTime;
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        cameraPos += velocity * cameraFront;
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        cameraPos -= velocity * cameraFront;
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * velocity;
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * velocity;

}
void mouse_callback(GLFWwindow* window, double xpos, double ypos){
    static float sensitivity = 0.1f;
    if(firstMouse){
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }
    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos;

    lastX = xpos;
    lastY = ypos;
    xoffset *= sensitivity;
    yoffset *= sensitivity;
    yaw += xoffset;
    pitch += yoffset;
    // clamp pitch
    if (pitch > 89.0f)
        pitch = 89.0f;
    if (pitch < -89.0f)
        pitch = -89.0f;
    // update cameraFront
    glm::vec3 front;
    front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    front.y = sin(glm::radians(pitch));
    front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    cameraFront = glm::normalize(front);

}
int main(int, char**){
    std::cout << "Hello, from triangle_glfw!\n";
    // initialize glfw
    if(!glfwInit()){
        cerr << "failed to initialize glfw";
        return -1;
    }
    // configure glfw, opengl 4.1 core profile on macOS
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    // create a window
    GLFWwindow * window = glfwCreateWindow(800, 600, "My first opengl triange", nullptr, nullptr);
    
    if(!window) {
        cerr << "failed to create glfw window";
        glfwTerminate();
        return -1;
    }
    //what is this
    glfwMakeContextCurrent(window);

    // load opengl functions with glad
    if(!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        cerr << "failed to load glad";
        return -1;
    }
    /*********BEGIN after glad is loaded, create and complie shaders */
    // compile vertex shader
    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertex_shader_source, nullptr);
    glCompileShader(vertexShader);

    //check vertex shader compilation
    int success;
    char infoLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if(!success){
        glGetShaderInfoLog(vertexShader, 512, nullptr,infoLog);
        cerr << "Vertex shader compilation failed \n" << infoLog <<endl;
    }
    // compile fragment shader
    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragment_shader_source, nullptr);
    glCompileShader(fragmentShader);

    //check fragment shader compilation
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if(!success){
        glGetShaderInfoLog(fragmentShader, 512, nullptr,infoLog);
        cerr << "Fragment shader compilation failed \n" << infoLog <<endl;
    }
    //Link shaders into a program
   GLuint shaderProgram = glCreateProgram();
   glAttachShader(shaderProgram, vertexShader); 
   glAttachShader(shaderProgram, fragmentShader); 
   glLinkProgram(shaderProgram);

   glGetProgramiv(shaderProgram,GL_LINK_STATUS, &success);
   if(!success){
        glGetProgramInfoLog(shaderProgram, 512, nullptr,infoLog);
        cerr << "Program shader linking failed \n" << infoLog <<endl;
   }
   //delete shader objects after linking
   glDeleteShader(vertexShader);
   glDeleteShader(fragmentShader);

    

    /*********END after glad is loaded, create and complie shaders */
    /**** BEGIN draw the triangle */
   float vertices[] = {
    0.0f, 0.5f, 1.0f,
    -0.5f, -0.5f, 1.0f,
    0.5f, -0.5f, 1.0f
   };
   // upload vertex data(vbo + vao)
   // create vertex array object
   GLuint VAO;
   glGenVertexArrays(1,&VAO);
   glBindVertexArray(VAO);

   //create VBO, vertex buffer object
   GLuint VBO;
   glGenBuffers(1,&VBO);
   glBindBuffer(GL_ARRAY_BUFFER, VBO);
   
   //upload data to GPU
   glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

   //link vertex data to shader layout (location = 0)
   glVertexAttribPointer(
    0,3,GL_FLOAT,GL_FALSE,3*sizeof(float), (void*)0
   );
   glEnableVertexAttribArray(0);

   //unbind for safety
   glBindBuffer(GL_ARRAY_BUFFER,0);
   glBindVertexArray(0);

    /**** END draw the triangle */
    
    //set viewport and callback
    int width, height;
    glfwGetFramebufferSize(window, &width, &height);
    //glViewport(0,0,800,600);
    glViewport(0,0,width,height);
    //register mouse call back(after creating window and setting viewport)
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);


    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    //main render loop
    while(!glfwWindowShouldClose(window)){
        //input
        if(glfwGetKey(window, GLFW_KEY_ESCAPE)==GLFW_PRESS){
            glfwSetWindowShouldClose(window,true);
        }
        //wasd to move
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;
        processInput(window, cameraPos, cameraFront, cameraUp, deltaTime);
        // create view matrix
        glm::mat4 view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
        //create model matrix (firstly assume no object transform)
        glm::mat4 model = glm::mat4(1.0f);
        glm::mat4 mvp = projection * view * model;

        //update shader to multiply mvp matrix
        int mvpLocation = glGetUniformLocation(shaderProgram, "mvp");
        glUniformMatrix4fv(mvpLocation, 1, GL_FALSE, glm::value_ptr(mvp));

        //render
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glUseProgram(shaderProgram);
        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 3);

        //swap buffer and poll event
        glfwSwapBuffers(window);
        glfwPollEvents();

    }
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}
