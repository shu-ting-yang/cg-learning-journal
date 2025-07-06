//GLAD is a library that manages OpenGL function pointers
#include <glad/glad.h> // include glad to get all the required OpenGL headers
#include <GLFW/glfw3.h>// create an OpenGL context, define a window, and handle inputs
#include <iostream> // for input and output operations
#include <fstream>
#include <sstream>
#include <string>
using namespace std;

float vertices[] = {
  // positions
    0.5f,  0.5f, 0.0f,  // top right
     0.5f, -0.5f, 0.0f,  // bottom right
    -0.5f, -0.5f, 0.0f,  // bottom left
    -0.5f,  0.5f, 0.0f   // top left 
};
unsigned int indices[] = {  // note that we start from 0!
    0, 1, 3,   // first triangle
    1, 2, 3    // second triangle
};

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
  // Make sure the viewport matches the new window dimensions; note that width and
  // height will be significantly larger than specified on retina displays.
  glViewport(0, 0, width, height);
}
void processInput(GLFWwindow* window)
{
  if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) // If the escape key is pressed, close the window
    glfwSetWindowShouldClose(window, true);
}

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
int main()
{
  glfwInit();
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);// Telling GLFW we want to use the core-profile means we'll get access to a smaller subset of OpenGL features without backwards-compatible features we no longer need.
#ifdef __APPLE__
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

  GLFWwindow* window = glfwCreateWindow(800, 600, "Learn OpenGL", NULL, NULL);
  if (window == NULL)
  {
    std::cout << "Failed to create GLFW window" << std::endl;
    glfwTerminate();
    return -1;
  }
  glfwMakeContextCurrent(window);// Make the window's context on the current thread
  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) //we pass the function pointer to GLAD, to load the addresses of the OpenGL functions pointers
  {
    std::cout << "Failed to initialize GLAD" << std::endl;
    return -1;
  }
  glViewport(0, 0, 800, 600); // Set the viewport size to the window size
                              //Behind the scenes OpenGL uses the data specified via glViewport to transform the 2D coordinates it processed to coordinates on your screen. For example, a processed point of location (-0.5,0.5) would (as its final transformation) be mapped to (200,450) in screen coordinates.
  glfwSetFramebufferSizeCallback(window, framebuffer_size_callback); // Set the viewport size to the window size when the window is resized
  // VBO and VAO setup
  unsigned int VAO;
  unsigned int VBO;
  unsigned int EBO; // Element Buffer Object for indexed drawing
  glGenVertexArrays(1, &VAO); // Generate one vertex array object, with the ID stored in VAO
  glGenBuffers(1, &VBO); // Generate one buffer object, with the ID stored in VBO
  glGenBuffers(1, &EBO); // Generate one element buffer object, with the ID stored in EBO

  // SETUP PHASE - Configure what the VAO should remember
  glBindVertexArray(VAO); // Bind the vertex array object, which will store the vertex attributes
                          // - "Start recording my vertex setup to the VAO"
  glBindBuffer(GL_ARRAY_BUFFER, VBO); // Any buffer calls will now be used to configure the currently bound buffer object, which is the VBO
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO); // Bind the element buffer object, which will store the indices for indexed drawing

  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW); // Copy the vertex data to the buffer's memory
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW); // Copy the index data to the element buffer's memory

  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0); // Specify the layout of the vertex data
  glEnableVertexAttribArray(0); // Enable the vertex attribute array at index 0

  // Load and compile vertex shader
  std::string vertexShaderSource = loadShaderSource("../shaders/vertex.glsl");
  unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER); // Create a vertex shader object
  const char* vertexShaderCode = vertexShaderSource.c_str(); // Convert the shader source
  glShaderSource(vertexShader, 1, &vertexShaderCode, NULL); // Attach the shader source code to the shader object
  glCompileShader(vertexShader); // Compile the vertex shader
  // Check for compilation errors
  int success;
  char infoLog[512];
  glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success); // Check if the shader compiled successfully
  if (!success)
  {
    glGetShaderInfoLog(vertexShader, 512, NULL, infoLog); // Get the error log
    std::cout << "ERROR::SHADER::VERTEX::COMPILE_FAILED\n"
              << infoLog << std::endl; // Print the error log
  }
  // Load and compile fragment shader
  std::string fragmentShaderSource = loadShaderSource("../shaders/fragment.glsl");
  unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER); // Create a fragment shader object
  const char* fragmentShaderCode = fragmentShaderSource.c_str(); // Convert the shader source
  glShaderSource(fragmentShader, 1, &fragmentShaderCode, NULL); // Attach the shader source code to the shader object
  glCompileShader(fragmentShader); // Compile the fragment shader
  // Check for compilation errors
  glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success); // Check if the shader compiled successfully
  if (!success)
  {
    glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog); // Get the error log
    std::cout << "ERROR::SHADER::FRAGMENT::COMPILE_FAILED\n"
              << infoLog << std::endl; // Print the error log
  }
  // link shaders into a shader program(a collection of shaders that are used together)
  // The activated shader program is the one that will be used when we issue render calls.
  unsigned int shaderProgram = glCreateProgram(); // Create a shader program object
  glAttachShader(shaderProgram, vertexShader); // Attach the vertex shader to the program
  glAttachShader(shaderProgram, fragmentShader); // Attach the fragment shader to the program
  glLinkProgram(shaderProgram); // Link the shaders into a program
  // Check for linking errors
  glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success); // Check if the program linked successfully
  if (!success)
  {
    glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog); // Get the error log
    std::cout << "ERROR::SHADER::PROGRAM::LINK_FAILED\n"
              << infoLog << std::endl; // Print the error log
  }

  // Delete the shader objects after linking them into the program
  glDeleteShader(vertexShader); // Delete the vertex shader object
  glDeleteShader(fragmentShader); // Delete the fragment shader object
  
  // Main loop
  while (!glfwWindowShouldClose(window))
  {
    // Input
    processInput(window); // Process input (e.g. keyboard input, mouse movement, etc.)
    glfwSwapBuffers(window); // Swap the front and back buffers
    // Render
    // Here you would typically clear the screen, draw your objects, etc.
    // glClear is a state-setting function and glClear is a state-using functino in the it uses the current state to retrieve the clearning color from and glClear is a state-using functino in the it uses the current state to retrieve the clearning color from.
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f); // Set the clear color to a dark blue
    glClear(GL_COLOR_BUFFER_BIT); // Clear the color buffer with the specified clear color
    glUseProgram(shaderProgram); // Use the shader program for rendering
    // RENDER PHASE - Activate the VAO's remebered configuration
    glBindVertexArray(VAO); // Bind the vertex array object, which contains the vertex attributes
                            // - "Use the setup you remembered in the VAO"
    //glDrawArrays(GL_TRIANGLES, 0, 3); // Use VBO to draw the triangle
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0); // Use EBO to draw the triangles
    glBindVertexArray(0);
    // Check for events and call the corresponding callback functions
    glfwPollEvents(); // Poll for and process events(e.g. keyboard input, mouse movement, etc.)
  }
  glfwTerminate(); // Terminate GLFW, clearing any resources allocated by GLFW
  std::cout << "GLFW terminated successfully" << std::endl;
  return 0;
  
}