#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <fstream>
#include <sstream>
#include <string>
using namespace std;


/**** helper function to load shader string ****/
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
    GLFWwindow* window = glfwCreateWindow(800, 600, "Shader Playground", NULL, NULL);
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
    std::string vertexCode = loadShaderSource("../shaders/vertex.glsl");
    std::string fragmentCode = loadShaderSource("../shaders/01_checker.glsl");
    const char* vertex_shader_source = vertexCode.c_str();
    const char* fragment_shader_source = fragmentCode.c_str();

    // Print OpenGL version
    std::cout << "OpenGL version: " << glGetString(GL_VERSION) << "\n";
    float quadVertices[] = {
        //positions
        -1.0f, 1.0f,
        -1.0f, -1.0f,
        1.0f, -1.0f,

        -1.0f, 1.0f,
        1.0f, -1.0f,
        1.0f, 1.0f,
    };
    /************ setup VAO, VBO ***********/
    unsigned int quadVAO, quadVBO;
    glGenVertexArrays(1, &quadVAO);
    glGenBuffers(1, &quadVBO);
    
    glBindVertexArray(quadVAO);

    glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), quadVertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2* sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glBindVertexArray(0);//unbind VAO
    //Finish setting VAO, VBO

    /**************** Link shader code ************/
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

   /************** Grid size control ********/
   int gridSize = 6;

   /************** Render Loop ********/

    while(!glfwWindowShouldClose(window)){
        //input
        if(glfwGetKey(window, GLFW_KEY_ESCAPE)==GLFW_PRESS){
            glfwSetWindowShouldClose(window,true);
        }
        glClear(GL_COLOR_BUFFER_BIT);

        //get time and resolution
        float timeValue = glfwGetTime();
        int width, height;
        glfwGetFramebufferSize(window, &width, &height);

        //bind shader and set uniform
        glUseProgram(shaderProgram);
        glUniform1f(glGetUniformLocation(shaderProgram, "u_time"), timeValue);
        glUniform2f(glGetUniformLocation(shaderProgram, "u_resolution"), width, height);
        glUniform1i(glGetUniformLocation(shaderProgram, "u_grid_size"), gridSize);

        //draw fullscreen quad
        glBindVertexArray(quadVAO);
        glDrawArrays(GL_TRIANGLES, 0, 6);
        //swap buffer and poll event
        glfwSwapBuffers(window);
        glfwPollEvents();
        if(glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS) gridSize = 1;
        if(glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS) gridSize = 2;
        if(glfwGetKey(window, GLFW_KEY_3) == GLFW_PRESS) gridSize = 3;
        if(glfwGetKey(window, GLFW_KEY_4) == GLFW_PRESS) gridSize = 4;
        if(glfwGetKey(window, GLFW_KEY_5) == GLFW_PRESS) gridSize = 5;
        if(glfwGetKey(window, GLFW_KEY_6) == GLFW_PRESS) gridSize = 6;
        if(glfwGetKey(window, GLFW_KEY_7) == GLFW_PRESS) gridSize = 7;
        if(glfwGetKey(window, GLFW_KEY_8) == GLFW_PRESS) gridSize = 8;
        if(glfwGetKey(window, GLFW_KEY_9) == GLFW_PRESS) gridSize = 9;
    }
    glDeleteVertexArrays(1, &quadVAO);
    glDeleteBuffers(1, &quadVBO);
    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;

}